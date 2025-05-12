#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////


//Bai 3.6: Vi trí
Position::Position(int r = 0, int c = 0) : r(r), c(c) {}
Position::Position(const string& str_pos) {
	string temp = str_pos;
	for (char& ch : temp) {
		if ((ch < '0' || ch > '9') && ch != '-')
			ch = ' ';
	}
	stringstream ss(temp);
	int row, col;
	if (ss >> row >> col) {
		r = row;
		c = col;
	}
	else {
		r = 0;
		c = 0;
	}
}
void Position::setRow(int r) {
	this->r = r;
}
void Position::setCol(int c) {
	this->c = c;
}
int Position::getRow() const {
	return r;
}
int Position::getCol() const {
	return c;
}

string Position::str() const {
	return "(" + to_string(getRow()) + "," + to_string(getCol()) + ")";
}

//Bai 3.1: Don vi quan su
Unit::Unit(int quantity, int weight, const Position & pos)
	:quantity(quantity), weight(weight), pos(pos) {}

Unit:: ~Unit() {}

Position Unit::getCurrentPosition() const {
	return pos;
}

int Unit::getQuantity() const {
	return quantity;
}

void Unit::increaseQuantity(int num) {
	if (num >= 0)
		this->quantity += num;
}

void Unit::setQuantity(int q) {
	this->quantity = q;
}

//Bai 3.2 Phuong tien chien dau
Vehicle::Vehicle(int quantity, int weight, const Position& pos, VehicleType vehicleType)
	:Unit(quantity, weight, pos), vehicleType(vehicleType) {}

int Vehicle::getAttackScore() override {
	return (static_cast<int>(vehicleType) * 304 + quantity * weight) / 30;
}

string Vehicle::str() const override {
	static const string vehicleTypeNames[] = { "TRUCK", "MORTAR", "ANTIAIRCRAFT", "ARMOREDCAR", "APC", "ARTILLERY", "TANK" };
	stringstream ss;
	ss << "Vehicle[vehicleType=" << vehicleTypeNames[static_cast<int>(vehicleType)] << ",quantity=" << quantity
		<< ",weight=" << weight << ",pos=" << pos.str() << "]";
	return ss.str();
}

VehicleType Vehicle::getVehicleType() {
	return vehicleType;
}

bool Vehicle::isVehicleType() const {
	return true;
}

bool Vehicle::isInfantryType() const {
	return false;
}

//Bai 3.3 Luc luong bo binh
bool Infantry::isSquareNum(int n) {
	int sqrtn = (int)sqrt(n);
	if (sqrtn * sqrtn == n) return true;
	return false;
}
Infantry::Infantry(int quantity, int weight, const Position& pos, InfantryType infantryType)
	:Unit(quantity, weight, pos), infantryType(infantryType) {}

int Infantry::getAttackScore() override {
	int score = static_cast<int>(infantryType) * 56 + quantity * weight;
	if (infantryType == SPECIALFORCES && isSquareNum(weight))
		score += 75;

	int personalNum = score + 4;
	while (personalNum >= 10) {
		int sum = 0;
		while (personalNum > 0) {
			sum += personalNum % 10;
			personalNum /= 10;
		}
		personalNum = sum;
	}

	int newQuantity = quantity;
	if (personalNum > 7)
		newQuantity = (int)ceil(quantity * 1.2);
	else if (personalNum < 3)
		newQuantity = (int)floor(quantity * 0.9);

	this->quantity = newQuantity;

	score = static_cast<int>(infantryType) * 56 + newQuantity * weight;
	if (infantryType == SPECIALFORCES && isSquareNum(weight))
		score += 75;

	return score;
}

InfantryType Infantry::getInfantryType () const {
	return infantryType;
}

bool Infantry::isInfantryType() const {
	return true;
}

bool Infantry::isVehicleType() const {
	return false;
}

string Infantry::str() const override {
	static const string infantryTypeNames[] = { "SNIPER", "ANTIAIRCRAFTSQUAD", "MORTARSQUAD", "ENGINEER", "SPECIALFORCES", "REGULARINFANTRY" };
	stringstream ss;
	ss << "Infantry[infantryType=" << infantryTypeNames[static_cast<int>(infantryType)] << ",quantity=" << quantity
		<< ",weight=" << weight << ",pos=" << pos.str() << "]";
	return ss.str();
}

//Bai 3.5 Danh sach cac don vi quan su
UnitList::UnitList(int LF, int EXP) {
	this->head = nullptr;
	int s = LF + EXP;
	this->capacity = isSpecialNum(s) ? 12 : 8;
	this->amount = 0;
}

UnitList::Node* UnitList::MakeNode(Unit* unit) {
	if (!unit) return nullptr;
	Node* newNode = new Node(unit);
	
	return newNode;
}

bool UnitList::isSpecialNum(const int& s) {
	int bases[3] = { 3, 5, 7 }; // dieu kien so nguyen to le be hon 10

	for (int b = 0; b < 3; ++b) {
		int current = s;
		int currentBase = bases[b];
		int maxPow = int(log(s) / log(currentBase)); //bien doi toan hoc de lay so mu lon nhat?

		for (int i = maxPow; i >= 0; --i) {
			int power = (int)pow(currentBase, i);

			if (power <= current)
				current -= power;
		}
		if (current == 0)
			return true;
	}

	return false;
}

