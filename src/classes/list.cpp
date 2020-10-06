#include "pch.h"
#include "list.h"

void list::getLength() {
    m_displayedLength = (int)(m_listStrings.size() - m_listOffset) < m_maxDisplayedLength ? (m_listStrings.size() - m_listOffset) : m_maxDisplayedLength;
}

void list::toggle(void*& button, bool enabled) {
    using namespace cocos;

    menuItem::setEnabled(button, enabled);
    node::setVisible(button, enabled);
}

bool list::isParent(void* button) {
    return (button == m_upBtn || button == m_downBtn);
}

bool list::isUp(void* button) {
    return (button == m_upBtn);
}

void list::navigate(bool up) {
    if (!m_listStrings.empty()) {
        if (up) {
            if (m_listOffset > 0)
                --m_listOffset;
        }
        else {
            if (m_listOffset < (int)m_listStrings.size() - 1)
                ++m_listOffset;
        }

        update();
    }
}

void list::update() {
    using namespace cocos;

    getLength();

    if (m_displayedLength) {
        if (!m_entered) {
            m_pArrListLabels[0] = label::create(m_listStrings[m_listOffset].c_str(), "bigFont.fnt");
            node::setPos(m_pArrListLabels[0], m_x, m_y);
            node::setScale(m_pArrListLabels[0], 1.3f / ((m_listStrings[m_listOffset].length() + 10) * 0.1f));
            node::addChild(m_menu, m_pArrListLabels[0]);
        }
        else {
            label::set(m_pArrListLabels[0], m_listStrings[m_listOffset].c_str(), true);
            node::setScale(m_pArrListLabels[0], 1.3f / ((m_listStrings[m_listOffset].length() + 10) * 0.1f));
        }
    }
}

void list::enter(void* scene) {
    using namespace cocos;

    m_menu = menu::create();

    m_titleLabel = label::create(m_titleStr, "goldFont.fnt");
    node::setPos(m_titleLabel, m_x, m_y + 30.0f);
    node::addChild(m_menu, m_titleLabel);

    void* navSprite = sprite::create("navArrowBtn_001.png");

    m_upBtn = menuItem::createSpr(navSprite, navSprite, 0, m_menu, m_navFn);
    node::setPos(m_upBtn, m_x - 70.0f, m_y);
    node::setRot(m_upBtn, -180.0f);
    node::setScale(m_upBtn, 0.60f);
    node::addChild(m_menu, m_upBtn);

    m_downBtn = menuItem::createSpr(navSprite, navSprite, 0, m_menu, m_navFn);
    node::setPos(m_downBtn, m_x + 70.0f, m_y);
    node::setScale(m_downBtn, 0.60f);
    node::addChild(m_menu, m_downBtn);

    update();

    node::addChild(scene, m_menu);

    m_entered = true;
}

void list::exit() {
    m_entered = false;
}

list::list(const char* title, int length) {
    m_titleStr = title;

    m_maxDisplayedLength = length;

    m_navFn = listManager::navigate;

    m_pArrListLabels = new void*[length];

    listManager::add(this);
}

void list::setVector(const std::vector<std::string>& vec) {
    m_listStrings = vec;
}

void list::removeIfNotFound(const std::vector<std::string>& other, bool isTarget) {
    if (isTarget) {
        for (int i{}; i < (int)other.size(); ++i) {
            if (std::find(m_listStrings.begin(), m_listStrings.end(), other[i]) == m_listStrings.end()) {
                m_listStrings.erase(m_listStrings.begin() + i);
                --i;
            }

        }
    }
    else {
        for (int i{}; i < (int)m_listStrings.size(); ++i) {
            if (std::find(other.begin(), other.end(), m_listStrings[i]) == other.end()) {
                m_listStrings.erase(m_listStrings.begin() + i);
                --i;
            }
        }
    }
    
    getLength();

    //not tested, maybe do it later but the use case isnt functional in this instance
    if (!m_listStrings.empty()) {
        if (m_listOffset > (int)m_listStrings.size() - 1)
            m_listOffset = (int)m_listStrings.size() - 1;
    }
    else {
        m_listOffset = 0;
    }

    update();
}

std::vector<std::string>& list::getVector() {
    return m_listStrings;
}

const int list::getCurrentIndex() {
    return m_listOffset;
}

void list::setPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

//listExt

bool listExt::isParent(void* button) {
    return (button == m_upBtn || button == m_downBtn || button == m_swapUpBtn || button == m_swapDownBtn || button == m_moveBtn);
}


bool listExt::isUp(void* button) {
    return (button == m_upBtn || button == m_swapUpBtn);
}

void listExt::navigate(bool up) {
    if (up) {
        if (m_moveIndex > 0)
            --m_moveIndex;
        else if (m_listOffset > 0)
            --m_listOffset;
    }
    else {
        if (m_moveIndex < m_displayedLength - 1)
            ++m_moveIndex;
        else if (m_listOffset < (int)m_listStrings.size() - m_maxDisplayedLength)
            ++m_listOffset;
    }

    update();
}

