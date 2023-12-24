#pragma once
#include <gl/glut.h>
#include <vector>
#include <string>

class MenuItem {
    std::string name;
public:
    enum MenuItemType {
        SubMenu = 0,
        Command
    } const Type;
    MenuItem(const std::string& name, MenuItemType type) : name(name), Type(type) {}
    void Draw(GLint x, GLint y, GLint menu_item_width, GLint menu_item_height);
};

class SubMenu : public MenuItem {
    std::vector<int> sub_menu_;
public:
    SubMenu(const std::string& name) : MenuItem(name, MenuItemType::SubMenu) {}
    void AddSubMenu(int menu_id);
    friend class Menu;
};

class CommandMenuItem : public MenuItem {
    void (*func_)(std::vector<int>&);
public:
    CommandMenuItem(const std::string& name, void (*func_)(std::vector<int>&)) : MenuItem(name, MenuItemType::Command), func_(func_) {}
    void Hit(std::vector<int>& menu_id);
};

class Menu {
    const int n_menu_item_horizon = 3;
    const int n_menu_item_vertical = 8;
    const int menu_item_height = 20;
    const int menu_item_width = 100;
    int n_root_nemu_item;
    std::vector<MenuItem*> menu_items_;
    std::vector<std::vector<int>> menu_show_;
    std::vector<int> menu_have_hit;
public:
    Menu(int n_root_nemu_item);
    int AddItem(MenuItem* item);
    void AddRootItem(MenuItem* item);
    int ItemNum() { return menu_items_.size(); }
    void Draw(GLint x, GLint y);
    bool Hit(GLint x, GLint y);
    ~Menu();
};