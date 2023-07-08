#include <string>

#ifndef GAME_DEV_C___FUNCTIONS_H
#define GAME_DEV_C___FUNCTIONS_H

using namespace std;

enum class WeaponType;
enum class CellType;

class Weapon;
class Operative {
protected:
    static char Op_ID;
    char char_id;
    Weapon *weapon;
public:
    Operative ();
};

class Creature {

};

string enumToString(WeaponType wt);

class AmmoContainer {
protected:
    WeaponType wt;
    int currentAmmo;
    int maxAmmo;
public:
    explicit AmmoContainer (WeaponType wt_);
};

class AidKit {
protected:
    int rep_hp;
    int weight;
public:
    AidKit();
};

class Weapon : public AmmoContainer {
private:
    int damage;
    int fireTime;
    int reloadTime;
public:
    explicit Weapon(WeaponType wt_);
    void get_info ();
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
    void set_type (CellType cellType);
    void set_operative (Operative *op_);
    void set_creature (Creature *creature_);
    void set_weapon (Weapon *weapon_);
    void set_ammoContainer (AmmoContainer *ammoContainer_);
    void set_aidKit (AidKit *aidKit_);
};

class Level {
private:
    int width;
    int height;
    vector<vector<Cell>> cells;
    vector<Operative *> ops;
    vector<Creature *> creatures;
public:
    explicit Level (string &&file_name);

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
                  Weapon *weapon = nullptr, AmmoContainer *ammoContainer = nullptr, AidKit *aidKit = nullptr);
//    string get_level () {
//        string res;
//        for (auto &v : cells) {
//            for (auto &u : v) {
//
//            }
//        }
//    }
};

#endif