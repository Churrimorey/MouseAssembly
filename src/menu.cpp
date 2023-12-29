#include "menu.h"
#include <iostream>
#ifndef MACOS
#include <thread>
#include <winsock.h>
#pragma comment (lib,"ws2_32.lib")
#define PORT 5240
#endif

using namespace std;

bool ARabout::DrawAbout = false;
bool ARabout::large_target = false;
#ifndef MACOS
SOCKET ARabout::server;
SOCKET ARabout::client;
Menu ARabout::submenu(3);
#endif

void MenuItem::Draw(GLint x, GLint y, GLint menu_item_width, GLint menu_item_height) {
    glPushMatrix();
    glRasterPos2i(x+5, y+6);
    for (char c : name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + menu_item_width, y);
    glVertex2i(x + menu_item_width, y + menu_item_height);
    glVertex2i(x, y + menu_item_height);
    glEnd();
    glPopMatrix();
}

void SubMenu::AddSubMenu(int menu_id) {
    sub_menu_.push_back(menu_id);
}

void CommandMenuItem::Hit(vector<int>& menu_id) {
    func_(menu_id);
}

Menu::Menu(int n_root_nemu_item) : n_root_nemu_item(n_root_nemu_item) {
    for (int i=0; i < n_menu_item_horizon; i++) {
        vector<int> tmp;
        for (int j=0; j < n_menu_item_vertical; j++) {
            tmp.push_back(-1);
        }
        menu_show_.push_back(tmp);
        menu_have_hit.push_back(-1);
    }

    for (int i=0; i < n_root_nemu_item; i++) {
        CommandMenuItem* item = new CommandMenuItem("Root"+to_string(i), [](vector<int>& menu_id) {
            printf("Root%d\n", menu_id[0]);
        });
        menu_items_.push_back(item);
        menu_show_[0][i] = i;
    }

    i_root_nemu_item = 0;
}

int Menu::AddItem(MenuItem* item) {
    menu_items_.push_back(item);
    return menu_items_.size() - 1;
}

void Menu::AddRootItem(MenuItem* item) {
    if (menu_items_[i_root_nemu_item]->Type == MenuItem::MenuItemType::SubMenu) {
        delete (SubMenu *)menu_items_[i_root_nemu_item];
    }
    else if (menu_items_[i_root_nemu_item]->Type == MenuItem::MenuItemType::Command) {
        delete (CommandMenuItem *)menu_items_[i_root_nemu_item];
    }
    menu_items_[i_root_nemu_item++] = item;
}

void Menu::Draw(GLint x, GLint y) {
    glDisable(GL_LIGHTING);
    for (int i=0; i < n_menu_item_horizon && menu_show_[i][0] >= 0; i++) {
        for (int j=0; j < n_menu_item_vertical && menu_show_[i][j] >= 0; j++) {
            if (menu_show_[i][j] >= 0 && menu_show_[i][j] < menu_items_.size()) {
                if (menu_have_hit[i] == menu_show_[i][j]) {
                    glColor3f(0.0f, 0.3f, 1.0f);
                }
                else {
                    glColor3f(0.0f, 0.0f, 0.0f);
                }
                menu_items_[menu_show_[i][j]]->Draw(x + i * menu_item_width, y + j * menu_item_height, menu_item_width, menu_item_height);
            }
        }
    }
    glEnable(GL_LIGHTING);
}

