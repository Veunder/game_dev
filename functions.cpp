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

char Creature::Creature_ID = 'A';

Creature::Creature(Point point) {
    char_id = Creature_ID;
    Creature_ID++;
    pos = point;
}

char Creature::get_char_id() const {
    return char_id;
}

Operative::Operative(Point point1, Point point) : Creature(point1) {
    char_id = Operative_ID;
    Operative_ID++;
    pos = point;
}

char Operative::Operative_ID = 'a';

char Operative::get_char_id() const {
    return char_id;
}

int Creature::move(char key, level &level) {
    if (key == 'u') { //up

    } else if (key == 'd') { // down

    } else if (key == 'r') { // right

    } else if (key == 'l') { // left

    } else { return 1; }
    return 0;
}

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
}

AidKit::AidKit() {
    rep_hp = 100;
    weight = 5;
}

Weapon::Weapon(WeaponType wt_) : AmmoContainer(wt_) {
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

void Weapon::get_info() {
    cout << "WeaponType " << enumToString(wt) << endl
         << "currentAmmo " << currentAmmo << endl
         << "maxAmmo " << maxAmmo << endl
         << "damage " << damage << endl
         << "fireTime " << fireTime << endl
         << "reloadTime " << reloadTime << endl;
}

void Cell::set_type(CellType cellType) {
    ct = cellType;
}

void Cell::set_operative(Operative *op_) {
    op = op_;
}

void Cell::set_creature(Creature *creature_) {
    creature = creature_;
}

void Cell::set_weapon(Weapon *weapon_) {
    weapon = weapon_;
}

void Cell::set_ammoContainer(AmmoContainer *ammoContainer_) {
    ammoContainer = ammoContainer_;
}

void Cell::set_aidKit(AidKit *aidKit_) {
    aidKit = aidKit_;
}

void Cell::set_symbol(char c) {
    symbol = c;
}

char Cell::get_symbol() const {
    return symbol;
}

CellType Cell::get_cell_type() {
    return ct;
}


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
    } else {
        if (file_name == "default_map.txt") {
            cout << "Не удалось найти дефолтный уровень, выход." << endl;
            exit(0);
        }
        cout << "Не удалось открыть файл, будет создан дефолтный уровень." << endl;
        *this = Level("default_map.txt");
    }
}

int Level::set_cell(int i, int j, char c, Operative *op, Creature *creature, Weapon *weapon,
                    AmmoContainer *ammoContainer, AidKit *aidKit) {

    cells[i][j].set_symbol(c);
    if (c == '#') {
        cells[i][j].set_type(CellType::Wall);
    } else if (c == '=') {
        cells[i][j].set_type(CellType::Partition);
    } else if (c == '-') {
        cells[i][j].set_type(CellType::Glass);
    } else if (c >= 'a' && c <= 'z') {
        if (!op) { op = new Operative(Point(), Point()); }
        cells[i][j].set_type(CellType::Operative);
        cells[i][j].set_operative(op);
        ops.push_back(op);
        cells[i][j].set_symbol(op->get_char_id());
    } else if (c >= 'A' && c <= 'Z') {
        if (!creature) { creature = new Creature; }
        cells[i][j].set_type(CellType::Creature);
        cells[i][j].set_creature(creature);
        creatures.push_back(creature);
        cells[i][j].set_symbol(creature->get_char_id());
    } else if (c == ')') {
        if (!ammoContainer) { ammoContainer = new AmmoContainer(WeaponType::Rifle); } // Сделать создание рандомным
        cells[i][j].set_type(CellType::AmmoContainer);
        cells[i][j].set_ammoContainer(ammoContainer);
    } else if (c == '+') {
        if (!aidKit) { aidKit = new AidKit; }
        cells[i][j].set_type(CellType::AidKit);
        cells[i][j].set_aidKit(aidKit);
    } else if (c == '$') {
        if (!weapon) { weapon = new Weapon(WeaponType::Rifle); }
        cells[i][j].set_type(CellType::Weapon);
        cells[i][j].set_weapon(weapon);
    } else {
        cells[i][j].set_type(CellType::Empty);
        cells[i][j].set_symbol('.');
    }
    return 0;
}

void Level::PrintLevel() {
    for (const auto &i: cells) {
        for (auto j: i) {
            cout << j.get_symbol();
        }
        cout << endl;
    }
}

char Level::get_cell_symbol(int x, int y) {
    return (cells[x][y]).get_symbol();
}

CellType Level::get_cell_type(int x, int y) {
    return (cells[x][y]).get_cell_type();
}

Weapon Inventory::get_weapon() {
    for (int i = 0; i < weapons.size(); ++i) {
        cout << i + 1 << '.' << endl;
        weapons[i]->get_info();
    }
    int choice;
    while (!(cin >> choice).good() || choice < 1 || choice > weapons.size()) {
        cout << "Incorrect number!\n";
        cin.clear();
        cin.ignore(100, '\n');
    }
    cin.clear();
    cin.ignore(100, '\n');
    choice--;
    Weapon *return_value = weapons[choice];
    weapons.erase(weapons.cbegin() + choice);
    return *(return_value);
}