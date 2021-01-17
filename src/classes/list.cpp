#include "pch.h"
#include "list.h"

inline void list::getLength() {
    m_displayedLength = (int)(m_listStrings.size() - m_listOffset) < m_maxDisplayedLength ? (m_listStrings.size() - m_listOffset) : m_maxDisplayedLength;
}

inline void list::toggle(button_t& button, bool enabled) {
    using namespace cocos;

    menuItem::setEnabled(button, enabled);
    node::setVisible(button, enabled);
}

inline bool list::isParent(button_t button) {
    return (button == m_upBtn || button == m_downBtn);
}

inline bool list::isUp(button_t button) {
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

    if (m_displayedLength > 0) {
        if (!m_entered) {
            m_listLabels[0] = label::create(m_listStrings[m_listOffset].c_str(), "bigFont.fnt");
            node::setPos(m_listLabels[0], m_x, m_y);
            node::setScale(m_listLabels[0], 1.3f / ((m_listStrings[m_listOffset].length() + 10) * 0.1f));
            node::addChild(m_menu, m_listLabels[0]);
        }
        else {
            label::set(m_listLabels[0], m_listStrings[m_listOffset].c_str(), true);
            node::setScale(m_listLabels[0], 1.3f / ((m_listStrings[m_listOffset].length() + 10) * 0.1f));
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

inline void list::exit() {
    m_entered = false;
}

bool list::load(void* file) {
    using namespace cocos::xml;

    if (void* list = firstChildElement(file, m_titleStr)) {
        if (queryAttribute(list, "offset", &m_listOffset))
            return false;

        for (void* i = firstChildElement(list, "entry"); i; i = nextSiblingElement(i, 0))
            m_listStrings.push_back(getText(i));
        
        if (m_maxDisplayedLength < (int)m_listStrings.size()) {
            if (m_listOffset > (int)m_listStrings.size() - m_maxDisplayedLength)
                m_listOffset = (int)m_listStrings.size() - m_maxDisplayedLength;
            else if (m_listOffset < 0)
                m_listOffset = 0;
        }
        else
            m_listOffset = 0;

        return true;
    }
    return false;
}

void list::save(void* file) {
    using namespace cocos::xml;

    void* list = newElement(file, m_titleStr);
    setAttribute(list, "offset", m_listOffset);
    insertEndChild(file, list);
    for (std::string entry : m_listStrings) {
        void* i = newElement(file, "entry");
        insertEndChild(i, newText(file, entry.c_str()));
        insertEndChild(list, i);
    }
}

list::list(const char* title, int length) : m_navFn{ listManager::navigate } {
    m_titleStr = title;

    m_maxDisplayedLength = length;

    m_listLabels = new label_t[length];

    listManager::add(this);
}

void list::setVector(const std::vector<std::string>& vec) {
    m_listStrings = vec;

    if (m_entered)
        update();
}

void list::ifNotFound(const std::vector<std::string>& other, bool add) {
    if (add) {
        for (int i{}; i < (int)other.size(); ++i) {
            if (std::find(m_listStrings.begin(), m_listStrings.end(), other[i]) == m_listStrings.end()) {
                m_listStrings.insert(m_listStrings.begin(), other[i]);
            }
        }
    }
    else {
        for (int i{}; i < (int)m_listStrings.size(); ++i) {
            if (std::find(other.begin(), other.end(), m_listStrings[i]) == other.end()) {
                m_listStrings.erase(m_listStrings.begin() + i);
                --i;

                //move to the start of the list, im too lazy
                m_listOffset = 0;
            }
        }
    }
    
    if (m_entered)
        update();
}

const std::vector<std::string>& list::getVector() {
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

bool listExt::isParent(button_t button) {
    return (button == m_upBtn || button == m_downBtn || button == m_swapUpBtn || button == m_swapDownBtn || button == m_moveBtn);
}


bool listExt::isUp(button_t button) {
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
        if (m_moveIndex + 1 < m_displayedLength)
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
        if (m_moveIndex + 1 < m_displayedLength) {
            std::iter_swap(m_listStrings.begin() + m_moveIndex + m_listOffset, m_listStrings.begin() + m_moveIndex + m_listOffset + 1);
            ++m_moveIndex;
        }
    }

    update();
}

void listExt::move() {
    m_target->m_listStrings.insert(m_target->m_listStrings.begin(), m_listStrings[m_moveIndex + m_listOffset]);
    m_listStrings.erase(m_listStrings.begin() + m_moveIndex + m_listOffset);

    getLength();

    if ((int)m_listStrings.size() >= m_maxDisplayedLength &&
        m_displayedLength < m_maxDisplayedLength)
        --m_listOffset;
    else if (m_displayedLength < m_moveIndex + 1 &&
        m_displayedLength > 0)
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
        for (int i{}; i < m_maxDisplayedLength; ++i) {
            if (i < m_displayedLength) {
                label::set(m_listLabels[i], m_listStrings[m_listOffset + i].c_str(), true);
                node::setScale(m_listLabels[i], 1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
            }
            else {
                label::set(m_listLabels[i], "", true);
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

bool listExt::load(void* file) {
    using namespace cocos::xml;

    if (void* list = firstChildElement(file, m_titleStr)) {
        if (queryAttribute(list, "offset", &m_listOffset) ||
            queryAttribute(list, "index", &m_moveIndex))
            return false;
        
        for (void* i = firstChildElement(list, "entry"); i; i = nextSiblingElement(i, 0))
            m_listStrings.push_back(getText(i));

        if (m_maxDisplayedLength < (int)m_listStrings.size()) {
            if (m_listOffset > (int)m_listStrings.size() - m_maxDisplayedLength)
                m_listOffset = (int)m_listStrings.size() - m_maxDisplayedLength;
            else if (m_listOffset < 0)
                m_listOffset = 0;
        }
        else
            m_listOffset = 0;

        getLength();

        if (m_moveIndex > m_displayedLength - 1 && m_displayedLength)
            m_moveIndex = m_displayedLength - 1;
        else if (m_moveIndex < 0)
            m_moveIndex = 0;

        return true;
    }
    return false;
}

void listExt::save(void* file) {
    using namespace cocos::xml;

    void* list = newElement(file, m_titleStr);
    setAttribute(list, "offset", m_listOffset);
    setAttribute(list, "index", m_moveIndex);
    insertEndChild(file, list);
    for (std::string entry : m_listStrings) {
        void* i = newElement(file, "entry");
        insertEndChild(i, newText(file, entry.c_str()));
        insertEndChild(list, i);
    }
}

listExt::listExt(const char* title, int length, listExt* target) : 
list(title, length), m_swapFn{ listManager::swap }, m_moveFn{ listManager::move } {
    m_target = target;
}

void listExt::ifNotFound(const std::vector<std::string>& other, bool add) {
    std::vector<std::string> old{ m_listStrings };
    old.insert(old.end(), m_target->m_listStrings.begin(), m_target->m_listStrings.end());

    if (add) {
        for (int i{}; i < (int)other.size(); ++i) {
            if (std::find(old.begin(), old.end(), other[i]) == old.end()) {
                m_listStrings.insert(m_listStrings.begin(), other[i]);
            }
        }
    }
    else {
        for (int i{}; i < (int)old.size(); ++i) {
            if (std::find(other.begin(), other.end(), old[i]) == other.end()) {
                std::vector<std::string>::iterator index = std::find(m_listStrings.begin(), m_listStrings.end(), old[i]);
                if (index != m_listStrings.end()) {
                    m_listStrings.erase(index);
                }
                else {
                    index = std::find(m_target->m_listStrings.begin(), m_target->m_listStrings.end(), old[i]);
                    m_target->m_listStrings.erase(index);
                }

                //move to the start of the list, im too lazy to do calculation
                m_listOffset = 0;
                m_moveIndex = 0;
            }
        }
    }

    if (m_entered) {
        update();
        m_target->update();
    }
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
        if (!i->load(m_saveFile))
            return false;
    }
    return true;
}

bool listManager::save() {
    using namespace cocos::xml;

    deleteChildren(m_saveFile);

    for (list* i : m_vec) {
        i->save(m_saveFile);
    }

    if (!saveFile(m_saveFile, m_filePath, false))
        return false;

    return true;
}