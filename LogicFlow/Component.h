#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

// 自訂例外類別：用於電路操作中的非法狀態 (如迴路偵測、違規接線等)。
// 繼承自 runtime_error。
class CircuitException : public runtime_error {
public:
    explicit CircuitException(const string& msg) : runtime_error(msg) {}
};

// 所有電路元件的基底類別
class Component {
protected:
    string name;
    vector<Component*> inputs;
    bool current = false;
    bool isOutput = false;

    // 子類別只需實作純粹的邏輯運算；empty 檢查與 log 交給 compute() 統一處理
    virtual bool evaluate() const = 0;

public:
    // 回傳元件類型字串，供存檔與 log 共用
    virtual string typeName() const = 0;
    
    static bool verbose;

    Component(const string& name) : name(name) {}
    virtual ~Component() = default;

    string getname() const { return name; }
    bool getOutput() const { return current; }

    void setOutput(bool v) { isOutput = v; }
    bool getIsOutput() const { return isOutput; }

    virtual void addInput(Component* a) {
        inputs.push_back(a);
    }

    // 範本方法 (Template Method)：統一處理空輸入防護與除錯輸出，
    // 子類別只要專心實作 evaluate() 的純邏輯即可。
    virtual void compute() {
        if (inputs.empty()) {
            current = false;
            return;
        }
        current = evaluate();
        if (verbose) {
            cout << "[模擬] " << name << " (" << typeName()
                << ") 計算結果為: " << current << "\n";
        }
    }

    const vector<Component*>& getInput() const { return inputs; }
};
