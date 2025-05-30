/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Spring 2025
* Date: 02.02.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.

//g++ -o main main.cpp hcmcampaign.cpp -I . -std=c++11

#include "hcmcampaign.h"

using namespace std;

void g_satc_01() {
    cout << "----- Sample Testcase 01 -----" << endl;
    Configuration* config = new Configuration("config.txt");
    cout << config->str() << endl;
    delete config;
}
void g_satc_02() {
    cout << "----- Sample Testcase 02 -----" << endl;
    Vehicle tankOfLiber(5, 2, Position(1, 2), TANK);
    Vehicle truckOfLiber(3, 1, Position(2, 2), TRUCK);
    Infantry sniperOfLiber(5, 2, Position(1, 1), SNIPER);

    Vehicle tankOfAr(5, 2, Position(3, 2), TANK);
    Vehicle truckOfAr(3, 1, Position(3, 1), TRUCK);
    Infantry sniperOfAr(5, 2, Position(3, 3), SNIPER);

    Unit** unitArrayOfLiber = new Unit * [3];
    unitArrayOfLiber[0] = &tankOfLiber;
    unitArrayOfLiber[1] = &truckOfLiber;
    unitArrayOfLiber[2] = &sniperOfLiber;

    Unit** unitArrayOfAr = new Unit * [3];
    unitArrayOfAr[0] = &tankOfAr;
    unitArrayOfAr[1] = &truckOfAr;
    unitArrayOfAr[2] = &sniperOfAr;

    LiberationArmy* liberationArmy = new LiberationArmy(unitArrayOfLiber, 3, "LiberationArmy", 0);
    cout << liberationArmy->str() << endl;
    ARVN* arvn = new ARVN(unitArrayOfAr, 3, "ARVN", 0);
    cout << arvn->str() << endl;

    Army* enemyLiber = arvn;
    liberationArmy->fight(enemyLiber, false);
    Army* enemyAr = liberationArmy;
    arvn->fight(enemyAr, false);

    cout << "* After the fight" << endl;
    cout << liberationArmy->str() << endl;
    cout << arvn->str() << endl;

    delete liberationArmy;
    delete arvn;
    delete[] unitArrayOfLiber;
    delete[] unitArrayOfAr;
}
void  g_satc_03() {
    cout << "----- Sample Testcase 03 -----" << endl;
    HCMCampaign* campaign = new HCMCampaign("config.txt");
    cout << "** Before the fight" << endl;
    cout << campaign->printResult() << endl;
    cout << "** After the fight" << endl;
    campaign->run();
    cout << campaign->printResult() << endl;
    delete campaign;
}

void g_satc_04() {
----- Sample Testcase 04 -----
num_rows = 10, num_cols = 8

arrayForest = [(1, 2), (3, 5)]
arrayRiver = [(0, 0), (0, 4)]
arrayFortification = [(6, 6)]
arrayUrban = [(2, 0)]
arraySpecialZone = [(9, 7)]

liberationUnits = [
    Vehicle[vehicleType=TANK, quantity=5, weight=2, position=(1, 2)],
    Infantry[infantryType=REGULARINFANTRY, quantity=5, weight=2, position=(1, 1)]
]

ARVNUnits = [
    Vehicle[vehicleType=TANK, quantity=5, weight=2, position=(3, 2)],
    Infantry[infantryType=REGULARINFANTRY, quantity=5, weight=2, position=(3, 3)]
]

eventCode = 23
}

void g_satc_05(){
----- Sample Testcase 05 -----
Vehicle[vehicleType=MORTAR,quantity=20,weight=20,position=(1,2)]
}

void g_stac_06(){
----- Sample Testcase 06 -----
LiberationArmy[LF=0,EXP=0,unitList=UnitList[count_vehicle=0;count_infantry=0],battleField=]
}

void g_stac_07(){
----- Sample Testcase 07 -----
LiberationArmy[
    LF=38, EXP=400,
    unitList=UnitList[
        count_vehicle=1; count_infantry=1;
        Infantry[infantryType=SNIPER, quantity=24, weight=20, position=(1, 0)],
        Vehicle[vehicleType=MORTAR, quantity=25, weight=20, position=(1, 2)]
    ],
    battleField=
]
}

int main(int argc, const char * argv[]) {
    g_satc_03();
    return 0;
}
