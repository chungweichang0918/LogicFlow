#pragma once
#include "Component.h"

using namespace std;

class NandGate : public Component {
public:
    NandGate(const string& name) : Component(name) {}

protected:
    // NAND = NOT(AND)：只要有任一輸入為 false，結果就是 true
    bool evaluate() const override {
        for (Component* input : inputs) {
            if (!input->getOutput()) return true;
        }
        return false;
    }
    string typeName() const override { return "NAND"; }
};
