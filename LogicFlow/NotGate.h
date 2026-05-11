#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class NotGate : public Component {
public:
	NotGate(const string& name) : Component(name) {}

	void compute() override {
		if (inputs.empty()) {
			// 例外防護預留區：如果沒有輸入就執行，應該要拋出例外
			current = false;
			return;
		}

		current = !inputs[0] -> getOutput(); // NOT 邏輯：輸入取反
		cout << "[模擬] " << name << " (NOT) 計算結果為: " << current << "\n";
	}
};