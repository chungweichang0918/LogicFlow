#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class XorGate : public Component {
public:
	XorGate (const string& name): Component(name) {}

	void compute() override {
		if (inputs.empty()) {
			// 例外防護預留區：如果沒有輸入就執行，應該要拋出例外
			current = false;
			return;
		}
		
		int count = 0; // 計算輸入數量
		for (Component* input : inputs) {
			if (input -> getOutput()) {
				count++;  // 每遇到一個就切換結果
			}
		}

		current = (count % 2 != 0); // XOR 邏輯：輸入為奇數個時結果為 true
		cout << "[模擬] " << name << " (XOR) 計算結果為: " << current << "\n";
	}
};