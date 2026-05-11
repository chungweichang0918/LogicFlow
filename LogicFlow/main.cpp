#include <iostream>
#include <string>
#include <cstdlib> 
#include "CircuitGraph.h"

// 引入所有元件
#include "InputPin.h"
#include "AndGate.h"
#include "OrGate.h"
#include "NotGate.h"
#include "XorGate.h"
#include "XNorGate.h"
#include "NandGate.h"

using namespace std;

void displayMenu() {
    cout << "\n=== LogicFlow 數位電路模擬器 ===\n";
    cout << "1~6. 建立邏輯閘 (1:AND, 2:OR, 3:NOT, 4:NAND, 5:XOR, 6:XNOR)\n";
    cout << "7. 建立 InputPin (輸入接腳)\n";
    cout << "8. 連接元件 (接線)\n";
    cout << "9. 設定 InputPin 的訊號值 (0 或 1)\n";
    cout << "10. 執行電路模擬\n";
    cout << "0. 離開系統\n";
    cout << "---------------------------------\n";
    cout << "請輸入指令: ";
}

int main() {
    CircuitGraph graph;
    int choice = -1;

    while (choice != 0) {
        displayMenu();
        cin >> choice;

        // 建立邏輯閘 (1~6)
        if (choice >= 1 && choice <= 6) {
            string gateName;
            cout << "請為邏輯閘命名 (例如 G1): ";
            cin >> gateName;

            Component* newGate = nullptr;
            switch (choice) {
            case 1: newGate = new AndGate(gateName); break;
            case 2: newGate = new OrGate(gateName); break;
            case 3: newGate = new NotGate(gateName); break;
            case 4: newGate = new NandGate(gateName); break;
            case 5: newGate = new XorGate(gateName); break;
            case 6: newGate = new XNorGate(gateName); break;
            }
            if (newGate) graph.addComponent(newGate);
        }
        // 建立輸入接腳 (7)
        else if (choice == 7) {
            string pinName;
            cout << "請為輸入接腳命名 (例如 IN_A): ";
            cin >> pinName;
            graph.addComponent(new InputPin(pinName));
        }
        // 連接元件 (8)
        else if (choice == 8) {
            string fromName, toName;
            cout << "請輸入【來源】元件名稱: ";
            cin >> fromName;
            cout << "請輸入【目標】元件名稱: ";
            cin >> toName;
            graph.connect(fromName, toName);
        }
        // 設定訊號值 (9)
        else if (choice == 9) {
            string pinName;
            int val;
            cout << "請輸入要設定的接腳名稱: ";
            cin >> pinName;

            Component* comp = graph.findComponent(pinName);
            if (comp) {
                // 將 Component 指標轉型為 InputPin 指標
                InputPin* pin = dynamic_cast<InputPin*>(comp);
                if (pin) {
                    cout << "請輸入訊號值 (0 或 1): ";
                    cin >> val;
                    pin->setValue(val != 0);
                }
                else {
                    cout << "[錯誤] 這不是輸入接腳，不能直接設定值！\n";
                }
            }
            else {
                cout << "[錯誤] 找不到這個元件！\n";
            }
        }
        // 執行模擬 (10)
        else if (choice == 10) {
            graph.dummySimulate();
        }
        else if (choice == 0) {
            cout << "感謝使用 LogicFlow，系統關閉中...\n";
        }
        else {
            cout << "無效指令，請重新輸入。\n";
        }
    }
    return 0;
}