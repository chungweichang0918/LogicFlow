#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <queue>
#include <limits>
#include <unordered_map>
#include "Component.h"
#include "InputPin.h"

#include "AndGate.h"
#include "OrGate.h"
#include "NotGate.h"
#include "NandGate.h"
#include "XorGate.h"
#include "XNorGate.h"

using namespace std;

class CircuitGraph {
private:
    vector<unique_ptr<Component>> components;

public:
    CircuitGraph() = default;
    ~CircuitGraph() = default; // unique_ptr 會自動清理，毋需手動 delete

    // 清空目前畫布上的所有元件
    void clear() {
        components.clear();
    }

    Component* findComponent(const string& name) {
        for (auto& comp : components) {
            if (comp->getname() == name) {
                return comp.get();
            }
        }
        return nullptr; // 沒有找到
    }

    // 加入新元件。announce 控制是否印出提示
    bool addComponent(unique_ptr<Component> comp, bool announce = true) {
        if (findComponent(comp->getname()) != nullptr) {
            cout << "[錯誤] 已存在同名元件: " << comp->getname() << "，新增失敗！\n";
            return false;
        }
        if (announce) {
            cout << "已加入元件: " << comp->getname() << "\n";
        }
        components.push_back(std::move(comp));
        return true;
    }

    // 負責把兩個元件的線接起來。
    // 可能拋出 CircuitException（例如對 NOT 閘接第二條線時）。
    bool connect(const string& fromName, const string& toName) {
        Component* fromComp = findComponent(fromName);
        Component* toComp = findComponent(toName);

        if (fromComp == nullptr) {
            throw CircuitException("接線失敗：找不到來源元件 [" + fromName + "]");
        }
        if (toComp == nullptr) {
            throw CircuitException("接線失敗：找不到目標元件 [" + toName + "]");
        }

        // addInput() 可能拋出例外 (如 NotGate 拒絕第二條輸入)
        toComp->addInput(fromComp);
        cout << "[系統] 成功將 [" << fromName << "] 接線至 [" << toName << "]\n";
        return true;
    }

    // 將指定元件標記為「輸出」，影響模擬結果的顯示 (取代寫死 SUM / COUT 字串)
    bool markOutput(const string& name) {
        Component* comp = findComponent(name);
        if (comp == nullptr) {
            cout << "[錯誤] 找不到要標記為輸出的元件: " << name << "\n";
            return false;
        }
        comp->setOutput(true);
        return true;
    }

