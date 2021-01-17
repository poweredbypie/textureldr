#include "pch.h"
#include "list.h"

inline void list::getLength() {
    m_displayedLength = (int)(m_listStrings.size() - m_listOffset) < m_maxDisplayedLength ? (m_listStrings.size() - m_listOffset) : m_maxDisplayedLength;
}

inline void list::toggle(button_t& button, bool enabled) {
    using namespace cocos2d;

    button->setEnabled(enabled);
    button->setVisible(enabled);
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
    using namespace cocos2d;

    getLength();

    if (m_displayedLength > 0) {
        if (!m_entered) {
            m_listLabels[0] = CCLabelBMFont::create(m_listStrings[m_listOffset].c_str(), "bigFont.fnt");
            m_listLabels[0]->setPosition(m_x, m_y);
            m_listLabels[0]->setScale(1.3f / ((m_listStrings[m_listOffset].length() + 10) * 0.1f));
            m_menu->addChild(m_listLabels[0]);
        }
        else {
            m_listLabels[0]->setString(m_listStrings[m_listOffset].c_str(), true);
            m_listLabels[0]->setScale(1.3f / ((m_listStrings[m_listOffset].length() + 10) * 0.1f));
        }
    }
}

void list::enter(cocos2d::CCScene* scene) {
    using namespace cocos2d;
    using namespace gd;

    m_menu = CCMenu::create();

    m_titleLabel = CCLabelBMFont::create(m_titleStr, "goldFont.fnt");
    m_titleLabel->setPosition(m_x, m_y + 30.0f);
    m_menu->addChild(m_titleLabel);

    m_upBtn = ButtonSprite::create(
        CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
        m_menu,
        m_navFn
    );
    m_upBtn->setPosition(m_x - 70.0f, m_y);
    m_upBtn->setRotation(-180.0f);
    m_upBtn->setScale(0.60f);
    m_menu->addChild(m_upBtn);

    m_downBtn = ButtonSprite::create(
        CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
        m_menu,
        m_navFn
    );
    m_downBtn->setPosition(m_x + 70.0f, m_y);
    m_downBtn->setScale(0.60f);
    m_menu->addChild(m_downBtn);

    update();

    scene->addChild(m_menu);

    m_entered = true;
}

inline void list::exit() {
    m_entered = false;
}

