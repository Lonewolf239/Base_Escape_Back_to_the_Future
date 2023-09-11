#include <iostream>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <string>
#include <sstream>
#include <ctime>
#include "ini\INIReader.h"
#pragma comment(lib, "user32.lib")

using namespace std;

//инвентарь
//глава 1
bool isHasKnife = false, isHasKey1 = false;
//глава 2
bool isHasCrowbar = false, tear_off = false, isHasMaul = false,
first_try_crowbar = false, on_theehouse = false, boards_fell_off = false, first_tree_inspect = false,
light_on = false;

//параметры
HANDLE hMutex = NULL;
bool first_start = true, win64 = false;
int serial;
string text = "\033[38;2;0;170;0m",
player = "\n\033[38;2;0;255;0m > ";
string player_choice, save_str = "none";
string save_folder = "C:\\Program Files\\BaseEscapeData\\base_escape_bthf.ini";
RECT r;
POINT p;
//коды
string save_1 = "X66AoYgdS}fsN~yLDH%3}hiM#O%t6v~S";


//костыли функций
//глава 1
void start();
void save();
void location();
void cycle1();
void window();
void closet();
void escape();
//глава 2
void cycle2();
void yard();
void treehouse();
void enter_mansion();



bool check() {
    if (win64) {
        ofstream mig("C:\\Program Files (x86)\\BaseEscapeData\\test.txt");
        if (mig.is_open()) {
            mig.close();
            remove("C:\\Program Files (x86)\\BaseEscapeData\\test.txt");
            return true;
        }
        else
            return false;
    }
    else {
        ofstream mig("C:\\Program Files\\BaseEscapeData\\test.txt");
        if (mig.is_open()) {
            mig.close();
            remove("C:\\Program Files\\BaseEscapeData\\test.txt");
            return true;
        }
        else
            return false;
    }
}
//main
int main() {
    if (first_start) {
#ifdef _WIN64
        save_folder = "C:\\Program Files (x86)\\BaseEscapeData\\base_escape_bthf.ini";
        win64 = true;
#endif
        if (!check()) {
            if (win64)
                CreateDirectoryW(L"C:\\Program Files (x86)\\BaseEscapeData", NULL);
            else
                CreateDirectoryW(L"C:\\Program Files\\BaseEscapeData", NULL);
        }
        if (first_start) {
            LPCWSTR mutexName = L"Base_Escape_Back_to_the_Future_Unique_Mutex";
            hMutex = CreateMutex(NULL, TRUE, mutexName);
            if (GetLastError() == ERROR_ALREADY_EXISTS)
                return 0;
        }
        SetConsoleTitleA("Base_Escape - Back to the Future");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_FONT_INFOEX fontInfo;
        fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
        wcscpy_s(fontInfo.FaceName, L"Courier New");
        fontInfo.dwFontSize.Y = 36;
        fontInfo.dwFontSize.X = 18;
        SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
        system("mode con cols=100 lines=60");
        HWND console = GetConsoleWindow();
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MINIMIZEBOX & ~WS_SIZEBOX);
        ShowWindow(console, SW_MAXIMIZE);
        HANDLE consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
        DWORD consoleMode;
        GetConsoleMode(consoleHandle, &consoleMode);
        SetConsoleMode(consoleHandle, consoleMode & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS | ENABLE_INSERT_MODE));
        srand(static_cast<unsigned int>(time(NULL)));
        first_start = false;
    }
    INIReader read(save_folder);
    if (read.ParseError() < 0) {
        ofstream config(save_folder);
        if (config.is_open()) {
            serial = (rand() % 100000 * (9999 + rand())) + rand() % 100000;
            if(serial >= 99999999)
                serial -= rand() % 100000 + rand() % 100000;
            else if (serial <= 999999)
                serial += rand() % 100000 + rand() % 100000;
            ostringstream data;
            data << "[DATA]\nsave=" << save_str << "\nserial=" << serial;
            config << data.str();
            config.close();
        }
    }
    else {
        serial = read.GetInteger("DATA", "serial", 0);
        save_str = read.GetString("DATA", "save", "none");
    }
    bool not_start = true;
    while (not_start) {
        system("cls");
        cout << text << "Copyright (c) 1984-1995 Lonewolf239, Inc. All right reserved.\nRevision: 110923 / Serial number: " << serial << "\n\n";
        cout << "Welcome to the game \"Base_Escape - Back to the Future\"\n"
            "Version: 0.2  Developer: Lonewolf239  Icon: qsvhu\n"
            "Control: Describe the desired action to perform it.\n\n"
            "This is an exciting text-based game in which you will strive to survive by fighting against a\nbloodthirsty maniac, and possibly an entire cult...\n"
            "During the game, you will need to constantly look for the right items\nand use them wisely to ensure your escape.\nWill you find a way to survive in this world of terrifying nightmares?.." << player;
        getline(cin, player_choice);
        transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
        if (player_choice == "start" || player_choice == "start game" || player_choice == "start the game") {
            not_start = false;
            start();
        }
        else if (player_choice == "load" || player_choice == "load game" || player_choice == "load the game" || player_choice == "load save" || player_choice == "load the save") {
            not_start = false;
            save();
        }
        else if (player_choice == "oh gib mir kraft") {
            cout << text << "Mutter...Mutter..." << player << "Press any key to continue...";
            system("pause >NUL");
        }
        else if (player_choice == "exit" || player_choice == "exit game" || player_choice == "exit the game")
            exit(0);
    }
    if (hMutex != 0)
        CloseHandle(hMutex);
    return 0;
}
//старт игры
void start() {
    isHasKnife = false, isHasKey1 = false,
        isHasCrowbar = false, tear_off = false,
        isHasMaul = false, first_try_crowbar = false,
        on_theehouse = false, boards_fell_off = false,
        first_tree_inspect = false, light_on = false;
    bool not_start = true;
    cout << text << "\n\n                  |=======================|\n"
        "                  |  Chapter 1. Basement  |\n"
        "                  |=======================|\n\n\n";
    while (not_start) {
        cout << text << "You woke up in an unknown place. Looks like a basement..." << player;
        getline(cin, player_choice);
        transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
        if (player_choice == "look around" || player_choice == "look" || player_choice == "explore" || player_choice == "inspect" || player_choice == "take a look around" || player_choice == "take look around") {
            cout << text << "Looking around, you realized that you did not see anything :(" << player;
            getline(cin, player_choice);
            cout << text << "Looking around AT FEEL, you realized that it was a small room...\nYou found: Window, Closet, Door, Carpet" << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            not_start = false;
            location();
        }
    }
}
//загрузка сохранения
void save() {
    isHasKnife = false, isHasKey1 = false,
        isHasCrowbar = false, tear_off = false, 
        isHasMaul = false, first_try_crowbar = false,
        on_theehouse = false, boards_fell_off = false,
        first_tree_inspect = false, light_on = false;
    GetCursorPos(&p);
    SetCursorPos(0, 900000000);
    BlockInput(true);
    int load = 3 + rand() % 3;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &structCursorInfo);
    for (int i = 0; i < load; i++) {
        cout << "\r" << text << "LOADING SAVE.  " << flush;
        Sleep(200);
        cout << "\r" << text << "LOADING SAVE.. " << flush;
        Sleep(200);
        cout << "\r" << text << "LOADING SAVE..." << flush;
        Sleep(200);
    }
    INIReader read(save_folder);
    if (read.ParseError() < 0) {
        ofstream config(save_folder);
        if (config.is_open()) {
            ostringstream data;
            data << "[DATA]\nsave=" << save_str << "\nserial=" << serial;
            config << data.str();
            config.close();
        }
        cout << text << "\nSAVE FILE MISSING!\n";
        Sleep(1000);
        SetCursorPos(p.x, p.y);
        BlockInput(false);
        GetConsoleCursorInfo(handle, &structCursorInfo);
        structCursorInfo.bVisible = TRUE;
        SetConsoleCursorInfo(handle, &structCursorInfo);
        start();
    }
    else {
        save_str = read.GetString("DATA", "save", "none");
        if (save_str == save_1) {
            cout << text << "\nSAVE LOADED SUCCESSFULLY\n";
            Sleep(1000);
            SetCursorPos(p.x, p.y);
            BlockInput(false);
            GetConsoleCursorInfo(handle, &structCursorInfo);
            structCursorInfo.bVisible = TRUE;
            SetConsoleCursorInfo(handle, &structCursorInfo);
            cout << text << "\n\n                  |=======================|\n"
                "                  |    Chapter 2. Yard    |\n"
                "                  |=======================|\n\n";
            cycle2();
        }
        else if (save_str == "none") {
            cout << text << "\nSAVE FILE MISSING!\n";
            Sleep(1000);
            SetCursorPos(p.x, p.y);
            BlockInput(false);
            GetConsoleCursorInfo(handle, &structCursorInfo);
            structCursorInfo.bVisible = TRUE;
            SetConsoleCursorInfo(handle, &structCursorInfo);
            start();
        }
        else {
            cout << text << "\nSAVE FILE CORRUPTED!\n";
            Sleep(1000);
            SetCursorPos(p.x, p.y);
            BlockInput(false);
            GetConsoleCursorInfo(handle, &structCursorInfo);
            structCursorInfo.bVisible = TRUE;
            SetConsoleCursorInfo(handle, &structCursorInfo);
            start();
        }
    }
}

