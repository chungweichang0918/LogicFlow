#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class AndGate : public Component {
public:
    AndGate(const string& name) : Component(name) {}

    // 覆寫 compute 邏輯
    void compute() override {
        if (inputs.empty()) {
            // 例外防護預留區：如果沒有輸入就執行，應該要拋出例外
            current = false;
            return;
        }

        // AND 邏輯：只要有一個輸入是 false，結果就是 false
        bool result = true;
        for (Component* input : inputs) {
            if (!input->getOutput()) {
                result = false;
                break;
            }
        }
        current = result;
        // 測試用輸出，確認元件有在運作
        std::cout << "[模擬] " << name << " (AND) 計算結果為: " << current << "\n";
    }
};