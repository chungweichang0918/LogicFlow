#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <cstdlib>
#include "CircuitGraph.h"

#include "InputPin.h"
#include "AndGate.h"
#include "OrGate.h"
#include "NotGate.h"
#include "XorGate.h"
#include "XNorGate.h"
#include "NandGate.h"

using namespace std;

// Component::verbose 的定義 (宣告在 Component.h)。整個專案只能定義一次，放在 main.cpp 最合適。
bool Component::verbose = true;

// 安全讀取整數：遇到非數字輸入時清除錯誤旗標並回傳 false，避免選單陷入無限迴圈。
bool readInt(int& out) {
    cin >> out;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// 依選項建立對應的邏輯閘 (1:AND 2:OR 3:NOT 4:NAND 5:XOR 6:XNOR)，失敗回傳 nullptr。
unique_ptr<Component> makeGate(int choice, const string& name) {
    switch (choice) {
    case 1: return make_unique<AndGate>(name);
    case 2: return make_unique<OrGate>(name);
    case 3: return make_unique<NotGate>(name);
    case 4: return make_unique<NandGate>(name);
    case 5: return make_unique<XorGate>(name);
    case 6: return make_unique<XNorGate>(name);
    default: return nullptr;
    }
}

void displayMenu() {
    cout << "\n=== LogicFlow 數位電路模擬器 ===\n";
    cout << "1~6. 單獨建立邏輯閘 (1:AND 2:OR 3:NOT 4:NAND 5:XOR 6:XNOR)\n";
    cout << "7. 建接腳 -> 建邏輯閘 -> 接線\n";
    cout << "8. 單獨連接元件 (手動接線)\n";
    cout << "9. 批次設定所有 InputPin 的訊號值\n";
    cout << "10. 執行電路模擬\n";
    cout << "11. 從檔案載入電路 (.cir)\n";
    cout << "12. 儲存目前電路到檔案 (.cir)\n";
    cout << "13. 標記輸出元件 (OUT)\n";
    cout << "14. 切換詳細計算過程顯示 (verbose)\n";
    cout << "0. 離開系統\n";
    cout << "---------------------------------\n";
    cout << "請輸入指令: ";
}

int main() {
    CircuitGraph graph;
    int choice = -1;

    while (choice != 0) {
        displayMenu();
        if (!readInt(choice)) {
            cout << "無效輸入，請輸入數字指令。\n";
            continue;
        }

        // 用 try-catch 包住每一輪的指令處理：
        // 當 connect / simulate / loadFromFile 等操作拋出 CircuitException 時，
        // 在這裡統一攔截並印出錯誤訊息，程式不會崩潰而是回到主選單繼續運行。
        try {

            if (choice >= 1 && choice <= 6) {
                string gateName;
                cout << "請為邏輯閘命名 (例如 G1): ";
                cin >> gateName;

                unique_ptr<Component> newGate = makeGate(choice, gateName);
                if (newGate) graph.addComponent(std::move(newGate));
            }

            // 指令 7：快速建構接腳 + 邏輯閘 + 自動接線
            else if (choice == 7) {
                graph.clear();
                string currentTargetGate = "";

                // --- 建立第一個輸入接腳 ---
                string pinName;
                int val;
                cout << "\n【步驟 1：建立第一個輸入接腳】\n";
                cout << "請為輸入接腳命名 (例如 IN_A): ";
                cin >> pinName;

                auto newPin = make_unique<InputPin>(pinName);
                cout << "請輸入 [" << pinName << "] 的初始訊號值 (0 或 1): ";
                if (!readInt(val)) { val = 0; cout << "[警告] 輸入無效，已預設為 0。\n"; }
                newPin->setValue(val != 0);
                graph.addComponent(std::move(newPin));

                // --- 確認目標邏輯閘 ---
                cout << "\n【步驟 2：設定目標邏輯閘】\n";
                cout << "請選擇 (1:AND, 2:OR, 3:NOT, 4:NAND, 5:XOR, 6:XNOR, 0:不建立新邏輯閘): ";
                int gateChoice;
                if (!readInt(gateChoice)) { gateChoice = 0; cout << "[警告] 輸入無效，視為不建立新邏輯閘。\n"; }

                if (gateChoice >= 1 && gateChoice <= 6) {
                    // 建立新邏輯閘
                    cout << "請為這個新邏輯閘命名 (例如 G1): ";
                    cin >> currentTargetGate;

                    unique_ptr<Component> newGate = makeGate(gateChoice, currentTargetGate);
                    if (newGate) graph.addComponent(std::move(newGate));

                    // 自動接線
                    cout << "[系統] 自動將 [" << pinName << "] 連接至 [" << currentTargetGate << "]\n";
                    graph.connect(pinName, currentTargetGate);
                }
                else {
                    cout << "您剛剛沒有建立新邏輯閘。\n";
                    cout << "請問要將 [" << pinName << "] 連接到哪個【已存在】的邏輯閘？\n";
                    cout << "(請輸入目標元件名稱，若不想接線請輸入 0 跳過): ";
                    cin >> currentTargetGate;

                    if (currentTargetGate != "0") {
                        graph.connect(pinName, currentTargetGate);
                    }
                    else {
                        currentTargetGate = "";
                        cout << "[系統] 已跳過接線步驟。\n";
                    }
                }

                // --- 無限擴充腳位 (B、C、D...) ---
                if (gateChoice == 3) {
                    cout << "[系統] NOT 閘僅支援單一輸入，已自動跳過接腳擴充流程！\n";
                }
                else if (currentTargetGate != "") {
                    while (true) {
                        cout << "\n【步驟 3：繼續新增接腳？】\n";
                        cout << "是否還要新增其他接腳 並自動連接到 [" << currentTargetGate << "]？\n";
                        cout << "(1:是, 0:否，完成設定): ";
                        int addMore;
                        if (!readInt(addMore)) { addMore = 0; }

                        if (addMore == 1) {
                            string nextPinName;
                            cout << "請為【新接腳】命名: ";
                            cin >> nextPinName;

                            auto nextPin = make_unique<InputPin>(nextPinName);
                            cout << "請輸入 [" << nextPinName << "] 的初始訊號值 (0 或 1): ";
                            if (!readInt(val)) { val = 0; cout << "[警告] 輸入無效，已預設為 0。\n"; }
                            nextPin->setValue(val != 0);
                            graph.addComponent(std::move(nextPin));

                            cout << "[系統] 瞬間將 [" << nextPinName << "] 連接至 [" << currentTargetGate << "]\n";
                            graph.connect(nextPinName, currentTargetGate);
                        }
                        else {
                            break;
                        }
                    }
                }

                cout << "\n--- 設定完畢！ ---\n";
            }

            // 單獨連接元件 (8)
            else if (choice == 8) {
                string fromName, toName;
                cout << "請輸入【來源】元件名稱 (訊號給出者): ";
                cin >> fromName;
                cout << "請輸入【目標】元件名稱 (訊號接收者): ";
                cin >> toName;
                graph.connect(fromName, toName);
            }

            // 修改訊號值 (9)
            else if (choice == 9) {
                graph.setAllInputValues();
            }

            // 執行模擬 (10)
            else if (choice == 10) {
                graph.simulate();
            }

            // 載入檔案 (11)
            else if (choice == 11) {
                string filename;
                cout << "請輸入要載入的檔案名稱 (包含副檔名，例如 half_adder.cir): ";
                cin >> filename;
                graph.loadFromFile(filename);
            }

            // 儲存檔案 (12)
            else if (choice == 12) {
                string filename;
                cout << "請輸入要儲存的檔案名稱 (包含副檔名，例如 my_circuit.cir): ";
                cin >> filename;
                graph.saveToFile(filename);
            }

            // 標記輸出元件 (13)
            else if (choice == 13) {
                string name;
                cout << "請輸入要標記為【輸出】的元件名稱: ";
                cin >> name;
                if (graph.markOutput(name)) {
                    cout << "[系統] 已將 [" << name << "] 標記為輸出元件。\n";
                }
            }

            // 切換詳細模式 (14)
            else if (choice == 14) {
                Component::verbose = !Component::verbose;
                cout << "[系統] 詳細計算過程顯示已" << (Component::verbose ? "開啟" : "關閉") << "。\n";
            }

            // 離開系統 (0)
            else if (choice == 0) {
                cout << "感謝使用 LogicFlow，系統關閉中...\n";
            }
            else {
                cout << "無效指令，請重新輸入。\n";
            }

        } // end try
        catch (const CircuitException& e) {
            cout << "[例外] " << e.what() << "\n";
        }
        catch (const exception& e) {
            cout << "[未預期錯誤] " << e.what() << "\n";
        }
    }
    return 0;
}
