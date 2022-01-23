
#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <string>
#include <clocale>
#include <windows.h>

enum Cell {
    CROSS='X',
    ZERO='0',
    EMPTY='_'
};
enum GameProgress {
IN_PROGRESS,
WON_HUMAN,
WON_AI, 
DRAW};
struct Field {
    Cell** ppField = nullptr; //матрица -наше поле [3]x[3]
    const size_t SIZE=3;
    size_t turn = 0; //чей ход
    std::string sname;
    GameProgress progress = IN_PROGRESS;
    Cell AI; //чем играет машина
    Cell human;//чем играет человек
};

//===========================================================================
struct Coord {
    size_t y;
    size_t x;
};
void clearScr() {
    system("cls");
}
int32_t myrand(int32_t min, int32_t max) {
    const static auto seed = std::chrono:: system_clock::now().time_since_epoch().count();
    static std::mt19937_64 generator(seed);
    std::uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}
//================================================================================
void initField(Field& f) {
    // выделение памяти
    f.ppField = new(std::nothrow) Cell * [f.SIZE];
    for (size_t i = 0; i < f.SIZE; i++) {
        f.ppField[i] = new (std::nothrow) Cell[f.SIZE];
    }
    // инициализация поля
    for (size_t y = 0; y < f.SIZE; y++) {
        for (size_t x = 0; x < f.SIZE; x++) {
            f.ppField[y][x] = EMPTY;
        }
    }
        // кто чем ходит
    if (myrand(0, 1000) > 500) {
        f.human = CROSS;
        f.AI = ZERO;
        f.turn = 0;
    }
    else {
        f.human = ZERO;
        f.AI = CROSS;
        f.turn = 1;
    }
}

//освобождение памяти
void deinitField(Field& f) {
    for (size_t i = 0; i < f.SIZE; i++) {
        delete[] f.ppField[i];
    }
    delete[] f.ppField;
    f.ppField = nullptr;
}

Coord getHumanCoord(const Field& f) {
    Coord c={ 0,0 };
    do {
        std::cout << " Введите координаты клетки по оси Х:";
        std::cin >> c.x;
        if (std::cin.fail()) // если предыдущее извлечение оказалось неудачным,
        {
            std::cout << " Ошибка! Введите корректные координаты!" << std::endl;
            std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
            std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
            continue;
        }
        
        std::cout << " Введите координаты клетки по оси Y:";
        std::cin >> c.y;
        if (std::cin.fail()) // если предыдущее извлечение оказалось неудачным,
        {
            std::cout << " Ошибка! Введите корректные координаты!" << std::endl;
            std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
            std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
            continue;
        }
    } while (c.x==0||c.y==0||c.x>3||c.y>3||f.ppField[c.y-1][c.x-1]!=EMPTY);
    c.x--;
    c.y--;
    return c;
}
GameProgress getWon(const Field& f);
Coord getAICoord(Field& f) {
       
    //PreWIN SIt
    for (size_t y = 0; y < f.SIZE; y++) {
        for (size_t x = 0; x < f.SIZE; x++){
            if (f.ppField[y][x] == EMPTY) {
                f.ppField[y][x] = f.AI;
                if (getWon(f) == WON_AI) {
                    f.ppField[y][x] = EMPTY;
                    return { y,x };
                }
                f.ppField[y][x] = EMPTY;
            }
        }
    }
    //PreFAIL
    for (size_t y = 0; y < f.SIZE; y++) {
        for (size_t x = 0; x < f.SIZE; x++) {
            if (f.ppField[y][x] == EMPTY) {
                f.ppField[y][x] = f.human;
                if (getWon(f) == WON_HUMAN) {
                    f.ppField[y][x] = EMPTY;
                    return { y,x };
                }
                f.ppField[y][x] = EMPTY;
            }
        }
    }
    Coord c;
    //центр
    if (f.ppField[1][1] == EMPTY) {
        return{ 1, 1 };
    }
    //углы
    if (myrand(1, 4) == 1)
    {
        if (f.ppField[0][0] == EMPTY) {
            return{ 0, 0 };
        }
    }
    if (myrand(1, 4) == 2) {
        if (f.ppField[0][2] == EMPTY) {
            return{ 0, 2 };
        }
    }
    if (myrand(1, 4) == 3) {
        if (f.ppField[2][0] == EMPTY) {
            return{ 2, 0 };
        }
    }
    if (myrand(1, 4) == 4) {
        if (f.ppField[2][2] == EMPTY) {
            return{ 2, 2 };
        }
    }
    //не углы
    if (f.ppField[0][1] == EMPTY) {
         return{ 0, 1 };
    }
    if (f.ppField[1][0] == EMPTY) {
        return{ 1, 0 };
    }
    if (f.ppField[2][1] == EMPTY) {
        return{ 2, 1 };
    }
    if (f.ppField[1][2] == EMPTY) {
        return{ 1, 2 };
    }
}

