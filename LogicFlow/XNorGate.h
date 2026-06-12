#pragma once
#include "Component.h"

using namespace std;

class XNorGate : public Component {
public:
    XNorGate(const string& name) : Component(name) {}

protected:
    // XNOR 邏輯：輸入為偶數個 true 時結果為 true (與 XOR 完全相反)
    // 修正：原本用 break 提前跳出會在多輸入時算錯，改用計數判斷奇偶。
    bool evaluate() const override {
        int count = 0;
        for (Component* input : inputs) {
            if (input->getOutput()) count++;
        }
        return (count % 2 == 0);
    }
    string typeName() const override { return "XNOR"; }
};
