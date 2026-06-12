# LogicFlow — 數位邏輯電路模擬器

> 國立臺中科技大學 資訊工程系｜進階程式設計 期末專題

一套以 C++ 開發的主控台式數位邏輯電路模擬器，支援六種基本邏輯閘的自由組合、拓樸排序模擬、`.cir` 格式電路檔案讀寫，以及完善的例外處理機制。

## 功能特色

- **6 種邏輯閘**：AND、OR、NOT、NAND、XOR、XNOR
- **拓樸排序模擬**：Kahn's Algorithm 確保正確的計算順序，同時自動偵測回授迴路
- **檔案 I/O**：自訂 `.cir` 純文字格式（ADD / CONNECT / OUT 三段式指令）
- **記憶體安全**：`unique_ptr` 智慧指標自動管理元件生命週期
- **Template Method 模式**：`compute()` → `evaluate()` 消除重複程式碼
- **try-catch 例外處理**：自訂 `CircuitException` 類別，雙層 catch 防禦架構

## 專案結構

```
LogicFlow/
├── main.cpp               # 主程式進入點、選單邏輯
├── Component.h            # 抽象基底類別 + CircuitException 自訂例外
├── InputPin.h             # 輸入接腳
├── AndGate.h              # AND 閘
├── OrGate.h               # OR 閘
├── NotGate.h              # NOT 閘（含單輸入限制與例外拋出）
├── NandGate.h             # NAND 閘
├── XorGate.h              # XOR 閘
├── XNorGate.h             # XNOR 閘
├── CircuitGraph.h         # 電路圖管理器（拓樸排序、檔案 I/O、try-catch）
├── half_adder.cir         # 半加器範例電路
├── full_adder.cir         # 全加器範例電路
├── adder_4bit.cir         # 4-bit 漣波進位加法器
├── mux_2to1.cir           # 2-to-1 多工器
├── test_circuit.cir       # 全閘種測試電路
└── circuits/              # 範例電路備份
```

## 編譯與執行

```bash
# Linux / macOS
g++ -std=c++14 -Wall main.cpp -o logicflow
./logicflow

# Windows（Developer Command Prompt）
cl /std:c++14 /EHsc main.cpp /Fe:logicflow.exe
logicflow.exe
```

相容 C++14 / C++17 / C++20，可直接使用 Visual Studio 開啟 `.vcxproj` 建置。

## .cir 檔案格式

以半加器為例：

```
ADD PIN IN_A
ADD PIN IN_B
ADD XOR GATE_SUM
ADD AND GATE_CARRY
CONNECT IN_A GATE_SUM
CONNECT IN_B GATE_SUM
CONNECT IN_A GATE_CARRY
CONNECT IN_B GATE_CARRY
OUT GATE_SUM
OUT GATE_CARRY
```

| 指令 | 說明 |
|------|------|
| `ADD <類型> <名稱>` | 宣告元件（PIN / AND / OR / NOT / NAND / XOR / XNOR） |
| `CONNECT <來源> <目標>` | 建立訊號接線 |
| `OUT <名稱>` | 標記為輸出端點 |

## 使用技術

| 技術 | 說明 |
|------|------|
| OOP | 抽象類別、繼承、多型、虛擬函式 |
| 設計模式 | Template Method（compute → evaluate） |
| 資料結構 | vector、queue、unordered_map |
| 演算法 | Kahn's Algorithm（拓樸排序 + 迴路偵測） |
| 記憶體管理 | unique_ptr（RAII 自動釋放） |
| 例外處理 | 自訂 CircuitException + 雙層 try-catch |

## 團隊成員

| 姓名 | 學號 | 負責項目 |
|------|------|---------|
| 張鈞崴 | 1111232077 | 核心架構設計、檔案 I/O、.cir 格式設計、測試 |
| 黃麟凱 | 1111232083 | 元件指標管理、標頭檔互通測試 |
| 葉芷彤 | 1111103034 | 邏輯閘實作、拓樸排序 |
