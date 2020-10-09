#pragma once

#include "pch.h"
#include "extern/cocos.h"
#define FCNPTR_T void*
#define BUTTON_T void*

class listManager;

//the list class embodies all elements needed to form a list: a title, the actual list, and navigation buttons.

class list {
protected:
    const char* m_titleStr{};
    void* m_titleLabel{};

    std::vector<std::string> m_listStrings{};
    void** m_pArrListLabels{};
    int m_maxDisplayedLength{};
    int m_displayedLength{};
    bool m_entered{ false };
    float m_x{}, m_y{};

    BUTTON_T m_upBtn{};
    BUTTON_T m_downBtn{};
    FCNPTR_T m_navFn{};
    int m_listOffset{ 0 };

    void* m_menu{};

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
    virtual void removeIfNotFound(const std::vector<std::string>& other, bool isTarget);
    std::vector<std::string>& getVector();
    const int getCurrentIndex();
    
    void setPosition(float x, float y);

    friend class listManager;
};

//listExt adds swap and move abilities to the list, and also allows for the list to display more than one element at a time.

class listExt : public list {
protected:
    BUTTON_T m_swapUpBtn{};
    BUTTON_T m_swapDownBtn{};
    FCNPTR_T m_swapFn{};

    BUTTON_T m_moveBtn{};
    FCNPTR_T m_moveFn{};
    int m_moveIndex{};

    void* m_menu{};

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
    virtual void removeIfNotFound(const std::vector<std::string>& other, bool isTarget);

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