bool list::load(cocos2d::tinyxml2::XMLDocument* file) {
    using namespace cocos2d::tinyxml2;

    if (auto list = file->FirstChildElement(m_titleStr)) {
        if (list->QueryIntAttribute("offset", &m_listOffset))
            return false;

        for (auto i = list->FirstChildElement("entry"); i; i = i->NextSiblingElement(0))
            m_listStrings.push_back(i->GetText());

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

void list::save(cocos2d::tinyxml2::XMLDocument* file) {
    using namespace cocos2d::tinyxml2;

    auto list = file->NewElement(m_titleStr);
    list->SetAttribute("offset", m_listOffset);
    file->InsertEndChild(list);
    for (std::string entry : m_listStrings) {
        auto i = file->NewElement("entry");
        i->InsertEndChild(file->NewText(entry.c_str()));
        list->InsertEndChild(i);
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
    using namespace cocos2d;
    
    getLength();

    if (!m_entered) {

        for (int i{}; i < m_maxDisplayedLength; ++i) {
            if (i < m_displayedLength) {
                m_listLabels[i] = CCLabelBMFont::create(m_listStrings[m_listOffset + i].c_str(), "bigFont.fnt");
                m_listLabels[i]->setPosition(m_x, m_y - (20.0f * i));
                m_listLabels[i]->setScale(1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
                m_menu->addChild(m_listLabels[i]);
            }
            else {
                m_listLabels[i] = CCLabelBMFont::create("", "bigFont.fnt");
                m_listLabels[i]->setPosition(m_x, m_y - (20.0f * i));
                m_menu->addChild(m_listLabels[i]);
            }
        }
    }
    else {
        for (int i{}; i < m_maxDisplayedLength; ++i) {
            if (i < m_displayedLength) {
                m_listLabels[i]->setString(m_listStrings[m_listOffset + i].c_str(), true);
                m_listLabels[i]->setScale(1.0f / ((m_listStrings[m_listOffset + i].length() + 10) * 0.1f));
            }
            else {
                m_listLabels[i]->setString("", true);
            }
        }
    }
}

void listExt::updateSelector() {
    using namespace cocos2d;
    using namespace gd;

    if (!m_entered) {
        m_moveBtn = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
            m_menu,
            m_moveFn
        );

        m_swapUpBtn = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
            m_menu,
            m_swapFn
        );

        m_swapDownBtn = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
            m_menu,
            m_swapFn
        );

        float mult = m_x < 0 ? 55.0f : -55.0f;

        m_moveBtn->setPosition(m_x + mult, m_y - 20.0f * m_moveIndex);
        m_moveBtn->setRotation(mult == -55.0f ? 180.0f : 0.0f);

        m_swapUpBtn->setPosition(m_x - mult, m_y + 5.0f - 20.0f * m_moveIndex);
        m_swapDownBtn->setPosition(m_x - mult, m_y - 5.0f - 20.0f * m_moveIndex);

        m_moveBtn->setScale(0.25f);
        m_menu->addChild(m_moveBtn);

        m_swapUpBtn->setRotation(-90.0f);
        m_swapDownBtn->setRotation(90.0f);
        m_swapUpBtn->setScale(0.25f);
        m_swapDownBtn->setScale(0.25f);
        m_menu->addChild(m_swapUpBtn);
        m_menu->addChild(m_swapDownBtn);
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
        else {
            float mult = m_x < 0 ? 55.0f : -55.0f;

            m_moveBtn->setPosition(m_x + mult, m_y - 20.0f * m_moveIndex);
            m_moveBtn->setRotation(mult == -55.0f ? 180.0f : 0.0f);
            toggle(m_moveBtn, true);

            m_swapUpBtn->setPosition(m_x - mult, m_y + 5.0f - 20.0f * m_moveIndex);
            m_swapDownBtn->setPosition(m_x - mult, m_y - 5.0f - 20.0f * m_moveIndex);
            toggle(m_swapUpBtn, true);
            toggle(m_swapDownBtn, true);
        }
    }
}

void listExt::update() {
    updateLabels();
    updateSelector();
}

void listExt::enter(cocos2d::CCScene* scene) {
    using namespace cocos2d;
    using namespace gd;

    m_menu = CCMenu::create();

    m_titleLabel = CCLabelBMFont::create(m_titleStr, "goldFont.fnt");
    m_titleLabel->setPosition(m_x, m_y + 50.0f);
    m_menu->addChild(m_titleLabel);

    m_upBtn = ButtonSprite::create(
        CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
        m_menu,
        m_navFn
    );
    m_upBtn->setPosition(m_x, m_y + 25.0f);
    m_upBtn->setRotation(-90.0f);
    m_upBtn->setScale(0.75f);
    m_menu->addChild(m_upBtn);

    m_downBtn = ButtonSprite::create(
        CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
        m_menu,
        m_navFn
    );
    m_downBtn->setPosition(m_x, m_y - 205.0f);
    m_downBtn->setRotation(90.0f);
    m_downBtn->setScale(0.75f);
    m_menu->addChild(m_downBtn);

    update();

    scene->addChild(m_menu);

    m_entered = true;
}

bool listExt::load(cocos2d::tinyxml2::XMLDocument* file) {
    using namespace cocos2d::tinyxml2;

    if (auto list = file->FirstChildElement(m_titleStr)) {
        if (list->QueryIntAttribute("offset", &m_listOffset) ||
            list->QueryIntAttribute("index", &m_moveIndex))
            return false;
        
        for (auto i = list->FirstChildElement("entry"); i; i = i->NextSiblingElement(0))
            m_listStrings.push_back(i->GetText());

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

void listExt::save(cocos2d::tinyxml2::XMLDocument* file) {
    using namespace cocos2d::tinyxml2;

    auto list = file->NewElement(m_titleStr);
    list->SetAttribute("offset", m_listOffset);
    list->SetAttribute("index", m_moveIndex);
    file->InsertEndChild(list);
    for (std::string entry : m_listStrings) {
        auto i = file->NewElement("entry");
        i->InsertEndChild(file->NewText(entry.c_str()));
        list->InsertEndChild(i);
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
        if (i->isParent(reinterpret_cast<list::button_t>(pSender))) {
            target = i;
            break;
        }
    }

    if (target) {
        target->navigate(target->isUp(reinterpret_cast<list::button_t>(pSender)));
    }
}

void __stdcall listManager::swap(void* pSender) {
    listExt* target{};

    for (list* i : m_vec) {
        if (i->isParent(reinterpret_cast<list::button_t>(pSender))) {
            target = (listExt*)i;
            break;
        }
    }

    if (target) {
        target->swap(target->isUp(reinterpret_cast<list::button_t>(pSender)));
    }
}

void __stdcall listManager::move(void* pSender) {
    listExt* target{};

    for (list* i : m_vec) {
        if (i->isParent(reinterpret_cast<list::button_t>(pSender))) {
            target = (listExt*)i;
            break;
        }
    }

    if (target) {
        target->move();
    }
}

void listManager::enter(cocos2d::CCScene* scene) {
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
    using namespace cocos2d::tinyxml2;

    m_saveFile = XMLDocument::create(true, 1);
    if (m_saveFile->LoadFile(m_filePath)) {
        if (m_saveFile->LoadFile(m_backupPath))
            return false;

        m_saveFile->SaveFile(m_filePath, false);
    }
    else
        m_saveFile->SaveFile(m_backupPath, false);
    for (list* i : m_vec) {
        if (!i->load(m_saveFile))
            return false;
    }
    return true;
}

bool listManager::save() {
    using namespace cocos2d::tinyxml2;

    m_saveFile->DeleteChildren();

    for (list* i : m_vec) {
        i->save(m_saveFile);
    }

    if (!m_saveFile->SaveFile(m_filePath, false))
        return false;

    return true;
}