#pragma once

#include "pch.h"
#include "extern/cocos.h"

class listManager;

//the list class embodies all elements needed to form a list: a title, the actual list, and navigation buttons.

class list {
protected:
    //for more "understandability" i guess, better than macros i think?
    typedef void* button_t;
    typedef void* label_t;
    typedef void* menu_t;

protected:
    const char* m_titleStr{};
    label_t m_titleLabel{};

    std::vector<std::string> m_listStrings{};
    label_t* m_listLabels{};
    int m_maxDisplayedLength{};
    int m_displayedLength{};
    bool m_entered{ false };
    float m_x{}, m_y{};

    button_t m_upBtn{};
    button_t m_downBtn{};
    void(__stdcall* const m_navFn)(void* pSender) {};
    int m_listOffset{ 0 };

    menu_t m_menu{};

protected:
    void getLength();
    void toggle(void*& button, bool enabled);

    virtual bool isParent(void* button);
    virtual bool isUp(void* button);

    virtual void navigate(bool up);

    virtual void update();
    virtual void enter(void* scene);
    virtual void exit();

    virtual bool load(void* file);
    virtual void save(void* file);

public:
    list(const char* title, int length);
    void setVector(const std::vector<std::string>& vec);
    virtual void ifNotFound(const std::vector<std::string>& other, bool add);
    const std::vector<std::string>& getVector();
    const int getCurrentIndex();
    
    void setPosition(float x, float y);

    friend class listManager;
};

//listExt adds swap and move abilities to the list, and also allows for the list to display more than one element at a time.

class listExt : public list {
protected:
    button_t m_swapUpBtn{};
    button_t m_swapDownBtn{};
    void (__stdcall* const m_swapFn)(void* pSender){};

    button_t m_moveBtn{};
    void (__stdcall* const m_moveFn)(void* pSender){};
    int m_moveIndex{};

    listExt* m_target;

protected:
    virtual bool isParent(void* button);
    virtual bool isUp(void* button);

    virtual void navigate(bool up);
    void swap(bool up);
    void move();

    void updateLabels();
    void updateSelector();
    virtual void update();
    
    virtual void enter(void* scene);
    virtual void exit();

    virtual bool load(void* file);
    virtual void save(void* file);

public:
    listExt(const char* title, int length, listExt* target);
    virtual void ifNotFound(const std::vector<std::string>& other, bool add);

    friend class listManager;
};

/*listManager keeps track of all lists using a vector, which allows for callbacks to be cleaner and for full encapsulation of functionality.
* although it's probably very slow if you have too many lists or something lol idk
* 
* the alternative was to make my own inherited class from cocos and i dont know how tf that works soooo
*/

class listManager {
private:
    static inline std::vector<list*> m_vec;
    static inline void* m_saveFile;
    static inline const char* m_filePath;
    static inline const char* m_backupPath;

private:
    //you shouldn't be able to make instances of this class, as all members are static
    listManager() {};

    static void add(list* list);

    static void __stdcall navigate(void* pSender);
    static void __stdcall swap(void* pSender);
    static void __stdcall move(void* pSender);

public:
    static void enter(void* scene);
    static void exit();
    static void setSaveTargets(const char* file, const char* backup);
    static bool load();
    static bool save();

    friend class listExt;
    friend class list;
};