#pragma once
#include "Component.h"

using namespace std;

class XorGate : public Component {
public:
    XorGate(const string& name) : Component(name) {}

protected:
    // XOR 邏輯：輸入為奇數個 true 時結果為 true
    bool evaluate() const override {
        int count = 0;
        for (Component* input : inputs) {
            if (input->getOutput()) count++;
        }
        return (count % 2 != 0);
    }
    string typeName() const override { return "XOR"; }
};
