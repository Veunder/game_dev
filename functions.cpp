#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "functions.h"

using namespace std;

enum class WeaponType {
    Pistol,
    Rifle,
    Shotgun,
    Sniper
};

enum class CellType {
    Empty,
    Wall,
    Glass,
    Partition,
    AidKit,
    AmmoContainer,
    Weapon,
    Creature,
    Operative
};

Operative::Operative() {
    char_id = Op_ID;
    Op_ID++;
}

char Operative::Op_ID = 'a';

string enumToString(WeaponType wt) {
    switch (wt) {
        case WeaponType::Pistol:
            return "Pistol";
        case WeaponType::Rifle:
            return "Rifle";
        case WeaponType::Shotgun:
            return "Shotgun";
        case WeaponType::Sniper:
            return "Sniper";
        default:
            return "Unknown";
    }
}


AmmoContainer::AmmoContainer(WeaponType wt_) {
    if (wt_ == WeaponType::Pistol) {
        wt = wt_;
        currentAmmo = 90;
        maxAmmo = 90;
        return;
    }
    if (wt_ == WeaponType::Sniper) {
        wt = wt_;
        currentAmmo = 25;
        maxAmmo = 25;
        return;
    }
    if (wt_ == WeaponType::Shotgun) {
        wt = wt_;
        currentAmmo = 75;
        maxAmmo = 75;
        return;
    }
    if (wt_ == WeaponType::Rifle) {
        wt = wt_;
        currentAmmo = 150;
        maxAmmo = 150;
    }
};

class AidKit {
protected:
    int rep_hp;
    int weight;
public:
    AidKit() {
        rep_hp = 100;
        weight = 5;
    }
};

class Weapon : public AmmoContainer {
private:
    int damage;
    int fireTime;
    int reloadTime;
public:
    explicit Weapon(WeaponType wt_) : AmmoContainer(wt_){
        if (wt == WeaponType::Pistol) {
            damage = 15;
            fireTime = 1;
            reloadTime = 2;
            return;
        }
        if (wt == WeaponType::Sniper) {
            damage = 15;
            fireTime = 1;
            reloadTime = 2;
            return;
        }
        if (wt == WeaponType::Shotgun) {
            damage = 15;
            fireTime = 1;
            reloadTime = 2;
            return;
        }
        if (wt == WeaponType::Rifle) {
            damage = 15;
            fireTime = 1;
            reloadTime = 2;
        }
    }
    void get_info () {
        cout << "WeaponType " << enumToString(wt) << endl
             << "currentAmmo " << currentAmmo << endl
             << "maxAmmo " << maxAmmo << endl
             << "damage " << damage << endl
             << "fireTime " << fireTime << endl
             << "reloadTime " << reloadTime << endl;
    }
};

class Cell {
private:
    CellType ct;
    Operative * op;
    Creature * creature;
    Weapon *weapon;
    AmmoContainer *ammoContainer;
    AidKit *aidKit;
public:
    void set_type (CellType cellType) {
        ct = cellType;
    }
    void set_operative (Operative *op_) {
        op = op_;
    }
    void set_creature (Creature *creature_) {
        creature = creature_;
    }
    void set_weapon (Weapon *weapon_) {
        weapon = weapon_;
    }
    void set_ammoContainer (AmmoContainer *ammoContainer_) {
        ammoContainer = ammoContainer_;
    }
    void set_aidKit (AidKit *aidKit_) {
        aidKit = aidKit_;
    }
};

class Level {
private:
    int width;
    int height;
    vector<vector<Cell>> cells;
    vector<Operative *> ops;
    vector<Creature *> creatures;
public:
    explicit Level (string &&file_name) {
        ifstream InputFile(file_name);
        if (InputFile.is_open()) {
            int h, w;
            InputFile >> h >> w;
            if (h < 10 || w < 10) {
                cout << "Слишком маленькие размеры карты, будет создан дефолтный уровень." << endl;
                *this = Level("default_map.txt");
            } else {
                height = h;
                width = w;
                cells.resize(height);
                for (auto &v: cells) {
                    v.resize(width);
                }
                string s;
                for (int i = 0; i < height; ++i) {
                    InputFile >> s;
                    for (int j = 0; j < width; ++j) {
                        this->set_cell(i, j, s[j]);
                    }
                }
            }
            InputFile.close();
        }
        else {
            cout << "Не удалось открыть файл, будет создан дефолтный уровень." << endl;
            *this = Level("default_map.txt");
        }
    }

    [[nodiscard]] int get_width () const { return width; }
    [[nodiscard]] int get_height () const { return height; }

