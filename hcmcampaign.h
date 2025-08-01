/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

 // The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Army {
protected:
    int LF, EXP;
    string name;
    UnitList* unitList;
    BattleField* battleField;

public:
    Army();
    Army(Unit** unitArray, int size, string name, BattleField* battleField);
    virtual void fight(Army* enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    virtual ~Army();
    int getLF();
    int getEXP();
    virtual void setLF(int LF) = 0;
    virtual void setEXP(int EXP) = 0;
    virtual bool isLiberationArmy() const = 0;
    virtual bool isARVN() const = 0;
    virtual UnitList* getUnitlist() const = 0;
    void updateState(Unit** unitArray, int size);
    void updateState();

};

class LiberationArmy : public Army {
public:
    LiberationArmy(Unit** unitArray, int size, string name, BattleField* battleField);
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;
    int increaseToNearestFibo(int n);
    bool isLiberationArmy() const override;
    bool isARVN() const override;
    vector<Unit*> findSubset(const vector<Unit*>& units, int target);
    double calculateTotalPower(const vector<Unit*>& units);
    void mergeUnits(Unit* newUnit);
    void updateLFandEXP();
    void setLF(int LF) override;
    void setEXP(int EXP) override;
    UnitList* getUnitlist() const override;
};

class ARVN : public Army {
public:
    ARVN(Unit** unitArray, int size, string name, BattleField* battleField);
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;
    bool isLiberationArmy() const override;
    bool isARVN() const override;
    void setLF(int LF) override;
    void setEXP(int EXP) override;
    UnitList* getUnitlist() const override;
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string& str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
    static double distance(const Position& a, const Position& b);
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;

public:
    Unit(int quantity, int weight, const Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    virtual VehicleType getVehicleType() const { return TRUCK; }
    virtual InfantryType getInfantryType() const { return SNIPER; }
    virtual bool isInfantryType() const = 0;
    virtual bool isVehicleType() const = 0;
    virtual int getQuantity() const = 0;
    virtual int getWeight() const = 0;
    virtual void increaseQuantity(int num) = 0;
    virtual void setQuantity(int q);
    virtual void setWeight(int w) = 0;
    virtual bool isDestroyed() const;
    virtual void destroy();
    virtual Unit* clone() const = 0;
};

struct Node {
    Unit* data;
    Node* next;
    // Node(Unit* u) : data(u), next(nullptr) {};
};

class UnitList
{
private:
    int capacity;
    // TODO
    Node* head;
    int amount;
public:
    UnitList(int capacity);
    ~UnitList();
    bool insert(Unit* unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO
    Node* MakeNode(Unit* unit);
    static bool isSpecialNum(const int& s);
    Node* getHead();
    void setHead(Node* newHead);
    bool isUnitExist(Unit* unit);
    void remove(Unit* unit);
    void add(Unit* unit);
    void set_capacity(int x);
};

class TerrainElement
{
public:
    TerrainElement();
    virtual ~TerrainElement();
    virtual void getEffect(Army* army) const = 0;
    virtual string str() const = 0;
};

class Road : public TerrainElement {
private:
    Position pos;
public:
    void getEffect(Army* army) const override;
    string str() const override;
};

class Mountain : public TerrainElement {
private:
    Position pos;
public:
    void getEffect(Army* army) const override;
    string str() const override;
};

class River : public TerrainElement {
private:
    Position pos;
public:
    void getEffect(Army* army) const override;
    string str() const override;
};

class Urban : public TerrainElement {
private:
    Position pos;
public:
    void getEffect(Army* army) const override;
    string str() const override;
};

class Fortification : public TerrainElement {
private:
    Position pos;
public:
    void getEffect(Army* army) const override;
    string str() const override;
};

class SpecialZone : public TerrainElement {
private:
    Position pos;
public:
    void getEffect(Army* army) const override;
    string str() const override;
};

class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
    vector<vector<TerrainElement*>> terrain;
public:
    BattleField(int n_rows, int n_cols, vector<Position*> arrayForest,
        vector<Position*> arrayRiver, vector<Position*> arrayFortification,
        vector<Position*> arrayUrban, vector<Position*> arraySpecialZone);
    ~BattleField();
    string str() const;
    vector<vector<TerrainElement*>>& getTerrain();
    const vector<vector<TerrainElement*>>& getTerrain() const;
    int get_n_rows() const;
    int get_n_cols() const;
};

class HCMCampaign
{
private:
    Configuration* config;
    BattleField* battleField;
    LiberationArmy* liberationArmy;
    ARVN* arvn;

public:
    HCMCampaign(const string& config_file_path);
    void run();
    string printResult() const;
};

class Vehicle : public Unit {
private:
    VehicleType vehicleType;
public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);
    int getAttackScore() override;
    string str() const override;
    VehicleType getVehicleType() const override;
    bool isVehicleType() const override;
    bool isInfantryType() const override;
    int getQuantity() const override;
    int getWeight() const override;
    void increaseQuantity(int num) override;
    void setWeight(int w) override;
    Unit* clone() const override;
};

class Infantry : public Unit {
private:
    InfantryType infantryType;
public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);
    int getAttackScore() override;
    string str() const override;
    InfantryType getInfantryType() const override;
    bool isVehicleType() const override;
    bool isInfantryType() const override;
    static bool isSquareNum(int n);
    int getQuantity() const override;
    int getWeight() const override;
    void increaseQuantity(int num) override;
    void setWeight(int w) override;
    Unit* clone() const override;
};

class Configuration {
private:
    int num_rows, num_cols;
    vector <Position*> arrayForest,
        arrayRiver,
        arrayFortification,
        arrayUrban,
        arraySpecialZone;
    vector<Unit*> liberationUnits;
    vector<Unit*> ARVNUnits;
    int eventCode;
public:
    Configuration(const string& filepath);
    ~Configuration();
    string str() const;
    int getNumRows() const;
    int getNumCols() const;
    int getEventCode() const;
    vector<Position*> getArrayForest() const;
    vector<Position*> getArrayRiver() const;
    vector<Position*> getArrayFortification() const;
    vector<Position*> getArrayUrban() const;
    vector<Position*> getArraySpecialZone() const;
    vector<Unit*> getLiberationUnits() const;
    vector<Unit*> getARVNUnits() const;
};

#endif