//Глава 1
// 
//цикл подвала
void cycle1() {
    string inventory;
    if (isHasKnife)
        inventory += "\nYou have: Knife...";
    if (isHasKey1)
        inventory += "\nYou have: Key...";
    cout << text << "You are standing in the center of the basement...\nWindow, Closet, Door, Carpet" << inventory << player;
    getline(cin, player_choice);
    transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
    location();
}
//подвал
void location() {
    bool not_leave = true;
    if (player_choice == "window" || player_choice == "go to the window" || player_choice == "go to window" || player_choice == "approach the window") {
        bool window_first = true;
        while (not_leave) {
            if (window_first) {
                cout << text << "You went to the window and tried to open it...it's closed..." << player;
                window_first = false;
            }
            else
                cout << text << "You are standing at the window..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "look" || player_choice == "look out the window" || player_choice == "look out window" || player_choice == "look outside the window") {
                cout << text << "You looked out the window...there is night..." << player;
                getline(cin, player_choice);
                transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
                not_leave = false;
                window();
            }
            else if (player_choice == "open" || player_choice == "open the window" || player_choice == "open window") {
                cout << text << "Nothing happened..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
            }
            else if (player_choice == "leave" || player_choice == "get away from the window" || player_choice == "leave the window" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the room..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle1();
            }
        }
    }
    else if (player_choice == "closet" || player_choice == "go to the closet" || player_choice == "go to closet" || player_choice == "approach the closet") {
        bool closet_first = true;
        while (not_leave) {
            if (closet_first) {
                cout << text << "You have reached a closet... it is divided into two compartments, a lower and an upper one..." << player;
                closet_first = false;
            }
            else
                cout << text << "You're standing near the closet..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "top" || player_choice == "open top drawer" || player_choice == "open the top drawer" || player_choice == "open top") {
                not_leave = false;
                closet();
            }
            else if (player_choice == "bottom" || player_choice == "open bottom drawer" || player_choice == "open the lower drawer" || player_choice == "open the bottom drawer" || player_choice == "open bottom") {
                not_leave = false;
                closet();
            }
            else if (player_choice == "leave" || player_choice == "get away from the closet" || player_choice == "leave the closet" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the room..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle1();
            }
        }
    }
    else if (player_choice == "door" || player_choice == "go to the door" || player_choice == "go to door" || player_choice == "approach the door") {
        bool door_first = true;
        while (not_leave) {
            if (door_first) {
                cout << text << "You have reached the door... it is locked..." << player;
                door_first = false;
            }
            else
                cout << text << "You're standing near the door... it is locked..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "open" || player_choice == "open the door" || player_choice == "unlock the door" || player_choice == "open the lock" || player_choice == "open the door lock") {
                if (isHasKey1) {
                    cout << text << "You inserted the key into the lock and turned...\nThe lock opened and fell to the floor with a clang of metal..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                    escape();
                }
                else {
                    cout << text << "I can't open this door..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "inspect" || player_choice == "inspect the door" || player_choice == "examine the door" || player_choice == "inspect the lock" || player_choice == "examine the lock") {
                if (isHasKey1) {
                    cout << text << "You examined the door lock...\nIt seems your key matches it..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else {
                    cout << text << "Have you inspected the door lock...\nIt seems there must be a key to it somewhere..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "leave" || player_choice == "get away from the door" || player_choice == "leave the door" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the room..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle1();
            }
        }
    }
    else if (player_choice == "carpet" || player_choice == "look at the carpet" || player_choice == "look at the rug" || player_choice == "inspect the carpet" || player_choice == "examine the carpet") {
        bool carpet_first = true;
        while (not_leave) {
            if (!isHasKey1) {
                if (carpet_first) {
                    cout << text << "You looked at the carpet under your feet...\nIt looks like there's something under it...\nBut its edges are concreted..." << player;
                    carpet_first = false;
                }
                else
                    cout << text << "You're looking at the carpet under your feet...\nIt looks like there's something under it...\nBut its edges are glued down..." << player;
            }
            else {
                if (carpet_first) {
                    cout << text << "You looked at the carpet under your feet...\n.....why did you cut it in the shape of Among Us?..." << player;
                    carpet_first = false;
                }
                else
                    cout << text << "You're looking at the carpet under your feet...\n.....why did you cut it in the shape of Among Us?..." << player;
            }
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "cut" || player_choice == "cut the carpet" || player_choice == "cut the rug" || player_choice == "cut carpet") {
                if (isHasKey1) {
                    cout << text << "The carpet seemed to have already been cut..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else if (isHasKnife) {
                    cout << text << "You cut the carpet with difficulty... You broke the knife...\nBut you got the key from under the carpet..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                    isHasKey1 = true;
                    isHasKnife = false;
                    not_leave = false;
                    cycle1();
                }
                else {
                    cout << text << "I can't cut this carpet..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "leave" || player_choice == "get away from the carpet" || player_choice == "leave the carpet" || player_choice == "go away") {
                cout << text << "You stopped looking at the carpet..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle1();
            }
        }
    }
    else
        cycle1();
}
//окно
void window() {
    bool not_leave = true, show = false;
    while (not_leave) {
        if (show) {
            cout << text << "You are standing at the window..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
        }
        show = true;
        if (player_choice == "open" || player_choice == "open the window" || player_choice == "open window") {
            cout << text << "Nothing happened..." << player << "Press any key to continue...";
            system("pause >NUL");
            cout << endl;
        }
        else if (player_choice == "leave" || player_choice == "get away from the window" || player_choice == "leave the window" || player_choice == "go away") {
            cout << text << "You turned around and went to the center of the room..." << player << "Press any key to continue...";
            system("pause >NUL");
            cout << endl;
            not_leave = false;
            cycle1();
        }
    }
}
//шкаф
void closet() {
    bool not_leave = true;
    if (player_choice == "top" || player_choice == "open top drawer" || player_choice == "open the top drawer" || player_choice == "open top") {
        while (not_leave) {
            cout << text << "You have opened the top drawer... it is empty..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "bottom" || player_choice == "open bottom drawer" || player_choice == "open the lower drawer" || player_choice == "open the bottom drawer" || player_choice == "open bottom") {
                not_leave = false;
                closet();
            }
            else if (player_choice == "leave" || player_choice == "get away from the closet" || player_choice == "leave the closet" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the room..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle1();
            }
        }
    }
    else if (player_choice == "bottom" || player_choice == "open bottom drawer" || player_choice == "open the lower drawer" || player_choice == "open the bottom drawer" || player_choice == "open bottom") {
        while (not_leave) {
            if (!isHasKnife && !isHasKey1)
                cout << text << "You have opened the bottom drawer... there is a knife..." << player;
            else
                cout << text << "You have opened the bottom drawer... it is empty..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "take" || player_choice == "get" || player_choice == "get a knife" || player_choice == "take a knife" || player_choice == "get knife" || player_choice == "take knife") {
                if (!isHasKnife && !isHasKey1) {
                    cout << text << "You took a knife..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                    isHasKnife = true;
                    not_leave = false;
                    cycle1();
                }
                else {
                    cout << text << "There seems to be no knife here..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                    not_leave = false;
                    cycle1();
                }
            }
            else if (player_choice == "top" || player_choice == "open top drawer" || player_choice == "open the top drawer" || player_choice == "open top") {
                not_leave = false;
                closet();
            }
            else if (player_choice == "leave" || player_choice == "get away from the closet" || player_choice == "leave the closet" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the room..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle1();
            }
        }
    }
}
//побег
void escape() {
    bool did_save = false;
    while (!did_save) {
        cout << text << "\nDo you want to save your progress?" << player;
        getline(cin, player_choice);
        transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
        if (player_choice == "yes" || player_choice == "save") {
            ofstream save_game(save_folder);
            if (save_game.is_open()) {
                ostringstream data;
                data << "[DATA]\nsave=" << save_1 << "\nserial=" << serial;
                save_game << data.str();
                save_game.close();
                cout << text << "SUCCESSFULLY SAVED\n";
            }
            else
                cout << text << "SAVE ERROR\n";
            did_save = true;
        }
        else if (player_choice == "no") {
            did_save = true;
        }
    }
    cout << text << "\n\n                  |=======================|\n"
                      "                  |    Chapter 2. Yard    |\n"
                      "                  |=======================|\n\n"
        "When you opened the door, a stream of fresh night wind immediately\n"
        "flew into your face, and the dim night light filled the room...\n"
        "Convinced that everything was safe, you went outside...\n"
        "It was a wonderful cloudless night, wich made the whole courtyard\n"
        "perfectly visible...\n"
        "Your attention was immediately attracted a large mansion in the\n"
        "center of the courtyard...\nLooking closer, you find: Mansion, Gate, Tree house, Barn" << player;
    getline(cin, player_choice);
    transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
    yard();
}