    /*
    1. . - пустая клетка пола;
    2. # - стена;
    3. = - перегородка;
    4. - - стекло;
    5. a, b, c, ..., z - оперативники, подконтрольные пользователю;
    6. A, B, C, ..., Z - противники;
    7. ) - контейнер с патронами;
    8. + - аптечка;
    9. $ - оружие;
    */
    int set_cell (int i, int j, char c, Operative * op = nullptr, Creature * creature = nullptr,
                  Weapon *weapon = nullptr, AmmoContainer *ammoContainer = nullptr, AidKit *aidKit = nullptr) {

        if (c == '#') {
            cells[i][j].set_type(CellType::Wall);
        }
        else if (c == '.') {
            cells[i][j].set_type(CellType::Empty);
        }
        else if (c == '=') {
            cells[i][j].set_type(CellType::Partition);
        }
        else if (c == '-') {
            cells[i][j].set_type(CellType::Glass);
        }
        else if (c >= 'a' && c <= 'z') {
            if (!op) { op = new Operative; }
            cells[i][j].set_type(CellType::Operative);
            cells[i][j].set_operative(op);
            ops.push_back(op);
        }
        else if (c >= 'A' && c <= 'Z') {
            if (!creature) { creature = new Creature; }
            cells[i][j].set_type(CellType::Creature);
            cells[i][j].set_creature(creature);
            creatures.push_back(creature);
        }
        else if (c == ')'){
            if (!ammoContainer) { ammoContainer = new AmmoContainer(WeaponType::Rifle); } // Сделать создание рандомным
            cells[i][j].set_type(CellType::AmmoContainer);
            cells[i][j].set_ammoContainer(ammoContainer);
        }
        else if (c == '+') {
            if (!aidKit) {aidKit = new AidKit; }
            cells[i][j].set_type(CellType::AidKit);
            cells[i][j].set_aidKit(aidKit);
        }
        else if (c == '$') {
            if (!weapon) { weapon = new Weapon(WeaponType::Rifle); }
            cells[i][j].set_type(CellType::Weapon);
            cells[i][j].set_weapon(weapon);
        }
        return 0;
    }
//    string get_level () {
//        string res;
//        for (auto &v : cells) {
//            for (auto &u : v) {
//
//            }
//        }
//    }
};

Level::Level(std::string &&file_name) {
    ifstream InputFile(file_name);
    if (InputFile.is_open()) {
        int h, w;
        InputFile >> h >> w;
        if (h < 10 || w < 10) {
            cout << "Слишком маленькие размеры карты, будет создан дефолтный уровень." << endl;
            *this = Level("default_map.txt");
        } else {
            height = h;
            width = w;
            cells.resize(height);
            for (auto &v: cells) {
                v.resize(width);
            }
            string s;
            for (int i = 0; i < height; ++i) {
                InputFile >> s;
                for (int j = 0; j < width; ++j) {
                    this->set_cell(i, j, s[j]);
                }
            }
        }
        InputFile.close();
    }
    else {
        cout << "Не удалось открыть файл, будет создан дефолтный уровень." << endl;
        *this = Level("default_map.txt");
    }
}

int Level::set_cell(int i, int j, char c, Operative *op, Creature *creature, Weapon *weapon,
                    AmmoContainer *ammoContainer, AidKit *aidKit) {

    if (c == '#') {
        cells[i][j].set_type(CellType::Wall);
    }
    else if (c == '.') {
        cells[i][j].set_type(CellType::Empty);
    }
    else if (c == '=') {
        cells[i][j].set_type(CellType::Partition);
    }
    else if (c == '-') {
        cells[i][j].set_type(CellType::Glass);
    }
    else if (c >= 'a' && c <= 'z') {
        if (!op) { op = new Operative; }
        cells[i][j].set_type(CellType::Operative);
        cells[i][j].set_operative(op);
        ops.push_back(op);
    }
    else if (c >= 'A' && c <= 'Z') {
        if (!creature) { creature = new Creature; }
        cells[i][j].set_type(CellType::Creature);
        cells[i][j].set_creature(creature);
        creatures.push_back(creature);
    }
    else if (c == ')'){
        if (!ammoContainer) { ammoContainer = new AmmoContainer(WeaponType::Rifle); } // Сделать создание рандомным
        cells[i][j].set_type(CellType::AmmoContainer);
        cells[i][j].set_ammoContainer(ammoContainer);
    }
    else if (c == '+') {
        if (!aidKit) {aidKit = new AidKit; }
        cells[i][j].set_type(CellType::AidKit);
        cells[i][j].set_aidKit(aidKit);
    }
    else if (c == '$') {
        if (!weapon) { weapon = new Weapon(WeaponType::Rifle); }
        cells[i][j].set_type(CellType::Weapon);
        cells[i][j].set_weapon(weapon);
    }
    return 0;
}