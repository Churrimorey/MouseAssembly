#include "menu.h"

using namespace std;

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
}

int Menu::AddItem(MenuItem* item) {
    menu_items_.push_back(item);
    return menu_items_.size() - 1;
}

void Menu::AddRootItem(MenuItem* item) {
    static int i = 0;
    if (menu_items_[i]->Type == MenuItem::MenuItemType::SubMenu) {
        delete (SubMenu *)menu_items_[i];
    }
    else if (menu_items_[i]->Type == MenuItem::MenuItemType::Command) {
        delete (CommandMenuItem *)menu_items_[i];
    }
    menu_items_[i++] = item;
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