//Глава 2
//
//цикл двора 
void cycle2() {
    string inventory;
    if (isHasCrowbar)
        inventory += "\nYou have: Crowbar...";
    if (isHasMaul)
        inventory += "\nYou have: Maul...";
    cout << text << "You are standing in the middle of the courtyard...\nMansion, Gate, Tree house, Barn" << inventory << player;
    getline(cin, player_choice);
    transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
    yard();
}
//двор
void yard() {
    bool not_leave = true;
    if (player_choice == "mansion" || player_choice == "go to the mansion" || player_choice == "go to mansion" || player_choice == "walk up to the mansion" || player_choice == "approach the mansion" || player_choice == "walk up to mansion" || player_choice == "approach mansion") {
        bool mansion_first = true;
        while (not_leave) {
            if (mansion_first) {
                cout << text << "You have reached the main entrance to the mansion...\nYou saw a huge majestic wooden gate with metal edging..." << player;
                mansion_first = false;
            }
            else
                cout << text << "You are standing next to the main entrance to the mansion...\nYou see huge majestic wooden gates with metal edging..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "open" || player_choice == "open the lock" || player_choice == "open the door" || player_choice == "try to open the door" || player_choice == "try to open") {
                if (isHasMaul) {
                    not_leave = false;
                    enter_mansion();
                }
                else if (isHasCrowbar) {
                    cout << text << "You tried to open the door with a pry bar...\nIt didn't work..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else {
                    cout << text << "You tried to open the doors...\nNothing happened..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "inspect" || player_choice == "inspect the door" || player_choice == "examine the door" || player_choice == "inspect the lock" || player_choice == "examine the lock") {
                if (isHasMaul)
                    cout << text << "You examined the door of the mansion...\nIt seems to be closed on the other side...\nMaybe we should try to break down the door with a sledgehammer..." << player << "Press any key to continue...";
                else
                    cout << text << "You examined the door of the mansion...\nIt seems to be closed on the other side..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
            }
            else if (player_choice == "leave" || player_choice == "get away from the door" || player_choice == "leave the door" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the courtyard..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle2();
            }
        }
    }
    else if (player_choice == "gate" || player_choice == "go to the gate" || player_choice == "go to gate" || player_choice == "walk up to the gate" || player_choice == "approach the gate" || player_choice == "walk up to gate" || player_choice == "approach gate") {
        bool gate_first = true;
        while (not_leave) {
            if (gate_first) {
                cout << text << "You have come to a large iron gate with numerous designs...\n"
                    "A bulky old lock hangs on them...\n"
                    "It seems that it can no longer be opened..." << player;
                gate_first = false;
            }
            else
                cout << text << "You are standing next to a large iron gate with numerous patterns...\n"
                "A bulky old lock hangs on them...\n"
                "It seems that it can no longer be opened..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "open" || player_choice == "open the lock" || player_choice == "open the gate" || player_choice == "try to open the gate" || player_choice == "try to open" || player_choice == "try to open the lock" || player_choice == "try to break the lock" || player_choice == "try to break lock") {
                if (isHasMaul) {
                    cout << text << "You swung your arm around and hit the lock with a sledgehammer...\nThe lock was not damaged... But your hands were very sore..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else if (isHasCrowbar) {
                    if (!first_try_crowbar) {
                        cout << text << "You tried to break the lock with a pry bar...\nYou slightly bent the pry bar and scratched the lock...\nNothing else happened..." << player << "Press any key to continue...";
                        first_try_crowbar = true;
                    }
                    else
                        cout << text << "It's better not to repeat this again...\nThis could break the pry bar..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else {
                    cout << text << "You tried to open the lock...\nIt seems really impossible..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "inspect" || player_choice == "inspect the gate" || player_choice == "examine the gate" || player_choice == "inspect the lock" || player_choice == "examine the lock") {
                if (!first_try_crowbar)
                    cout << text << "You examined the old lock hanging on the gate...\nIt has long since rusted, but is still very strong..." << player << "Press any key to continue...";
                else
                    cout << text << "You examined the old lock hanging on the gate...\nA large scratch stretched from top to bottom...\nIt has long since rusted, but is still very strong..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
            }
            else if (player_choice == "leave" || player_choice == "get away from the gate" || player_choice == "leave the gate" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the courtyard..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle2();
            }
        }
    }
    else if (player_choice == "tree house" || player_choice == "go to the tree house" || player_choice == "go to tree house" || player_choice == "walk up to the tree house" || player_choice == "approach the tree house" || player_choice == "walk up to tree house" || player_choice == "approach tree house" || player_choice == "Sjiwe29z02kd___2Lonewolf239") {
        bool treehouse_first = true;
        if (player_choice == "Sjiwe29z02kd___2Lonewolf239")
            treehouse_first = false;
        while (not_leave) {
            if (treehouse_first) {
                treehouse_first = false;
                cout << text << "You come to a tree house...\nOld rotten boards nailed to a tree led into it..." << player;
            }
            else {
                if (!on_theehouse)
                    cout << text << "You are standing near a tree house...\nOld rotten boards nailed to a tree led into it..." << player;
                else {
                    if (!tear_off)
                        cout << text << "You are standing on the balcony of a tree house...\nThe entrance is boarded up..." << player;
                    else
                        cout << text << "You are standing on the balcony of a tree house...\nNearby there are boards torn from the door..." << player;
                }
            }
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "climb up" || player_choice == "go up" || player_choice == "climb to the tree house" || player_choice == "climb into a tree house" || player_choice == "climb to tree house" || player_choice == "climb into tree house" || player_choice == "go upstairs") {
                if (!on_theehouse) {
                    on_theehouse = true;
                    if (!boards_fell_off) {
                        cout << text << "You approached the stairs and carefully climbed the stairs...\n"
                            "While you were climbing, one of the boards fell off...\n"
                            "Fortunately, you did not fall from the tree and were able to climb up..." << player << "Press any key to continue...";
                        boards_fell_off = true;
                    }
                    else
                        cout << text << "You approached the stairs and carefully climbed the stairs..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else {
                    cout << text << "It seems like you already climbed a tree house..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "enter" || player_choice == "enter the house" || player_choice == "enter house" || player_choice == "enter the home" || player_choice == "enter home" || player_choice == "enter the tree house" || player_choice == "enter tree house" || player_choice == "enter in") {
                if (tear_off)
                    treehouse();
                else {
                    cout << text << "You can't enter through a boarded door...\n"
                        "Or maybe you can still walk through walls?.." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "tear off" || player_choice == "rip off" || player_choice == "tear off the boards" || player_choice == "tear off boards" || player_choice == "tear the boards off" || player_choice == "tear boards off" || player_choice == "try to tear the boards off" || player_choice == "try to tear off the boards" || player_choice == "try to tear boards off" || player_choice == "try to tear off boards") {
                if (tear_off) {
                    cout << text << "It looks like the boards have already been torn off..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else if (isHasCrowbar)
                    treehouse();
                else {
                    cout << text << "You can't do this with your bare hands...\n"
                        "Need to find something to do this..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "inspect" || player_choice == "explore the surroundings" || player_choice == "explore the area" || player_choice == "look around" || player_choice == "look around the area" || player_choice == "look around area") {
                if (on_theehouse) {
                    if (isHasMaul) {
                        if (!first_tree_inspect) {
                            first_tree_inspect = true;
                            cout << text << "You entered the house and examined it again...\n"
                                "But there was nothing in any of the boxes except garbage...\n"
                                "Realizing that you would not find anything useful...\n"
                                "You went out onto the balcony..." << player << "Press any key to continue...";
                        }
                        else
                            cout << text << "Nothing new has definitely appeared since your last round...\n"
                            "There's no point in looking for something that doesn't exist..." << player << "Press any key to continue...";
                    }
                    else if (tear_off)
                        cout << text << "You walked along the balcony and looked out the window...\n"
                        "Inside you can see a table and a couple of lying next boxes to it...\n"
                        "Next to the door lay several boards, as well as two halves of a crowbar..." << player << "Press any key to continue...";
                    else if (isHasCrowbar)
                        cout << text << "You walked along the balcony and looked out the window...\n"
                                        "Inside you can see a table and a couple of boxes lying next to it...\n"
                                        "The door is boarded up with several boards...\n"
                                        "Maybe you should try tearing off the boards with a pry bar..." << player << "Press any key to continue...";
                    else
                        cout << text << "You walked along the balcony and looked out the window...\n"
                                        "Inside you can see a table and a couple of boxes lying next to it...\n"
                                        "The door is boarded up with several boards...\n"
                                        "You need to find something to remove them..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else {
                    if (boards_fell_off)
                        cout << text << "You walked around the tree, inspecting the surroundings...\nNext to the tree, there was a piece of rotten board...\n"
                                        "And it was because of it that you almost fell off the tree..." << player << "Press any key to continue...";
                    else
                        cout << text << "You walked around the tree examining the surroundings...\nYou didn't find anything interesting..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "go down" || player_choice == "go downstairs" || player_choice == "come down" || player_choice == "climb down from the tree house" || player_choice == "get down from the tree house" || player_choice == "climb down from tree house" || player_choice == "get down from tree house") {
                if (on_theehouse) {
                    on_theehouse = false;
                    cout << text << "You climbed down from the treehouse and stood next to him..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else {
                    cout << text << "It seems like you are already standing on the ground..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
            else if (player_choice == "leave" || player_choice == "get away from the tree house" || player_choice == "leave the tree house" || player_choice == "go away") {
                if (!on_theehouse) {
                    cout << text << "You turned around and went to the center of the courtyard..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                    not_leave = false;
                    on_theehouse = false;
                    cycle2();
                }
                else {
                    on_theehouse = false;
                    cout << text << "You climbed down from the treehouse and stood next to him..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
            }
        }
    }
    else if (player_choice == "barn" || player_choice == "go to the barn" || player_choice == "go to barn" || player_choice == "walk up to the barn" || player_choice == "approach the barn" || player_choice == "walk up to barn" || player_choice == "approach barn") {
        bool barn_first = true;
        while (not_leave) {
            if (barn_first) {
                cout << text << "You come to a barn...\n"
                    "It seems the door is open..." << player;
                barn_first = false;
            }
            else
                cout << text << "You are standing next to a barn...\n"
                "It seems the door is open..." << player;
            getline(cin, player_choice);
            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
            if (player_choice == "enter" || player_choice == "enter the barn" || player_choice == "enter barn" || player_choice == "go into the barn" || player_choice == "go into barn" || player_choice == "enter in") {
                bool leave_check = false, first_text_check = true;
                if (light_on) {
                    while (!leave_check) {
                        if (first_text_check) {
                            cout << text << "You entered the barn...\n"
                                "In the light of the lanterns you can see it much better...\n"
                                "This is a small room with a workbench and a closet against the wall..." << player;
                            first_text_check = false;
                        }
                        else
                            cout << text << "You are standing in the center of the barn...\n"
                            "In the light of the lanterns you can see it much better...\n"
                            "This is a small room with a workbench and a closet against the wall..." << player;
                        getline(cin, player_choice);
                        transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
                        if (player_choice == "check the workbench" || player_choice == "check workbench" || player_choice == "inspect the workbench" || player_choice == "inspect workbench" || player_choice == "examine the workbench" || player_choice == "examine workbench" || player_choice == "workbench") {
                            if (!isHasCrowbar && !tear_off) {
                                cout << text << "You examined the workbench...\n"
                                    "You didn't find anything interesting until you looked underneath...\n"
                                    "Under the workbench lay an open toolbox...\n"
                                    "There was a crowbar in it, whoever he was - now it's yours..." << player << "Press any key to continue...";
                                system("pause >NUL");
                                cout << endl;
                                isHasCrowbar = true;
                            }
                            else {
                                cout << text << "You have examined the workbench...\n"
                                    "Now there is definitely nothing interesting here..." << player << "Press any key to continue...";
                                system("pause >NUL");
                                cout << endl;
                            }
                        }
                        else if (player_choice == "check the closet" || player_choice == "check closet" || player_choice == "inspect the closet" || player_choice == "inspect closet" || player_choice == "examine the closet" || player_choice == "examine closet" || player_choice == "closet") {
                            cout << text << "You tried to open the closet...\n"
                                "It is locked... apparently not fate..." << player << "Press any key to continue...";
                            system("pause >NUL");
                            cout << endl;
                        }
                        else if (player_choice == "leave" || player_choice == "get out of the barn" || player_choice == "leave the barn" || player_choice == "leave barn" || player_choice == "go out") {
                            cout << text << "You turned around and walked out of the barn..." << player << "Press any key to continue...";
                            system("pause >NUL");
                            cout << endl;
                            leave_check = true;
                        }
                    }
                }
                else {
                    cout << text << "You entered a small barn...\n"
                        "Inside the barn it is pitch black, you can see literally nothing...\n"
                        "It's worth finding a light source..." << player;
                    bool leave = false, show_text = false;
                    while (!leave) {
                        if (show_text)
                            cout << text << "You are standing in the center of the barn...\n"
                            "It's dark inside, you can literally see nothing...\n"
                            "It's worth finding a light source..." << player;
                        getline(cin, player_choice);
                        show_text = true;
                        if (player_choice == "leave" || player_choice == "get away from the barn" || player_choice == "leave barn" || player_choice == "go away") {
                            cout << text << "You turned around and walked out of the barn..." << player << "Press any key to continue...";
                            system("pause >NUL");
                            cout << endl;
                            leave = true;
                        }
                        else {
                            transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
                            cout << text << "It's too dark to see anything..." << player << "Press any key to continue...";
                            system("pause >NUL");
                            cout << endl;
                        }
                    }
                }
            }
            else if (player_choice == "inspect" || player_choice == "explore the surroundings" || player_choice == "explore the area" || player_choice == "look around" || player_choice == "look around the area" || player_choice == "look around area") {
                if (light_on) {
                    cout << text << "You walked around the barn...\n"
                        "There is nothing more interesting here..." << player << "Press any key to continue...";
                    system("pause >NUL");
                    cout << endl;
                }
                else {
                    cout << text << "You walked around the barn...\n"
                        "You found a switch behind it...\n"
                        "You didn't find anything else interesting..." << player;
                    bool leave = false, show_text = false;
                    while (!leave) {
                        if (show_text)
                            cout << text << "You are standing next to the switch..." << player;
                        getline(cin, player_choice);
                        transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
                        show_text = true;
                        if (player_choice == "turn on" || player_choice == "turn on the switch" || player_choice == "turn on switch" || player_choice == "turn the breaker on" || player_choice == "turn breaker on" || player_choice == "switch the switch" || player_choice == "switch switch" || player_choice == "switch the breaker" || player_choice == "switch breaker") {
                            if (!light_on) {
                                cout << text << "You turned on the switch...\n"
                                    "Light began to pour in from the window at the back of the barn..." << player << "Press any key to continue...";
                                system("pause >NUL");
                                cout << endl;
                                light_on = true;
                            }
                            else {
                                cout << text << "It seems the switch is already on..." << player << "Press any key to continue...";
                                system("pause >NUL");
                                cout << endl;
                            }
                        }
                        else if (player_choice == "leave" || player_choice == "go away" || player_choice == "return to the entrance" || player_choice == "return to entrance" || player_choice == "back to the entrance" || player_choice == "back to entrance" || player_choice == "come back" || player_choice == "go back" || player_choice == "return back") {
                            cout << text << "You've returned to the barn entrance..." << player << "Press any key to continue...";
                            system("pause >NUL");
                            cout << endl;
                            leave = true;
                        }
                    }
                }
            }
            else if (player_choice == "leave" || player_choice == "get away from the barn" || player_choice == "leave barn" || player_choice == "go away") {
                cout << text << "You turned around and went to the center of the courtyard..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
                not_leave = false;
                cycle2();
            }
        }
    }
    else
        cycle2();
}
//дом на дереве
void treehouse() {
    bool not_leave = true;
    bool treehouse_first_enter = true;
    while (not_leave) {
        if (!tear_off) {
            if (treehouse_first_enter) {
                cout << text << "You tore off the boards with difficulty, but the pry bar broke in two...\n"
                    "Apparently it was too rusty, but luckily you were able to open the door...\n"
                    "Inside the house there is a table and a couple of drawers next to it..." << player;
                treehouse_first_enter = false;
                tear_off = true;
                isHasCrowbar = false;
            }
            else
                cout << text << "You are standing in the center of a treehouse room...\n"
                "There is a table and several drawers next to you..." << player;
        }
        else {
            if (treehouse_first_enter) {
                cout << text << "You have entered the treehouse premises...\n"
                    "There is a table next to the far wall...\n"
                    "Nearby there are several boxes..." << player;
                treehouse_first_enter = false;
            }
            else
                cout << text << "You are standing in the center of a treehouse room...\n"
                "There is a table and several drawers next to you..." << player;
        }
        getline(cin, player_choice);
        transform(player_choice.begin(), player_choice.end(), player_choice.begin(), [](unsigned char c) { return tolower(c); });
        if (player_choice == "table" || player_choice == "desk" || player_choice == "inspect the table" || player_choice == "inspect table" || player_choice == "look around the table" || player_choice == "look around table" || player_choice == "look at the table" || player_choice == "look at table" || player_choice == "check the table" || player_choice == "check table") {
            cout << text << "You walked up to the table and examined it...\n"
                "It was a simple homemade table...\n"
                "It doesn't even have drawers, and it itself is made crookedly..." << player << "Press any key to continue...";
            system("pause >NUL");
            cout << endl;
        }
        else if (player_choice == "boxes" || player_choice == "drawers" || player_choice == "inspect the boxes" || player_choice == "inspect boxes" || player_choice == "examine the boxes" || player_choice == "examine boxes" || player_choice == "check the boxes" || player_choice == "check boxes" || player_choice == "inspect the drawers" || player_choice == "inspect drawers" || player_choice == "examine the drawers" || player_choice == "examine drawers" || player_choice == "check the drawers" || player_choice == "check drawers") {
            if (isHasMaul) {
                cout << text << "A bunch of boxes... but they only contain trash..." << player << "Press any key to continue...";
                system("pause >NUL");
                cout << endl;
            }
            else {
                cout << text << "You started looking through every drawer...\n"
                    "Almost all the boxes contained some kind of construction waste...\n"
                    "But in one of the boxes there was a sledgehammer...\n"
                    "It was quite heavy, but you managed to hold it..." << player << "Press any key to continue...";
                isHasMaul = true;
                system("pause >NUL");
                cout << endl;
            }
        }
        else if (player_choice == "leave" || player_choice == "get away from the tree house" || player_choice == "leave the tree house" || player_choice == "go away") {
            cout << text << "You turned around and walked out of the house onto the balcony..." << player << "Press any key to continue...";
            system("pause >NUL");
            cout << endl;
            not_leave = false;
            player_choice = "Sjiwe29z02kd___2Lonewolf239";
            yard();
        }
    }
}
//вход в особняк
void enter_mansion() {
    cout << text << "End of test version 0.2... Thanks for playing!" << player << "Press any key to continue...";
    system("pause >NUL");
    cout << endl;
    main();
}



/*
    ===============================================================
    |							  План:							  |
    |					✅ - готово  ❎ - не готово				  |
    ===============================================================
    | 1. Добавить всем предметам в подвале Inspect				❎|
    | 2. Проверить работоспособность и наличие багов............❎|
    | 3. Сделать главу 3                            			❎|
    ===============================================================

*/