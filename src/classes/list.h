#pragma once

#include "pch.h"
#include "extern/cocos.h"
#define LIST_LENGTH 10
#define FCNPTR_T void*
#define BUTTON_T void*

class listManager;


/*the list class embodies all elements needed to form a list: a title, the actual list, navigation buttons,
* swap list element buttons, and a button to move the list to its target list.
*/

class horizontalList {
protected:
    const char* m_titleStr{};
    void* m_titleLabel{};

    std::vector<std::string> m_listStrings{};
    void** m_pArrListLabels{};
    int m_listLength{};
    bool m_init{ false };
    float m_x{}, m_y{};

    BUTTON_T m_upBtn{};
    BUTTON_T m_downBtn{};
    FCNPTR_T m_navFn{};
    int m_listOffset{ 0 };

    void* m_menu{};

protected:
    void toggle(void*& button, bool enabled);
    virtual void getLength() = 0;

    virtual bool isParent(void* button);
    virtual bool isUp(void* button);

    virtual void navigate(bool sub);
    virtual void swap(bool up) = 0;
    virtual void move() = 0;

    virtual void update();
    virtual void enter(void* scene);
    virtual void exit();

public:
    horizontalList(const char* title, int length);
    
    void setPosition(float x, float y);

    friend class listManager;
};

class verticalList : public horizontalList {
    enum buttonType {
        NAVIGATE,
        SWAP,
        MOVE
    };
    struct listAttributes {
        buttonType m_type;
        bool m_up;
    };

protected:
    BUTTON_T m_swapUpBtn{};
    BUTTON_T m_swapDownBtn{};
    FCNPTR_T m_swapFn{};

    BUTTON_T m_moveBtn{};
    FCNPTR_T m_moveFn{};
    int m_moveIndex{};

    void* m_menu{};

    verticalList* m_target;

protected:
    virtual void getLength();

    virtual bool isParent(void* button);
    virtual bool isUp(void* button);

    virtual void navigate(bool sub);
    virtual void swap(bool up);
    virtual void move();

    void updateLabels();
    void updateSelector();
    virtual void update();
    
    virtual void enter(void* scene);
    virtual void exit();

public:
    verticalList(const char* title, int length, verticalList* target);
    void loadArray();

    std::vector<std::string>& getArray();

    friend class listManager;
};

/*listManager keeps track of all lists using a vector, which allows for callbacks to be cleaner and for full encapsulation of functionality.
* although it's probably very slow if you have too many lists or something lol idk
* 
* the alternative was to make my own inherited class from cocos and i dont know how tf that works soooo
*/

class listManager {
private:
    static inline std::vector<horizontalList*> m_vec;
    static inline bool m_up;

private:
    static void add(horizontalList* list);

    static void __stdcall navigate(void* pSender);
    static void __stdcall swap(void* pSender);
    static void __stdcall move(void* pSender);

public:
    static void enter(void* scene);
    static void exit();

    friend class verticalList;
    friend class horizontalList;
};