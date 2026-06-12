#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class InputPin : public Component {
public:
    InputPin(const string& name) : Component(name) {}

    void setValue(bool value) {
        current = value; // 直接設定輸出值
        cout << "[系統] 設定輸入接腳 " << name << " 的值為: " << current << "\n";
    }

    // InputPin 的輸出由使用者直接設定，不經過 evaluate()
    void compute() override {
        if (verbose) {
            cout << "[模擬] " << name << " (InputPin) 輸出為: " << current << "\n";
        }
    }

protected:
    bool evaluate() const override { return current; } // 不會被呼叫，僅為滿足介面
    string typeName() const override { return "PIN"; }
};
