#pragma once
#include "Component.h"

using namespace std;

class AndGate : public Component {
public:
    AndGate(const string& name) : Component(name) {}

protected:
    // AND 邏輯：只要有一個輸入是 false，結果就是 false
    bool evaluate() const override {
        for (Component* input : inputs) {
            if (!input->getOutput()) return false;
        }
        return true;
    }
    string typeName() const override { return "AND"; }
};