/*bool checkAIPreWin();
bool checkAIPreFail();
bool checkWin();*/

void printField (const Field& f) {
    std::cout << "       ";
    for (size_t x = 0; x < f.SIZE; x++) {
        std::cout << x + 1 << "   ";
    }
    std::cout << " X"<<std::endl;
    for (size_t y = 0; y < f.SIZE; y++) {
        std::cout << "  " << y + 1 << "  | ";
        for (size_t x = 0; x < f.SIZE; x++) {
            std::cout << (char)f.ppField[y][x] << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << "  Y" << std::endl << std::endl;
    std::cout <<" " << f.sname << " :" << (char)f.human << std::endl;
    std::cout << " Компьютер:" << (char)f.AI << std::endl;
    std::cout << " Ход № " << (char)f.turn+1 << std::endl;
}

GameProgress getWon(const Field & f) {
    //для строк
    for (size_t y = 0; y < f.SIZE; y++) {
        if (f.ppField[y][0] == f.ppField[y][1] && f.ppField[y][0] == f.ppField[y][2]) {
            if (f.ppField[y][0] == f.AI) {
                return WON_AI;
            }
            if (f.ppField[y][0] == f.human) {
                return WON_HUMAN;
            }
        }
    }
    for (size_t x = 0; x < f.SIZE; x++) {
        if (f.ppField[0][x] == f.ppField[1][x] && f.ppField[0][x] == f.ppField[2][x]) {
            if (f.ppField[0][x] == f.AI) {
                return WON_AI;
            }
            if (f.ppField[0][x] == f.human) {
                return WON_HUMAN;
            }
        }
    }
    if (f.ppField[0][0] == f.ppField[1][1] && f.ppField[0][0] == f.ppField[2][2]) {
        if (f.ppField[0][0] == f.AI) {
            return WON_AI;
        }
        if (f.ppField[0][0] == f.human) {
            return WON_HUMAN;
        }
    }
    if (f.ppField[0][2] == f.ppField[1][1] && f.ppField[0][2] == f.ppField[2][0]) {
        if (f.ppField[0][2] == f.AI) {
            return WON_AI;
        }
        if (f.ppField[0][2] == f.human) {
            return WON_HUMAN;
        }
    }

    bool draw = true;
    for (size_t y = 0; y < f.SIZE; y++) {
        for (size_t x = 0; x < f.SIZE; x++) {
            if (f.ppField[y][x] == EMPTY) {
                draw = false;
                break;
            }
        }
        if (!draw) {
            break;
        }
    }
    if (draw) {
        return DRAW;
    }
    return IN_PROGRESS;
}//выйграл ли кто-то или игра в прогрессе

std::string getName() {
    std::string name;
    std::cout << " Введите свое имя:";
    getline(std::cin, name);
    if (std::cin.fail()||(name==" ")||(name.find("  ")!=std::string::npos)) // если предыдущее извлечение оказалось неудачным,
    {
        std::cout << " Ошибка! Значит будете Аноним!" << std::endl;
        //std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
        //std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
        name = "Аноним";
        std::cin.get();
    }
    return name;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Field f;
    std::string newraund;
    
    f.sname = getName();
    /*std::cout << std::endl << " Какой размер поля: 3, 4, 5:";
    std::cin >> f.SIZE;*/
    do {
        initField(f);
        clearScr();
        printField(f);
        do {
            if (f.turn % 2 == 0) {
                //ход человека
                Coord c = getHumanCoord(f);
                f.ppField[c.y][c.x] = f.human;
            }
            else {
                Coord c = getAICoord(f);
                f.ppField[c.y][c.x] = f.AI;
            }
            clearScr();
            printField(f);
            f.turn++;
            f.progress = getWon(f);
        } while (f.progress == IN_PROGRESS);
        if (f.progress == WON_HUMAN) {
            std::cout << " Вы победили!" << std::endl;
        }
        if (f.progress == WON_AI) {
            std::cout << " Победил компьютер!" << std::endl;
        }
        if (f.progress == DRAW) {
            std::cout << " Победила дружба!" << std::endl;
        }
        deinitField(f);
        std::cout << "Повторить?" << std::endl;
        std::cin >> newraund;
        while (std::cin.fail()){
            std::cout << " Ошибка!" << std::endl;
            std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
            std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
            std::cout << "Повторить? д/н" << std::endl;
            std::cin >> newraund;
        }
    } while ((newraund.find("y") != std::string::npos) || (newraund.find("д") != std::string::npos) || (newraund.find("Y") != std::string::npos) || (newraund.find("Д") != std::string::npos));
}

