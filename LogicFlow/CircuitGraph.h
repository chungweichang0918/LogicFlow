#pragma once
#include <vector>
#include <string>
#include "Component.h"

using namespace std;

class CircuitGraph {
private:
	vector <Component*> components;
public:
	~CircuitGraph() {
		// 釋放元件的記憶體
		for (Component* comp : components) {
			delete comp;
		}
		components.clear();
	}

	Component* findComponent(const string& name) {
		for (Component* comp : components) {
			if (comp -> getname() == name) {
				return comp;
			}
		}
		return nullptr; // 沒有找到
	}

	// 負責把兩個元件的線接起來
    bool connect(const string& fromName, const string& toName) {
        Component* fromComp = findComponent(fromName);
        Component* toComp = findComponent(toName);

        if (fromComp == nullptr) {
            cout << "[錯誤] 找不到來源元件: " << fromName << "\n";
            return false;
        }
        if (toComp == nullptr) {
            cout << "[錯誤] 找不到目標元件: " << toName << "\n";
            return false;
        }

        // 把 fromComp 加到 toComp 的 inputs 裡面
        toComp->addInput(fromComp);
        cout << "[系統] 成功將 [" << fromName << "] 接線至 [" << toName << "]\n";
        return true;
    }

	// 加入新元件
	void addComponent(Component* comp) {
		components.push_back(comp);
		cout << "已加入元件: " << comp->getname() << "\n";
	}

	void dummySimulate() {
		cout << "開始模擬電路...\n";
		for (Component* comp : components) {
			comp->compute();
		}
		cout << "模擬結束。\n";
	}
};