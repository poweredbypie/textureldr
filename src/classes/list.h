#pragma once

#include "pch.h"
#include "extern/cocos.h"
#define LIST_LENGTH 10

class list {
public:

    std::vector<std::string> strings{};
    void* labels[LIST_LENGTH]{};
    bool init{};
    float x{}, y{};

    void* up{};
    void* upFn{};
    void* down{};
    void* downFn{};
    int index{};
    int length{};

    void* sel{};
    void* selFn{};
    int selIndex{};

    void* swapUp{};
    void* swapUpFn{};
    void* swapDown{};
    void* swapDownFn{};

    void* menu{};

private:

    void getLength();
    void toggle(void*& button, bool enabled);
    void updateLabels();
    void updateSelector();

public:

    void enter(void* up, void* down, void* sel, void* scene);
    void enter(void* up, void* down, void* sel, void* mvUp, void* mvDown, void* scene);
    void update();
    void exit();
};