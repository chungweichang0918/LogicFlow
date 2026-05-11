#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class NandGate : public  Component {
public:
	NandGate (const string &name) : Component(name) {}

	void compute() override {
		if (inputs.empty()) {
			// 例外防護預留區：如果沒有輸入就執行，應該要拋出例外
			current = false;
			return;
		}

		bool result = true;
		for (Component* input : inputs) {
			if (!input -> getOutput()) {
				result = true; // NAND 邏輯：只要有一個輸入是 false，結果就是 true
				break;
			}
			result = false; // 如果所有輸入都是 true，結果就是 false
		}

		current = !result; // NAND 邏輯：取反
		cout << "[模擬] " << name << " (NAND) 計算結果為: " << current << "\n";
	}
};