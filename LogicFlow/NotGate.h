#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class NotGate : public Component {
public:
    NotGate(const string& name) : Component(name) {}

    // NOT 為單一輸入閘，第二條以上的接線視為違規操作並拋出例外
    void addInput(Component* a) override {
        if (!inputs.empty()) {
            throw CircuitException("NOT 閘 [" + name + "] 僅支援單一輸入，無法再接線！");
        }
        inputs.push_back(a);
    }

protected:
    bool evaluate() const override {
        return !inputs[0]->getOutput(); // NOT 邏輯：輸入取反 (compute() 已保證 inputs 非空)
    }
    string typeName() const override { return "NOT"; }
};
