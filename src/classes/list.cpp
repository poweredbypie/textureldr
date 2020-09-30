#include "pch.h"
#include "list.h"

void horizontalList::toggle(void*& button, bool enabled) {
    using namespace cocos;

    menuItem::setEnabled(button, enabled);
    node::setVisible(button, enabled);
}

bool horizontalList::isParent(void* button) {
    return false;
}

bool horizontalList::isUp(void* button) {
    return false;
}

void horizontalList::navigate(bool sub) {

}

void horizontalList::update() {

}

void horizontalList::enter(void* scene) {

}

void horizontalList::exit() {

}

horizontalList::horizontalList(const char* title, int length) {
    m_titleStr = title;

    m_navFn = listManager::navigate;

    m_pArrListLabels = new void*[length];

    //fix listManager to handle this
    listManager::add(this);
}

void horizontalList::setPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

void verticalList::getLength() {
    m_listLength = (m_listStrings.size() - m_listOffset) < LIST_LENGTH ? (m_listStrings.size() - m_listOffset) : LIST_LENGTH;
}

bool verticalList::isParent(void* button) {
    return (button == m_upBtn || button == m_downBtn || button == m_swapUpBtn || button == m_swapDownBtn || button == m_moveBtn);
}


bool verticalList::isUp(void* button) {
    return (button == m_upBtn || button == m_swapUpBtn);
}

void verticalList::navigate(bool sub) {
    if (sub) {
        if (m_moveIndex > 0)
            --m_moveIndex;
        else if (m_listOffset > 0)
            --m_listOffset;
    }
    else {
        if (m_moveIndex < m_listLength - 1)
            ++m_moveIndex;
        else if (m_listOffset < (int)m_listStrings.size() - LIST_LENGTH)
            ++m_listOffset;
    }

    update();
}

void verticalList::swap(bool up) {
    if (up) {
        if (m_moveIndex + m_listOffset != 0) {
            std::iter_swap(m_listStrings.begin() + m_moveIndex + m_listOffset, m_listStrings.begin() + m_moveIndex + m_listOffset - 1);
            --m_moveIndex;
        }
    }
    else {
        if (m_moveIndex + m_listOffset + 1 != m_listStrings.size()) {
            std::iter_swap(m_listStrings.begin() + m_moveIndex + m_listOffset, m_listStrings.begin() + m_moveIndex + m_listOffset + 1);
            ++m_moveIndex;
        }
    }

    update();
}

void verticalList::move() {
    m_target->m_listStrings.insert(m_target->m_listStrings.begin(), m_listStrings[m_moveIndex + m_listOffset]);
    m_listStrings.erase(m_listStrings.begin() + m_moveIndex + m_listOffset);

    //moves the selector up if the index is the bottom one, so that "vector out of range" doesn't occur
    if (m_listStrings.size() - m_listOffset == m_moveIndex && m_listStrings.size() - m_listOffset != 0)
        --m_moveIndex;

    update();
    m_target->update();
}


void verticalList::updateLabels() {
    using namespace cocos;

    if (!m_init) {
        getLength();
        for (int i{}; i < LIST_LENGTH; ++i) {
            if (i < m_listLength) {
                m_pArrListLabels[i] = label::create(m_listStrings[m_listOffset + i].c_str(), "bigFont.fnt");
                node::setPos(m_pArrListLabels[i], m_x, m_y - (20.0f * i));
                node::setScale(m_pArrListLabels[i], 1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
                node::addChild(m_menu, m_pArrListLabels[i]);
            }
            else {
                m_pArrListLabels[i] = label::create("", "bigFont.fnt");
                node::setPos(m_pArrListLabels[i], m_x, m_y - (20.0f * i));
                node::addChild(m_menu, m_pArrListLabels[i]);
            }
        }
    }
    else {
        getLength();
        for (int i{}; i < LIST_LENGTH; ++i) {
            if (i < m_listLength) {
                label::set(m_pArrListLabels[i], m_listStrings[m_listOffset + i].c_str(), true);
                node::setScale(m_pArrListLabels[i], 1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
            }
            else {
                label::set(m_pArrListLabels[i], "", true);
            }
        }
    }
}

void verticalList::updateSelector() {
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

void verticalList::update() {
    updateLabels();
    updateSelector();
}

void verticalList::enter(void* scene) {
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

void verticalList::exit() {
    m_init = false;
}

verticalList::verticalList(const char* title, int length, verticalList* target) : horizontalList(title, length) {
    m_moveFn = listManager::move;
    m_swapFn = listManager::swap;

    m_target = target;

    listManager::add(this);
}

void verticalList::loadArray() {
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

std::vector<std::string>& verticalList::getArray() {
    return m_listStrings;
}
//listmanager starts here

void listManager::add(horizontalList* list) {
    m_vec.push_back(list);
}

void __stdcall listManager::navigate(void* pSender) {
    horizontalList* target{};
    
    for (horizontalList* i : m_vec) {
        if (i->isParent(pSender)) {
            target = i;
            break;
        }
    }

    if (target) {
        target->navigate(target->isUp(pSender));
    }
}

void __stdcall listManager::swap(void* pSender) {
    horizontalList* target{};

    for (horizontalList* i : m_vec) {
        if (i->isParent(pSender)) {
            target = i;
            break;
        }
    }

    if (target) {
        target->swap(target->isUp(pSender));
    }
}

void __stdcall listManager::move(void* pSender) {
    horizontalList* target{};

    for (horizontalList* i : m_vec) {
        if (i->isParent(pSender)) {
            target = i;
            break;
        }
    }

    if (target) {
        target->move();
    }
}

void listManager::enter(void* scene) {
    for (horizontalList* i : m_vec) {
        i->enter(scene);
    }
}

void listManager::exit() {
    for (horizontalList* i : m_vec) {
        i->exit();
    }
}