    // 從檔案載入電路。
    // 檔案不存在時拋出 CircuitException；解析過程中的接線錯誤會被捕捉並附上行號資訊。
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            throw CircuitException("找不到檔案: " + filename + "，請確認檔案是否放在正確位置！");
        }

        clear(); // 讀檔前先清空目前的電路
        string line;
        int lineNo = 0;

        while (getline(file, line)) {
            lineNo++;
            if (line.empty()) continue; // 跳過空行

            // 用 try-catch 包住每行的解析，這樣單行出錯不會中斷整份檔案的載入，
            // 同時能精確報告是「第幾行」發生了什麼問題。
            try {
                stringstream ss(line);
                string command;
                ss >> command;

                if (command == "ADD") {
                    string type, name;
                    ss >> type >> name;

                    unique_ptr<Component> newComp;
                    if (type == "PIN") newComp = make_unique<InputPin>(name);
                    else if (type == "AND") newComp = make_unique<AndGate>(name);
                    else if (type == "OR") newComp = make_unique<OrGate>(name);
                    else if (type == "NOT") newComp = make_unique<NotGate>(name);
                    else if (type == "NAND") newComp = make_unique<NandGate>(name);
                    else if (type == "XOR") newComp = make_unique<XorGate>(name);
                    else if (type == "XNOR") newComp = make_unique<XNorGate>(name);

                    if (newComp) {
                        addComponent(std::move(newComp), false);
                    }
                    else {
                        cout << "[警告] 第 " << lineNo << " 行：未知的元件類型 \"" << type << "\"，已略過。\n";
                    }
                }
                else if (command == "CONNECT") {
                    string fromName, toName;
                    ss >> fromName >> toName;
                    connect(fromName, toName); // connect() 內部可能 throw CircuitException
                }
                else if (command == "OUT") {
                    string name;
                    ss >> name;
                    markOutput(name);
                }
                else {
                    cout << "[警告] 第 " << lineNo << " 行：未知指令 \"" << command << "\"，已略過。\n";
                }
            }
            catch (const CircuitException& e) {
                // 捕捉接線或元件相關的例外，附上行號後繼續解析下一行
                cout << "[錯誤] 第 " << lineNo << " 行：" << e.what() << "\n";
            }
        }
        file.close();
        cout << "[系統] 成功從 " << filename << " 載入並建構電路！\n";
        return true;
    }

    void setAllInputValues() {
        bool found = false;
        cout << "\n--- 批次設定輸入接腳 ---\n";

        // 掃描畫布上的每一個元件
        for (auto& comp : components) {
            InputPin* pin = dynamic_cast<InputPin*>(comp.get());
            if (pin != nullptr) {
                found = true;
                int val;
                cout << "請輸入 [" << pin->getname() << "] 的訊號值 (0 或 1): ";
                if (!(cin >> val)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    val = 0;
                    cout << "[警告] 輸入無效，已預設為 0。\n";
                }
                pin->setValue(val != 0);
            }
        }

        if (!found) {
            cout << "[系統] 目前畫布上沒有任何輸入接腳！\n";
        }
        else {
            cout << "--- 所有輸入接腳設定完畢 ---\n";
        }
    }

    bool saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cout << "[錯誤] 無法開啟檔案: " << filename << "\n";
            return false;
        }
        // 先寫入所有元件的定義 (改用 typeName() 取代一長串 dynamic_cast)
        for (auto& comp : components) {
            file << "ADD " << comp->typeName() << " " << comp->getname() << "\n";
        }
        // 再寫入所有連線
        for (auto& comp : components) {
            for (Component* input : comp->getInput()) {
                file << "CONNECT " << input->getname() << " " << comp->getname() << "\n";
            }
        }
        // 最後寫入輸出標記
        for (auto& comp : components) {
            if (comp->getIsOutput()) {
                file << "OUT " << comp->getname() << "\n";
            }
        }
        file.close();
        cout << "[系統] 成功將電路儲存至 " << filename << "！\n";
        return true;
    }

    // 以拓樸排序 (Kahn's algorithm) 決定計算順序，確保上游先算完；
    // 同時能偵測回授迴路 (cycle)，避免結果錯誤或無限等待。
    void simulate() {
        unordered_map<Component*, int> indeg;                 // 入度 = 該元件的輸入數
        unordered_map<Component*, vector<Component*>> consumers; // 出邊：誰用了我的輸出

        for (auto& up : components) {
            indeg[up.get()] = 0; // 確保每個節點都有鍵值
        }
        for (auto& up : components) {
            Component* c = up.get();
            for (Component* in : c->getInput()) {
                consumers[in].push_back(c);
                indeg[c]++;
            }
        }

        // 入度為 0 的節點先入列 (通常是 InputPin)
        queue<Component*> q;
        for (auto& up : components) {
            if (indeg[up.get()] == 0) {
                q.push(up.get());
            }
        }

        vector<Component*> order;
        while (!q.empty()) {
            Component* c = q.front();
            q.pop();
            order.push_back(c);
            for (Component* cons : consumers[c]) {
                if (--indeg[cons] == 0) {
                    q.push(cons);
                }
            }
        }

        // 若排序後的數量不等於總數，代表電路中存在回授迴路，拋出例外
        if (order.size() != components.size()) {
            throw CircuitException("偵測到回授迴路 (cycle)！目前僅支援組合邏輯電路，無法完成模擬。");
        }

        // 依正確的依賴順序計算
        for (Component* c : order) {
            c->compute();
        }

        // ---------- 顯示結果 ----------
        cout << "\n========== 模擬結果 ==========\n";
        cout << "【輸入接腳狀態】\n";
        for (auto& comp : components) {
            if (dynamic_cast<InputPin*>(comp.get()) != nullptr) {
                cout << "  " << comp->getname() << " = " << comp->getOutput() << "\n";
            }
        }

        cout << "---------------------------------\n";
        cout << "【輸出結果】\n";

        // 是否有任何被標記的輸出元件
        bool hasMarked = false;
        for (auto& comp : components) {
            if (comp->getIsOutput()) { hasMarked = true; break; }
        }

        for (auto& comp : components) {
            bool isPin = (dynamic_cast<InputPin*>(comp.get()) != nullptr);
            if (hasMarked) {
                // 有標記輸出：只顯示被標記者
                if (comp->getIsOutput()) {
                    cout << "  " << comp->getname() << " = " << comp->getOutput() << "\n";
                }
            }
            else {
                // 沒有任何標記時：退而顯示所有邏輯閘 (不再寫死 SUM / COUT)
                if (!isPin) {
                    cout << "  " << comp->getname() << " = " << comp->getOutput() << "\n";
                }
            }
        }
        cout << "=================================\n";
    }
};