void listExt::swap(bool up) {
    if (up) {
        if (m_moveIndex > 0) {
            std::iter_swap(m_listStrings.begin() + m_moveIndex + m_listOffset, m_listStrings.begin() + m_moveIndex + m_listOffset - 1);
            --m_moveIndex;
        }
    }
    else {
        if (m_moveIndex < m_displayedLength - 1) {
            std::iter_swap(m_listStrings.begin() + m_moveIndex + m_listOffset, m_listStrings.begin() + m_moveIndex + m_listOffset + 1);
            ++m_moveIndex;
        }
    }

    update();
}

void listExt::move() {
    m_target->m_listStrings.insert(m_target->m_listStrings.begin(), m_listStrings[m_moveIndex + m_listOffset]);
    m_listStrings.erase(m_listStrings.begin() + m_moveIndex + m_listOffset);

    //moves the selector up if the index is the bottom one, so that "vector out of range" doesn't occur
    if (m_listStrings.size() - m_listOffset == m_moveIndex && m_listStrings.size() - m_listOffset != 0)
        --m_moveIndex;

    update();
    m_target->update();
}


void listExt::updateLabels() {
    using namespace cocos;
    
    getLength();

    if (!m_entered) {

        for (int i{}; i < m_maxDisplayedLength; ++i) {
            if (i < m_displayedLength) {
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
        for (int i{}; i < m_maxDisplayedLength; ++i) {
            if (i < m_displayedLength) {
                label::set(m_pArrListLabels[i], m_listStrings[m_listOffset + i].c_str(), true);
                node::setScale(m_pArrListLabels[i], 1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
            }
            else {
                label::set(m_pArrListLabels[i], "", true);
            }
        }
    }
}

void listExt::updateSelector() {
    using namespace cocos;

    if (!m_entered) {
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

void listExt::update() {
    updateLabels();
    updateSelector();
}

void listExt::enter(void* scene) {
    using namespace cocos;

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

    m_entered = true;
}

void listExt::exit() {
    m_entered = false;
}

listExt::listExt(const char* title, int length, listExt* target) : list(title, length) {
    m_moveFn = listManager::move;
    m_swapFn = listManager::swap;

    m_target = target;
}

void listExt::removeIfNotFound(const std::vector<std::string>& other, bool isTarget) {
    if (isTarget) {
        for (int i{}; i < (int)other.size(); ++i) {
            if (std::find(m_listStrings.begin(), m_listStrings.end(), other[i]) == m_listStrings.end()) {
                m_listStrings.erase(m_listStrings.begin() + i);
                --i;
            }

        }
    }
    else {
        for (int i{}; i < (int)m_listStrings.size(); ++i) {
            if (std::find(other.begin(), other.end(), m_listStrings[i]) == other.end()) {
                m_listStrings.erase(m_listStrings.begin() + i);
                --i;
            }
        }
    }

    getLength();

    //make this better. if you move more than 10 items, it'll look weird. i dont think it breaks, but it just isn't very clean.
    if (m_moveIndex + 1 > m_displayedLength) {
        if (m_displayedLength > 1)
            m_moveIndex = m_displayedLength - 1;
        else
            m_moveIndex = 0;
    }

    update();
}

//listManager

void listManager::add(list* list) {
    m_vec.push_back(list);
}

void __stdcall listManager::navigate(void* pSender) {
    list* target{};
    
    for (list* i : m_vec) {
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
    listExt* target{};

    for (list* i : m_vec) {
        if (i->isParent(pSender)) {
            target = (listExt*)i;
            break;
        }
    }

    if (target) {
        target->swap(target->isUp(pSender));
    }
}

void __stdcall listManager::move(void* pSender) {
    listExt* target{};

    for (list* i : m_vec) {
        if (i->isParent(pSender)) {
            target = (listExt*)i;
            break;
        }
    }

    if (target) {
        target->move();
    }
}

void listManager::enter(void* scene) {
    for (list* i : m_vec) {
        i->enter(scene);
    }
}

void listManager::exit() {
    for (list* i : m_vec) {
        i->exit();
    }
}

void listManager::setSaveTargets(const char* file, const char* backup) {
    m_filePath = file;
    m_backupPath = backup;
}

bool listManager::load() {
    using namespace cocos::xml;

    m_saveFile = create(true, 1);
    if (loadFile(m_saveFile, m_filePath)) {
        if (loadFile(m_saveFile, m_backupPath))
            return false;

        saveFile(m_saveFile, m_filePath, false);
    }
    else
        saveFile(m_saveFile, m_backupPath, false);
    for (list* i : m_vec) {
        if (void* node = firstChildElement(m_saveFile, i->m_titleStr)) {
            for (void* element = firstChildElement(node, 0); element; element = nextSiblingElement(element, 0))
                i->m_listStrings.push_back(getText(element));
        }
        else
            return false;
    }
    return true;
}

bool listManager::save() {
    using namespace cocos::xml;

    deleteChildren(m_saveFile);

    for (list* i : m_vec) {
        void* node = newElement(m_saveFile, i->m_titleStr);
        insertEndChild(m_saveFile, node);
        for (std::string pack : i->m_listStrings) {
            void* element = newElement(m_saveFile, "pack");
            insertEndChild(element, newText(m_saveFile, pack.c_str()));
            insertEndChild(node, element);
        }
    }

    if (!saveFile(m_saveFile, m_filePath, false))
        return false;

    return true;
}