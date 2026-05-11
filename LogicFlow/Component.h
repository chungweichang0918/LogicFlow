#pragma once
#include <string>
#include <vector>

using namespace std;

// 所有電路元件 (輸入腳、邏輯閘) 的基底類別
class Component {
protected:
	string name;
	vector<Component*> inputs;
	bool current = false;
public:
	Component(const string name) : name(name) {}
	virtual ~Component() = default;

	string getname() const {
		return name;
	}
	bool getOutput() const {
		return current;
	}

	virtual void addInput(Component* a) {
		inputs.push_back(a);
	}
	virtual void compute() = 0;
};