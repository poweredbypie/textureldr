#include "pch.h"
#include "list.h"

void list::getLength() {
    m_listLength = (m_listStrings.size() - m_listOffset) < LIST_LENGTH ? (m_listStrings.size() - m_listOffset) : LIST_LENGTH;
}

void list::toggle(void*& button, bool enabled) {
    using namespace cocos;

    menuItem::setEnabled(button, enabled);
    node::setVisible(button, enabled);
}

void list::updateLabels() {
    using namespace cocos;

    if (!m_init) {
        getLength();
        for (int i{}; i < LIST_LENGTH; ++i) {
            if (i < m_listLength) {
                m_listLabels[i] = label::create(m_listStrings[m_listOffset + i].c_str(), "bigFont.fnt");
                node::setPos(m_listLabels[i], m_x, m_y - (20.0f * i));
                node::setScale(m_listLabels[i], 1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
                node::addChild(m_menu, m_listLabels[i]);
            }
            else {
                m_listLabels[i] = label::create("", "bigFont.fnt");
                node::setPos(m_listLabels[i], m_x, m_y - (20.0f * i));
                node::addChild(m_menu, m_listLabels[i]);
            }
        }
    }
    else {
        getLength();
        for (int i{}; i < LIST_LENGTH; ++i) {
            if (i < m_listLength) {
                label::set(m_listLabels[i], m_listStrings[m_listOffset + i].c_str(), true);
                node::setScale(m_listLabels[i], 1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
            }
            else {
                label::set(m_listLabels[i], "", true);
            }
        }
    }
}

void list::updateSelector() {
    using namespace cocos;

    if (!m_init) {
        void* navSprite = sprite::create("navArrowBtn_001.png");
        m_moveBtn = menuItem::createSpr(navSprite, navSprite, 0, m_menu, m_moveFn);

        m_swapUpBtn = menuItem::createSpr(navSprite, navSprite, 0, m_menu, m_swapFn);
        m_swapDownBtn = menuItem::createSpr(navSprite, navSprite, 0, m_menu, m_swapFn);
        if (m_x < 0) {
            node::setPos(m_moveBtn, m_x + 55.0f, m_y - 20.0f * m_moveIndex);

            node::setPos(m_swapUpBtn, m_x - 55.0f, m_y + 5.0f - 20.0f * m_moveIndex);
            node::setPos(m_swapDownBtn, m_x - 55.0f, m_y - 5.0f - 20.0f * m_moveIndex);
        }
        else {
            node::setPos(m_moveBtn, m_x - 55.0f, m_y - 20.0f * m_moveIndex);
            node::setRot(m_moveBtn, 180.0f);

            node::setPos(m_swapUpBtn, m_x + 55.0f, m_y + 5.0f - 20.0f * m_moveIndex);
            node::setPos(m_swapDownBtn, m_x + 55.0f, m_y - 5.0f - 20.0f * m_moveIndex);
        }
        node::setScale(m_moveBtn, 0.25f);
        node::addChild(m_menu, m_moveBtn);

        node::setRot(m_swapUpBtn, -90.0f);
        node::setRot(m_swapDownBtn, 90.0f);
        node::setScale(m_swapUpBtn, 0.25f);
        node::setScale(m_swapDownBtn, 0.25f);
        node::addChild(m_menu, m_swapUpBtn);
        node::addChild(m_menu, m_swapDownBtn);
        if (m_listStrings.size() - m_listOffset == 0) {
            toggle(m_moveBtn, false);
            toggle(m_swapUpBtn, false);
            toggle(m_swapDownBtn, false);
        }
    }
    else {
        if (m_listStrings.size() - m_listOffset == 0) {
            toggle(m_moveBtn, false);

            toggle(m_swapUpBtn, false);
            toggle(m_swapDownBtn, false);

        }
        else if (m_x < 0) {
            node::setPos(m_moveBtn, m_x + 55.0f, m_y - 20.0f * m_moveIndex);
            toggle(m_moveBtn, true);

            node::setPos(m_swapUpBtn, m_x - 55.0f, m_y + 5.0f - 20.0f * m_moveIndex);
            node::setPos(m_swapDownBtn, m_x - 55.0f, m_y - 5.0f - 20.0f * m_moveIndex);
            toggle(m_swapUpBtn, true);
            toggle(m_swapDownBtn, true);

        }
        else {
            node::setPos(m_moveBtn, m_x - 55.0f, m_y - 20.0f * m_moveIndex);
            node::setRot(m_moveBtn, 180.0f);
            toggle(m_moveBtn, true);

            node::setPos(m_swapUpBtn, m_x + 55.0f, m_y + 5.0f - 20.0f * m_moveIndex);
            node::setPos(m_swapDownBtn, m_x + 55.0f, m_y - 5.0f - 20.0f * m_moveIndex);
            toggle(m_swapUpBtn, true);
            toggle(m_swapDownBtn, true);

        }
    }
}

list::list(const char* title, list* target, listManager* manager) {
    m_titleStr = title;

    m_navFn = listManager::navigate;
    m_moveFn = listManager::move;
    m_swapFn = listManager::swap;

    m_target = target;

    manager->add(this);
}

void list::loadArray() {
    using namespace std::filesystem;

    m_listStrings.clear();
    path packs = current_path() / "packs";
    if (exists(packs)) {
        if (is_directory(packs)) {
            directory_iterator packsIter{ packs };
            for (directory_entry pack : packsIter) {
                if (is_directory(pack))
                    m_listStrings.push_back(pack.path().filename().string());
            }
        }
        else {
            MessageBox(0, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
        }
    }
    else {
        //TODO: change this to fading text so it doesn't exit you out of fullscreen
        create_directories(packs);
        MessageBox(0, "created packs folder.", "textureldr", MB_OK);
    }
}

std::vector<std::string>& list::getArray() {
    return m_listStrings;
}

void list::setPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

void list::enter(void* scene) {
    using namespace cocos;

    void* director = director::get();
    m_menu = menu::create();

    m_titleLabel = label::create(m_titleStr, "goldFont.fnt");
    node::setPos(m_titleLabel, m_x, m_y + 50.0f);
    node::addChild(m_menu, m_titleLabel);

    void* navSprite = sprite::create("navArrowBtn_001.png");

    m_upBtn = menuItem::createSpr(navSprite, navSprite, 0, m_menu, m_navFn);
    node::setPos(m_upBtn, m_x, m_y + 25.0f);
    node::setRot(m_upBtn, -90.0f);
    node::setScale(m_upBtn, 0.75f);
    node::addChild(m_menu, m_upBtn);

    m_downBtn = menuItem::createSpr(navSprite, navSprite, 0, m_menu, m_navFn);
    node::setPos(m_downBtn, m_x, m_y - 205.0f);
    node::setRot(m_downBtn, 90.0f);
    node::setScale(m_downBtn, 0.75f);
    node::addChild(m_menu, m_downBtn);

    update();

    node::addChild(scene, m_menu);

    m_init = true;
}

void list::update() {
    updateLabels();
    updateSelector();
}

void list::exit() {
    m_init = false;
}

//listmanager starts here

list* listManager::getAttributes(void* pSender) {
    for (list* i : listOfLists) {
        if (pSender == i->m_upBtn || pSender == i->m_swapUpBtn) {
            up = true;
            return i;
        }
        if (pSender == i->m_downBtn || pSender == i->m_swapDownBtn) {
            up = false;
            return i;
        }
        if (pSender == i->m_moveBtn) {
            return i;
        }
    }
    return 0;
}

void __stdcall listManager::navigate(void* pSender) {
    list* target = getAttributes(pSender);

    if (target) {
        if (up) {
            if (target->m_moveIndex > 0)
                --target->m_moveIndex;
            else if (target->m_listOffset > 0)
                --target->m_listOffset;
        }
        else {
            if (target->m_moveIndex < target->m_listLength - 1)
                ++target->m_moveIndex;
            else if (target->m_listOffset < (int)target->m_listStrings.size() - LIST_LENGTH)
                ++target->m_listOffset;
        }
        target->update();
    }
}

void __stdcall listManager::swap(void* pSender) {
    list* target = getAttributes(pSender);

    if (target) {
        if (up) {
            if (target->m_moveIndex + target->m_listOffset != 0) {
                std::iter_swap(target->m_listStrings.begin() + target->m_moveIndex + target->m_listOffset, target->m_listStrings.begin() + target->m_moveIndex + target->m_listOffset - 1);
                --target->m_moveIndex;
            }
        }
        else {
            if (target->m_moveIndex + target->m_listOffset + 1 != target->m_listStrings.size()) {
                std::iter_swap(target->m_listStrings.begin() + target->m_moveIndex + target->m_listOffset, target->m_listStrings.begin() + target->m_moveIndex + target->m_listOffset + 1);
                ++target->m_moveIndex;
            }
        }
        target->update();
    }
}

void __stdcall listManager::move(void* pSender) {
    list* target = getAttributes(pSender);

    if (target) {
        target->m_target->m_listStrings.insert(target->m_target->m_listStrings.begin(), target->m_listStrings[target->m_moveIndex + target->m_listOffset]);
        target->m_listStrings.erase(target->m_listStrings.begin() + target->m_moveIndex + target->m_listOffset);

        //moves the selector up if the index is the bottom one, so that "vector out of range" doesn't occur
        if (target->m_listStrings.size() - target->m_listOffset == target->m_moveIndex && target->m_listStrings.size() - target->m_listOffset != 0)
            --target->m_moveIndex;
        target->update();
        target->m_target->update();
    }
}

void listManager::add(list* list) {
    listOfLists.push_back(list);
}

void listManager::enter(void* scene) {
    for (list* i : listOfLists) {
        i->enter(scene);
    }
}

void listManager::exit() {
    for (list* i : listOfLists) {
        i->exit();
    }
}