bool Menu::Hit(GLint x, GLint y) {
    bool res = false;
    int i = x / menu_item_width;
    int j = y / menu_item_height;
    if (i < n_menu_item_horizon && j < n_menu_item_vertical) {
        if (menu_show_[i][j] >= 0 && menu_show_[i][j] < menu_items_.size()) {
            MenuItem *item = menu_items_[menu_show_[i][j]];
            menu_have_hit[i] = menu_show_[i][j];
            if (item->Type == MenuItem::MenuItemType::SubMenu) {
                SubMenu *sub_menu = (SubMenu *)item;
                menu_have_hit[i] = menu_show_[i][j];
                for (int u=i+1; u < n_menu_item_horizon; u++) {
                    for (int v=0; v < n_menu_item_vertical; v++) {
                        menu_show_[u][v] = -1;
                    }
                    menu_have_hit[u] = -1;
                }
                for (int v=0; v < sub_menu->sub_menu_.size(); v++) {
                    menu_show_[i+1][v] = sub_menu->sub_menu_[v];
                }
                res = true;
            }
            else if (item->Type == MenuItem::MenuItemType::Command) {
                CommandMenuItem *command = (CommandMenuItem *)item;
                command->Hit(menu_have_hit);
                res = true;
            }
        }
    }

    if (res == false) {
        for (int i=0; i < n_menu_item_horizon; i++) {
            for (int j=0; j < n_menu_item_vertical; j++) {
                menu_show_[i][j] = -1;
            }
            menu_have_hit[i] = -1;
        }
        for (j=0; j < n_root_nemu_item; j++) {
            menu_show_[0][j] = j;
        }
    }

    return res;
}

Menu::~Menu() {
    for (int i=0; i < menu_items_.size(); i++) {
        if (menu_items_[i]->Type == MenuItem::MenuItemType::SubMenu) {
            delete (SubMenu *)menu_items_[i];
        }
        else if (menu_items_[i]->Type == MenuItem::MenuItemType::Command) {
            delete (CommandMenuItem *)menu_items_[i];
        }
    }
}

void ARabout::InitAR(Menu& menu) {
    CommandMenuItem* about_menu = new CommandMenuItem("About", ARabout::Enable);
    menu.AddRootItem(about_menu);
    #ifndef MACOS
    thread listen_thread;
    listen_thread = thread(server_listen);
    listen_thread.detach();

    CommandMenuItem* show_base = new CommandMenuItem("Show Base", [](vector<int>& menu_id) {
		send_message(client, "Show Base");
	});

    CommandMenuItem* show_tip = new CommandMenuItem("Show Tip", [](vector<int>& menu_id) {
        send_message(client, "Show Tip");
    });

    CommandMenuItem* show_battery = new CommandMenuItem("Show Battery", [](vector<int>& menu_id) {
        send_message(client, "Show Battery");
    });

    submenu.AddRootItem(show_base);
    submenu.AddRootItem(show_battery);
    submenu.AddRootItem(show_tip);
    #endif
}


#ifndef MACOS
void ARabout::server_listen() {
    int len;

    thread thread_recv;

    SOCKADDR_IN server_addr;
    SOCKADDR_IN client_addr;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    server = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0"); 
    server_addr.sin_port = htons(PORT); 
    if (bind(server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == -1) {
		closesocket(server);
		WSACleanup();
		return;
	}

    listen(server, 10);

    while (true) {
        len = sizeof(SOCKADDR);
        client = accept(server, (SOCKADDR*)&client_addr, &len);

        thread_recv = thread(receive_message, client);
        thread_recv.join();
    }
}
#endif

void ARabout::Draw() {
    if (DrawAbout) {
        glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, texture[2]);

        glBegin(GL_QUADS);
        int height = large_target ? 360 : 240;
        int width = large_target ? 270 : 180;
        glTexCoord2f(0.0f, 0.0f); glVertex2i(100, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(width+100, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(width+100, height);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(100, height);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        #ifndef MACOS
        submenu.Draw(0, 0);
        #endif
    }
}

bool ARabout::Hit(GLint x, GLint y) {
    if (DrawAbout) {
        #ifndef MACOS
        if (!submenu.Hit(x, y)) {
            DrawAbout = false;
        }
        #else
        DrawAbout = false;
        #endif
        return true;
    }
    else {
        return false;
    }
}

void ARabout::Enable(vector<int>& menu_id) {
    DrawAbout = true;
}

#ifndef MACOS
void ARabout::send_message(SOCKET client, const char* msg) {
    cout << msg << endl;
    send(client, msg, strlen(msg), 0);
}

void ARabout::receive_message(SOCKET client) {
    char recv_buf[1460];//���ջ�����

    while (true) {
        // Receive message from the server
        recv_buf[recv(client, recv_buf, 1459, 0)] = '\0';
        if (recv_buf[0] == '\0')
            break;
        cout << recv_buf << endl;
        if (strcmp(recv_buf, "target") == 0) {
            large_target = !large_target;
        }
    }
}
#endif