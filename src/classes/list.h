#pragma once

#include "pch.h"
#include "extern/cocos.h"
#define LIST_LENGTH 10

class listManager;

/*the list class embodies all elements needed to form a list: a title, the actual list, navigation buttons,
* swap list element buttons, and a button to move the list to its target list.
*/

class list {
private:

    const char* m_titleStr{};
    void* m_titleLabel{};

    std::vector<std::string> m_listStrings{};
    void* m_listLabels[LIST_LENGTH]{};
    int m_listLength{};
    bool m_init{};
    float m_x{}, m_y{};

    void* m_upBtn{};
    void* m_downBtn{};
    void* m_navFn{};
    int m_listOffset{};

    void* m_moveBtn{};
    void* m_moveFn{};
    int m_moveIndex{};

    void* m_swapUpBtn{};
    void* m_swapDownBtn{};
    void* m_swapFn{};

    void* m_menu{};

    list* m_target;

    void getLength();
    void toggle(void*& button, bool enabled);
    void updateLabels();
    void updateSelector();

public:

    list(const char* title, list* target, listManager* manager);
    void loadArray();
    std::vector<std::string>& getArray();
    void setPosition(float x, float y);
    void enter(void* scene);
    void update();
    void exit();

    friend class listManager;
};

/*listManager keeps track of all lists using a vector, which allows for callbacks to be cleaner and for full encapsulation of functionality.
* although it's probably very slow if you have too many lists or something lol idk
* 
* the alternative was to make my own inherited class from cocos and i dont know how tf that works soooo
*/

class listManager {
    static inline std::vector<list*> listOfLists;
    static inline bool up;

private:
    static list* getAttributes(void* pSender);

public:
    static void __stdcall navigate(void* pSender);
    static void __stdcall swap(void* pSender);
    static void __stdcall move(void* pSender);

    static void add(list* list);
    static void enter(void* scene);
    static void exit();
};