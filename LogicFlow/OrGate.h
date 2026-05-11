#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class OrGate : public Component {
public:
	OrGate(const string& name) : Component(name) {}

	void compute() override {
		if (inputs.empty()) {
			// 例外防護預留區：如果沒有輸入就執行，應該要拋出例外
			current = false;
			return;
		}

		// OR 邏輯：只要有一個輸入是 true，結果就是 true
		bool result = false;
		for (Component* input : inputs) {
			if (input->getOutput()) {
				result = true;
				break;
			}
		}

		current = result;
		cout << "[模擬] " << name << " (OR) 計算結果為: " << current << "\n";
	}
};