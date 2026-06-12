#pragma once
#include "Component.h"

using namespace std;

class OrGate : public Component {
public:
    OrGate(const string& name) : Component(name) {}

protected:
    // OR 邏輯：只要有一個輸入是 true，結果就是 true
    bool evaluate() const override {
        for (Component* input : inputs) {
            if (input->getOutput()) return true;
        }
        return false;
    }
    string typeName() const override { return "OR"; }
};