bool UnitList::insert(Unit* unit) {
	if (!unit || amount >= capacity) return false;

	Node* current = head;
	while (current != nullptr) {
		if ((unit->isInfantryType() && current->data->isInfantryType() &&
			unit->getInfantryType() == current->data->getInfantryType()) ||
			(unit->isVehicleType() && current->data->isVehicleType() &&
				unit->getVehicleType() == current->data->getVehicleType())) {
			current->data->increaseQuantity(unit->getQuantity());
			return true;
		}
		current = current->next;
	}

	// Them don vi
	Node* newNode = MakeNode(unit);
	if (!newNode) return false;

	if (unit->isInfantryType()) {
		// Chen dau danh sach cho Infantry
		newNode->next = head;
		head = newNode;
	}
	else {
		// Chen cuoi danh sach cho Vehicle
		newNode->next = nullptr;
		if (!head) {
			head = newNode;
		}
		else {
			Node* temp = head;
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = newNode;
		}
	}
	amount++;
	return true;
}

bool UnitList::isContain(VehicleType vehicleType) {
	Node* temp = head;
	while (temp != nullptr) {
		if (temp->data->isVehicleType() && temp->data->getVehicleType() == vehicleType) {
			return true;
		}
		temp = temp->next;
	}
	return false;
}

bool UnitList::isContain(InfantryType infantryType) {
	Node* temp = head;
	while (temp != nullptr) {
		if (temp->data->isInfantryType() && temp->data->getInfantryType() == infantryType) {
			return true;
		}
		temp = temp->next;
	}
	return false;
}

string UnitList::str()const {
	stringstream ss; //luu chuoi str can xuat
	int numOfVeh = 0, numOfInfan = 0;
	string units; //luu chuoi cua cac don vi Unit
	Node* current = head;
	
	while (current) {
		if (!current->data) {
			current = current->next;
			continue;
		}

		if (current->data->isVehicleType())
			numOfVeh++;

		if (current->data->isInfantryType())
			numOfInfan++;

		if (!units.empty())
			units += ",";

		units += current->data->str();
		current = current->next;
	}

	ss << "UnitList[count_vehicle=" << numOfVeh << ";count_infantry=" << numOfInfan << ";" << units << "]";
	return ss.str();
}

Node* UnitList::getHead() {
	return head;
}

// Bai 3.4 Quan doi
Army::Army(Unit** unitArray, int size, string name, BattleField* battleField) {
	this->name = name;
	this->battleField = battleField;

	int sumScoreOfVeh = 0, sumScoreOfInfan = 0;
	for (int i = 0; i < size; ++i) {
		if (unitArray[i]) {
			if (unitArray[i]->isVehicleType())
				sumScoreOfVeh += unitArray[i]->getAttackScore();
			else if (unitArray[i]->isInfantryType())
				sumScoreOfInfan += unitArray[i]->getAttackScore();
		}
	}

	LF = (sumScoreOfVeh >= 1000) ? 1000 : sumScoreOfVeh;
	EXP = (sumScoreOfInfan >= 500) ? 500 : sumScoreOfInfan;

	int sum = LF + EXP;
	int capacity = UnitList::isSpecialNum(sum) ? 12 : 8;

	unitList = new UnitList(capacity);
	for (int i = 0; i < size; ++i) {
		if (unitArray[i])
			unitList->insert(unitArray[i]);
	}
}

Army::~Army() {
	delete unitList;
}

Army::getLF() {
	return LF;
}

Army::getEXP() {
	return EXP;
}

//3.4.1 Quan giai phong
LiberationArmy::LiberationArmy(const Unit** unitArray, int size, string name,
	BattleField* battleField) : Army(unitArray, size, name, battleField) {}

int LiberationArmy::increaseToNearestFibo(int n) {
	if (n <= 0) return 1;

	int fibo1 = 1, fibo2 = 1;
	int temp;
	while (fibo2 <= n) {
		temp = fibo1 + fibo2;
		fibo1 = fibo2;
		fibo2 = temp;
	}
	return fibo2;
}

void LiberationArmy::fight(Army* enemy, bool defense) {
	if (defense) {
		double fightLF = LF * 1.3;
		double fightEXP = EXP * 1.3;

		bool isLFgreater = fightLF >= enemy->getLF();
		bool isEXPgreater = fightEXP >= enemy->getEXP();

		if (isLFgreater && isEXPgreater)  return;

		else if (isLFgreater || isEXPgreater) {
			Node* current = unitList->getHead();
			while (current != pullptr) {
				int q = current->data->getQuantity();
				current->data->setQuantity(ceil(static_cast<int>(q * 0.9)));
				current = current->next;
			}
		}

		else if (!isLFgreater && !isEXPgreater) {
			Node* current = unitList->getHead();
			while (current != nullptr) {
				int q = current->data->getQuantity();
				current->data->setQuantity(increaseToNearestFibo(q));
				current = current->next;
			}
		}
	}

	else {
		double fightLF = 1.5 * LF;
		double fightEXP = 1.5 * EXP;

		Node* current = unitList->getHead();
		vector <Unit*> tempLF; //vehicle
		vector <Unit*> tempEXP; //infantry

		while (current != nullptr) {
			if (current->data->isVehicleType())
				tempLF.pushback(current->data);
			else if (current->data->isInfantryType())
				tempEXP.pushback(current->data);

			current = current->next;
		}

		int enemyLF = enemy->getLF();
		int enemyEXP = enemy->getEXP();


	}
}

//3.7 Cac yeu to dia hinh cua tran dia


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////