#pragma once
#include "Component.h"
#include <iostream>

using namespace std;

class XNorGate : public Component {
public:
	XNorGate (const string& name) : Component(name) {}

	void compute() override {
		if (inputs.empty()) {
			// 例外防護預留區：如果沒有輸入就執行，應該要拋出例外
			current = false;
			return;
		}

		bool result = true; // 預設為 true，只有當輸入為奇數個 true 時才會變成 false
		for (Component* input : inputs) {
			if (input->getOutput()) {
				result = !result; // 每遇到一個 true 就切換結果
				if (!result) {
					break; // 如果已經是 false 就不需要繼續檢查了
				}
			}
		}

		current = result;
		cout << "[模擬] " << name << " (XNOR) 計算結果為: " << current << "\n";
	}
};