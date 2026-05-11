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

	void compute() override {
		// InputPin 的 compute 不需要計算邏輯，因為它的輸出由使用者直接設定
		cout << "[模擬] " << name << " (InputPin) 輸出為: " << current << "\n";
	}
};