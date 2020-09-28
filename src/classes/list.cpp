#include "pch.h"
#include "list.h"

void list::getLength() {
    length = (strings.size() - index) < LIST_LENGTH ? (strings.size() - index) : LIST_LENGTH;
}

void list::toggle(void*& button, bool enabled) {
    using namespace cocos;

    menuItem::setEnabled(button, enabled);
    node::setVisible(button, enabled);
}

void list::updateLabels() {
    using namespace cocos;

    if (!init) {
        getLength();
        for (int i{}; i < LIST_LENGTH; ++i) {
            if (i < length) {
                labels[i] = label::create(strings[index + i].c_str(), "bigFont.fnt");
                node::setPos(labels[i], x, y - (20.0f * i));
                node::setScale(labels[i], 1.0f / ((strings[index + i].length() + 10) * 0.1f));
                node::addChild(menu, labels[i]);
            }
            else {
                labels[i] = label::create("", "bigFont.fnt");
                node::setPos(labels[i], x, y - (20.0f * i));
                node::addChild(menu, labels[i]);
            }
        }
    }
    else {
        getLength();
        for (int i{}; i < LIST_LENGTH; ++i) {
            if (i < length) {
                label::set(labels[i], strings[index + i].c_str(), true);
                node::setScale(labels[i], 1.0f / ((strings[index + i].length() + 10) * 0.1f));
            }
            else {
                label::set(labels[i], "", true);
            }
        }
    }
}

void list::updateSelector() {
    using namespace cocos;

    if (!init) {
        void* navSprite = sprite::create("navArrowBtn_001.png");
        sel = menuItem::createSpr(navSprite, navSprite, 0, menu, selFn);
        if (x < 0) {
            node::setPos(sel, x + 55.0f, 90.0f - 20.0f * selIndex);
        }
        else {
            node::setPos(sel, x - 55.0f, 90.0f - 20.0f * selIndex);
            node::setRot(sel, 180.0f);
        }
        node::setScale(sel, 0.25f);
        node::addChild(menu, sel);
        if (strings.size() - index == 0) {
            toggle(sel, false);
        }
        if (swapUpFn && swapDownFn) {
            swapUp = menuItem::createSpr(navSprite, navSprite, 0, menu, swapUpFn);
            swapDown = menuItem::createSpr(navSprite, navSprite, 0, menu, swapDownFn);
            if (x < 0) {
                node::setPos(swapUp, x - 55.0f, 95.0f - 20.0f * selIndex);
                node::setPos(swapDown, x - 55.0f, 85.0f - 20.0f * selIndex);
            }
            else {
                node::setPos(swapUp, x + 55.0f, 95.0f - 20.0f * selIndex);
                node::setPos(swapDown, x + 55.0f, 85.0f - 20.0f * selIndex);
            }
            node::setRot(swapUp, -90.0f);
            node::setRot(swapDown, 90.0f);
            node::setScale(swapUp, 0.25f);
            node::setScale(swapDown, 0.25f);
            node::addChild(menu, swapUp);
            node::addChild(menu, swapDown);
            if (strings.size() - index == 0) {
                toggle(swapUp, false);
                toggle(swapDown, false);
            }
        }
    }
    else {
        if (strings.size() - index == 0) {
            toggle(sel, false);
            if (swapUpFn && swapDownFn) {
                toggle(swapUp, false);
                toggle(swapDown, false);
            }
        }
        else if (x < 0) {
            node::setPos(sel, x + 55.0f, 90.0f - 20.0f * selIndex);
            toggle(sel, true);
            if (swapUpFn && swapDownFn) {
                node::setPos(swapUp, x - 55.0f, 95.0f - 20.0f * selIndex);
                node::setPos(swapDown, x - 55.0f, 85.0f - 20.0f * selIndex);
                toggle(swapUp, true);
                toggle(swapDown, true);
            }
        }
        else {
            node::setPos(sel, x - 55.0f, 90.0f - 20.0f * selIndex);
            node::setRot(sel, 180.0f);
            toggle(sel, true);
            if (swapUpFn && swapDownFn) {
                node::setPos(swapUp, x + 55.0f, 95.0f - 20.0f * selIndex);
                node::setPos(swapDown, x + 55.0f, 85.0f - 20.0f * selIndex);
                toggle(swapUp, true);
                toggle(swapDown, true);
            }
        }
    }
}

void list::enter(void* up, void* down, void* sel, void* scene) {
    using namespace cocos;

    void* director = director::get();
    menu = menu::create();

    upFn = up;
    downFn = down;
    selFn = sel;

    void* navSprite = sprite::create("navArrowBtn_001.png");
    up = menuItem::createSpr(navSprite, navSprite, 0, menu, upFn);
    node::setPos(up, x, 115.0f);
    node::setRot(up, -90.0f);
    node::setScale(up, 0.75f);
    node::addChild(menu, up);
    down = menuItem::createSpr(navSprite, navSprite, 0, menu, downFn);
    node::setPos(down, x, -115.0f);
    node::setRot(down, 90.0f);
    node::setScale(down, 0.75f);
    node::addChild(menu, down);

    update();

    node::addChild(scene, menu);
    init = true;
}

void list::enter(void* up, void* down, void* sel, void* swapUp, void* swapDown, void* scene) {
    using namespace cocos;

    void* director = director::get();
    menu = menu::create();

    upFn = up;
    downFn = down;
    selFn = sel;
    swapUpFn = swapUp;
    swapDownFn = swapDown;

    void* navSprite = sprite::create("navArrowBtn_001.png");
    up = menuItem::createSpr(navSprite, navSprite, 0, menu, upFn);
    node::setPos(up, x, 115.0f);
    node::setRot(up, -90.0f);
    node::setScale(up, 0.75f);
    node::addChild(menu, up);
    down = menuItem::createSpr(navSprite, navSprite, 0, menu, downFn);
    node::setPos(down, x, -115.0f);
    node::setRot(down, 90.0f);
    node::setScale(down, 0.75f);
    node::addChild(menu, down);

    update();

    node::addChild(scene, menu);
    init = true;
}

void list::update() {
    updateLabels();
    updateSelector();
}

void list::exit() {
    init = false;
}