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

double Position::distance(Position a, Position b) {
	return sqrt((a.r - b.r) * (a.r - b.r) + (a.c - b.c)*(a.c - b.c));
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

void Unit::setWeight(int w) {
	this->weight = w;
}

bool Unit::isDestroyed()const {
	return (weight < = 0 || quantity <= 0);
}

void Unit::destroy() {
	this->weight = 0;
	this->quantity = 0;
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

void UnitList::setHead(Node* newHead) {
	this->head = newHead;
}

bool UnitList::isUnitExist(Unit* unit) {
	Node* temp = head;
	while (temp != nullptr) {
		if (temp->data == unit) return true;
		temp->temp->next;
	}
	return false;
}

void UnitList::remove(Unit* unit) {
	if (!head) return;
	if (head->data == unit) {
		Unit* temp = head;
		head = head->next;
		delete temp;
		return;
	}
}

void UnitList::add(Unit* unit) {
	Node* newNode = new Node(unit);
	newNode->next = head;
	head = newNode;
}

// Bai 3.4 Quan doi
Army::Army(Unit** unitArray, int size, string name, BattleField* battleField) {
	this->name = name;
	this->battleField = battleField;

	updateState(unitArray, size);
}

void Army::updateState() {
	int sumScoreOfVeh = 0, sumScoreOfInfan = 0;

	Node* current = unitList->getHead();
	while (current != nullptr) {
		if (current->data->isVehicleType())
			sumScoreOfVeh += current->data->getAttackScore();
		else if (current->data->isInfantryType())
			sumScoreOfInfan += current->data->getAttackScore();
		current = current->next;
	}

	LF = std::min(1000, sumScoreOfVeh);
	EXP = std::min(500, sumScoreOfInfan);
}


void Army::updateState(Unit** unitArray, int size) {
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

void Army::setLF(int LF) {
	this->LF = LF;
}

void Army::setEXP(int EXP) {
	this->EXP = EXP;
}

int Army::getLF() {
	return LF;
}

int Army::getEXP() {
	return EXP;
}

Unitlist* Army::getUnitlist() const {
	return unitList;
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
	bool win, war;
	if (!enemy || !unitList || !enemy->unitList) {
		win = false;
		war = false;
		return;
	}

	if (defense) {
		double fightLF = LF * 1.3 * 1.0;
		double fightEXP = EXP * 1.3 * 1.0;

		bool isLFgreater = fightLF >= enemy->getLF();
		bool isEXPgreater = fightEXP >= enemy->getEXP();

		if (isLFgreater && isEXPgreater) {
			win = true;
			return;
		}

		else if (isLFgreater || isEXPgreater) {
			win = false;
			war = true;
			Node* current = unitList->getHead();
			while (current != nullptr) {
				int q = current->data->getQuantity();
				current->data->setQuantity(ceil(static_cast<int>(q * 0.9)));
				current = current->next;
			}
		}

		else if (!isLFgreater && !isEXPgreater) {
			win = false;
			war = true;
			Node* current = unitList->getHead();
			while (current != nullptr) {
				int q = current->data->getQuantity();
				current->data->setQuantity(increaseToNearestFibo(q));
				current = current->next;
			}
		}
		this->updateLFandEXP();
	}

	else {
		double fightLF = 1.5 * LF * 1.0;
		double fightEXP = 1.5 * EXP * 1.0;

		Node* current = unitList->getHead();
		vector <Unit*> tempLF; //vehicle
		vector <Unit*> tempEXP; //infantry

		while (current != nullptr) {
			if (current->data->isVehicleType())
				tempLF.push_back(current->data);
			else if (current->data->isInfantryType())
				tempEXP.push_back(current->data);

			current = current->next;
		}

		int enemyLF = enemy->getLF();
		int enemyEXP = enemy->getEXP();

		vector<Unit*> subsetA = findSubset(tempEXP, enemyEXP);
		vector<Unit*> subsetB = findSubset(tempLF, enemyLF);

		if (!subsetA.empty() && !subsetB.empty()) {
			win = true;
			war = true;
			for (Unit* unit : subsetA) {
				unitList->remove(unit);
			}
			for (Unit* unit : subsetB) {
				unitList->remove(unit);
			}
			current = enemy->unitList->getHead();
			while (current != nullptr) {
				if (!current->data->isDestroyed()) {
					mergeUnits(current->data);
				}
				current->data->destroy();
				current = current->next;
			}
			updateLFandEXP();
		}
		else if (!subsetA.empty() || !subsetB.empty()) {
			if (!subsetA.empty()) {
				double totalVehiclePower = calculateTotalPower(tempLF);
				if (totalVehiclePower > enemyLF) {
					win = true;
					war = true;
					for (Unit* unit : subsetA) {
						unitList->remove(unit);
					}
					for (Unit* unit : tempLF) {
						unitList->remove(unit);
					}
					current = enemy->unitList->getHead();
					while (current != nullptr) {
						if (!current->data->isDestroyed()) {
							mergeUnits(current->data);
						}
						current->data->destroy();
						current = current->next;
					}
					updateLFandEXP();
				}
				else {
					win = false;
					war = false;
					current = unitList->getHead();
					while (current != nullptr) {
						if (!current->data->isDestroyed()) {
							int w = current->data->getAttackScore();
							current->data->setWeight(static_cast<int>(w * 0.9));
						}
						current = current->next;
					}
					updateLFandEXP();
				}
			}
			else {
				double totalInfantryPower = calculateTotalPower(tempEXP);
				if (totalInfantryPower > enemyEXP) {
					win = true;
					war = true;
					for (Unit* unit : subsetB) {
						unitList->remove(unit);
					}
					for (Unit* unit : tempEXP) {
						unitList->remove(unit);
					}
					current = enemy->unitList->getHead();
					while (current != nullptr) {
						if (!current->data->isDestroyed()) {
							mergeUnits(current->data);
						}
						current->data->destroy();
						current = current->next;
					}
					updateLFandEXP();
				}
				else {
					win = false;
					war = false;
					current = unitList->getHead();
					while (current != nullptr) {
						if (!current->data->isDestroyed()) {
							int w = current->data->getAttackScore();
							current->data->setWeight(static_cast<int>(w * 0.9));
						}
						current = current->next;
					}
					updateLFandEXP();
				}
			}
		}
		else {
			win = false;
			war = false;
			current = unitList->getHead();
			while (current != nullptr) {
				if (!current->data->isDestroyed()) {
					int w = current->data->getAttackScore();
					current->data->setWeight(static_cast<int>(w * 0.9));
				}
				current = current->next;
			}
			updateLFandEXP();
		}
	}
}

bool LiberationArmy::isLiberationArmy() const {
	return true
}

bool LiberationArmy::isARVN() const {
	return false;
}

string LiberationArmy::str() const{
	string LF = to_string(this->LF);
	string EXP = to_string(this->EXP);
	return "LiberationArmy[LF=" + LF + ",EXP=" + EXP +
		",unitList=" + ((this->unitList != nullptr) ? this->unitList->str() : "UnitList[count_vehicle=0;count_infantry=0]") +
		",battleField=" + ((this->battleField != nullptr) ? this->battleField->str() : "]");
}

vector<Unit*> LiberationArmy::findSubset(const vector<Unit*>& units, int target) {
	vector<Unit*> bestSubset;
	int minSum = INT_MAX;
	int n = units.size();

	for (int mask = 1; mask < (1 << n); mask++) {
		vector<Unit*> subset;
		int sum = 0;
		for (int i = 0; i < n; i++) {
			if (mask & (1 << i)) {
				subset.push_back(units[i]);
				sum += units[i]->getAttackScore();
			}
		}
		if (sum >= target && sum < minSum) {
			minSum = sum;
			bestSubset = subset;
		}
	}
	return bestSubset;
}

double LiberationArmy::calculateTotalPower(const vector<Unit*>& units) {
	double total = 0;
	for (Unit* unit : units) {
		total += unit->getAttackScore();
	}
	return total;
}

void LiberationArmy::mergeUnits(Unit* newUnit) {
	Node* current = unitList->getHead();
	while (current != nullptr) {
		if (current->data->isVehicleType() == newUnit->isVehicleType() &&
			current->data->getAttackScore() == newUnit->getAttackScore()) {
			int newQty = current->data->getQuantity() + newUnit->getQuantity();
			current->data->setQuantity(newQty);
			return;
		}
		current = current->next;
	}
	unitList->insert(newUnit);
}

void LiberationArmy::updateLFandEXP() {
	int sumScoreOfVeh = 0, sumScoreOfInfan = 0;
	Node* current = unitList->getHead();
	while (current != nullptr) {
		if (!current->data->isDestroyed()) {
			if (current->data->isVehicleType()) {
				sumScoreOfVeh += current->data->getAttackScore();
			}
			else if (current->data->isInfantryType()) {
				sumScoreOfInfan += current->data->getAttackScore();
			}
		}
		current = current->next;
	}

	LF = (sumScoreOfVeh >= 1000) ? 1000 : sumScoreOfVeh;
	EXP = (sumScoreOfInfan >= 500) ? 500 : sumScoreOfInfan;
}

//3.4.2 Quan doi Sai Gon
void ARVN::fight(Army* enemy, bool defense) {
	if (!unitList || !enemy) return;

	if (!defense) {
		Node* current = unitList->getHead();
		Node* prev = nullptr;
		while (current != nullptr) {
			int q = current->data->getQuantity();
			current->data->setQuantity(static_cast<int>(ceil(1.0 * 0.8 * q)));
			
			if (current->data->getQuantity() <= 1) {
				Node* erase = current;

				if (!prev) {
					unitList->setHead(current->next);
					current = current->next;
				}
				else {
					prev->next = current->next;
					current = current->next;
				}
				delete erase;
				continue;
			}
			prev = current;
			current = current->next;
		}
		this->updateState();
	}
	
	else if (defense) {
		if (!unitList || !unitList->getHead()) {
			this->setLF(0);
			this->setEXP(0);

			return;
		}
		Node* current = unitList->getHead();
		UnitList* unit = enemy->getUnitlist(); //dsach qsu cua quan giai phong
		if (unit->isUnitExist(current->data)) {
			int q = current->data->getWeight();
			current->data->setWeight(ceil(0.8 * q));
		}
		else {
			Node* erase = current;
			current = current->next;
			delete erase;
			unitList->setHead(current);
		}
		while (current != nullptr && current->next != nullptr) {
			if (!unit->isUnitExist(current->next->data)) {
				//chua ton tai trong qgp
				Node* erase = current->next;
				current->next = current->next->next;
				delete erase;
			}
			else {
				//ton tai trong qgp
				int w = current->next->data->getWeight();
				current->next->data->setWeight(ceil(0.8 * w));
			}
			current = current->next;
		}
		this->updateState();
	}
}

bool ARVN::isLiberationArmy() const {
	return false;
}

bool ARVN::isARVN() const {
	return true;
}

string ARVN::str() const {
	string LF = to_string(this->LF);
	string EXP = to_string(this->EXP);
	return "ARVN[LF=" + LF +
		",EXP=" + EXP +
		",unitList=" + ((this->unitList != nullptr) ? this->unitList->str() : "UnitList[count_vehicle=0;count_infantry=0]") +
		",battleField=" + ((this->battleField != nullptr) ? this->battleField->str() : "]");
}

//3.7 Cac yeu to dia hinh cua tran dia
TerrainElement::TerrainElement() {};
TerrainElement::~TerrainElement() {};

void Road::getEffect(Army* army) const {}

string Road::str() const {
	return "Road";
}

string Mountain::str() const {
	return "Mountain";
}

void Mountain::getEffect(Army* army) const {
	UnitList* units = army->getUnitlist();
	Node* current = units->getHead();
	if (army->isLiberationArmy()) {
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			if (distance(posOfUnit, this->pos) <= 2.0) {
				int x = current->data->getAttackScore();
				if (current->data->isInfantryType()) {
					army->setEXP(army->getEXP() + static_cast<int>(ceil(0.3 * x)));
				}
				else if (current->data->isVehicleType()) {
					army->setLF(army->getLF() - static_cast<int>(ceil(0.1 * x)));
				}
			}
			current = current->next;
		}
	}

	else if (army->isARVN()) {
		current = units->getHead();
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			if (distance(posOfUnit, this->pos) <= 4.0) {
				int x = current->data->getAttackScore();
				if (current->data->isInfantryType()) {
					army->setEXP(army->getEXP() + static_cast<int>(ceil(0.2 * x)));
				}
				else if (current->data->isVehicleType()) {
					army->setLF(army->getLF() - static_cast<int>(ceil(0.05 * x)));
				}
			}
			current = current->next;
		}
	}

}

string River::str() const {
	return "River";
}

void River::getEffect(Army* army) const {
	UnitList* units = army->getUnitlist();
	Node* current = units->getHead();
	while (current != nullptr) {
		Position posOfUnit = current->data->getCurrentPosition();
		if (distance(posOfUnit, this->pos) <= 2.0 && current->data->isInfantryType()) {
			int oldScore = current->data->getAttackScore();
			int newScore = static_cast<int>(ceil(oldScore * 1.0 * 0.9));
			int quantity = current->data->getQuantity();
			if (quantity > 0) {
				int newWeight = ceil(1.0 * (newScore - static_cast<int>(current->data->getInfantryType()) * 56) / quantity);
				current->data->setWeight(newWeight);
			}
		}
		current = current->next;
	}
	army->updateState();
}

string Urban::str() {
	return "Urban";
}

void Urban::getEffect(Army* army) const {
	UnitList* units = army->getUnitlist();
	Node* current = units->getHead();
	if (army->isLiberationArmy()) {
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			InfantryType infantry = current->data->getInfantryType();
			VehicleType vehicle = current->data->getVehicleType();
			double D = distance(posOfUnit, this->pos);
			if (D <= 5.0 && (infantry == SPECIALFORCES || infantry == REGULARINFANTRY) && current->data->isInfantryType()) {
				int currentScore = current->data->getAttackScore();
				int extraScore = static_cast<int>(ceil(1.0 * 2 * currentScore / D));
				int newScore = currentScore + extraScore;
				int quantity = current->data->getQuantity();
				if (quantity > 0) {
					int newWeight = ceil(1.0 * (newScore - ((int)infantry * 56)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			else if (current->data->isVehicleType() && vehicle == ARTILLERY) {
				int currentScore = current->data->getAttackScore();
				int newScore = static_cast<int>(ceil(1.0 * 0.5 * currentScore));
				int quantity = current->data->getQuantity();
				if (quantity > 0) {
					int newWeight = ceil(1.0 * (newScore - ((int)vehicle * 56)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			current = current->next;
		}
	}

	else if (army->isARVN()) {
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			double D = distance(posOfUnit, this->pos);
			InfantryType infantry = current->data->getInfantryType();
			if (current->data->isInfantryType() && infantry == REGULARINFANTRY) {
				int currentScore = current->data->getAttackScore();
				int extraScore = static_cast<int>(ceil((1.0 * 3.0 * currentScore) / (2 * D)));
				int newScore = currentScore + extraScore;
				int quantity = current->data->getQuantity();
				if (quantity > 0) {
					int newWeight = ceil(1.0 * (newScore - ((int)infantry * 56)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			current = current->next;
		}
	}
}

string Fortification::str() const {
	return "Fortification";
}

void Fortification::getEffect(Army* army) const {
	UnitList* units = army->getUnitlist();
	Node* current = units->getHead();
	if (army->isLiberationArmy()) {
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			InfantryType infantry = current->data->getInfantryType();
			VehicleType vehicle = current->data->getVehicleType();
			bool flag = current->data->isInfantryType();
			if (distance(posOfUnit, this->pos) <= 2) {
				int currentScore = current->data->getAttackScore();
				int newScore = static_cast<int>(ceil(currentScore * 1.0 * 0.8));
				int quantity = current->data->getQuantity();
				if (quantity > 0) {
					int newWeight = ceil(1.0 * (newScore - ((int)(flag ? infantry : vehicle) * 56)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			current = current->next;
		}
	}

	else if (army->isARVN()) {
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			InfantryType infantry = current->data->getInfantryType();
			VehicleType vehicle = current->data->getVehicleType();
			bool flag = current->data->isInfantryType();
			if (distance(posOfUnit, this->pos) <= 2) {
				int currentScore = current->data->getAttackScore();
				int newScore = static_cast<int>(ceil(currentScore * 1.0 * 1.2));
				int quantity = current->data->getQuantity();
				if (quantity > 0) {
					int newWeight = ceil(1.0 * (newScore - ((int)(flag ? infantry : vehicle) * 56)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			current = current->next;
		}
	}
}

string SpecialZone::str()const {
	return "SpecialZone";
}

void SpecialZone::getEffect(Army* army) {
	UnitList* units = army->getUnitlist();
	Node* current = units->getHead();
	while (current != nullptr) {
		Position posOfUnit = current->data->getCurrentPosition();
		if (distance(posOfUnit, this->pos) <= 1.0) {
			int newScore = 0;
			current->data->setWeight(0);
		}
		current = current->next;
	}
}

//3.8 Tran dia
BattleField::BattleField(int n_rows, int n_cols, vector<Position*> arrayForest,
	vector<Position*> arrayRiver, vector<Position*> arrayFortification,
	vector<Position*> arrayUrban, vector<Position*> arraySpecialZone) {
	this->n_rows = n_rows;
	this->n_cols = n_cols;

	terrain.resize(n_rows, vector<TerrainElement*>(n_cols, nullptr));
	for (int i = 0; i < n_rows; ++i) {
		for (int j = 0; j < n_cols; ++j) {
			terrain[i][j] = new Road();
		}
	}

	for (Position* pos : arrayForest) {
		if (pos != nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new Mountain();
		}
	}

	for (Position* pos : arrayRiver) {
		if (pos != nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new River();
		}
	}

	for (Position* pos : arrayFortification) {
		if (pos != nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new Fortification();
		}
	}

	for (Position* pos : arrayUrban) {
		if (pos != nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new Urban();
		}
	}

	for (Position* pos : arraySpecialZone) {
		if (pos != nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new SpecialZone();
		}
	}
}

BattleField::~BattleField() {
	for (int i = 0; i < n_rows; ++i) {
		for (int j = 0; j < n_cols; ++j) {
			delete terrain[i][j];
		}
		delete[] terrain[i];
	}
	delete[] terrain;
}

string BattleField::str() const {
	stringstream ss;
	ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
	return ss.str();
}

//3.9 Thiet lap
Configuration::Configuration(const string& filepath) {
	num_rows = 0;
	num_cols = 0;
	eventCode = 0;

	ifstream file(filepath);

	if (!file.is_open())  return;

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		//tach key va value
		size_t index = line.find('='); //vi tri index cua dau =
		if (index == string::npos) continue;
		string key = line.substr(0, index);
		string val = line.substr(index + 1);

		try {
			if (key == "NUM_ROWS")	num_rows = stoi(val);

			else if (key == "NUM_COLS")	num_cols = stoi(val);

			else if (key == "EVENT_CODE")	eventCode = stoi(val);

			else if (key == "ARRAY_FOREST" || key == "ARRAY_RIVER" ||
					 key == "ARRAY_FORTIFICATION" || key == "ARRAY_URBAN" ||
					 key == "ARRAY_SPECIAL_ZONE") {
				stringstream ss(val);
				string pos_str;
				while (getline(ss, pos_str, ',')) {
					size_t colon = pos_str.find(':');
					if (colon == string::npos) continue;
					int r = stoi(pos_str.substr(0, colon));
					int c = stoi(pos_str.substr(colon + 1));

					Position* pos = new Position(r, c);
					if (key == "ARRAY_FOREST") arrayForest.push_back(pos);
					else if (key == "ARRAY_RIVER") arrayRiver.push_back(pos);
					else if (key == "ARRAY_FORTIFICATION") arrayFortification.push_back(pos);
					else if (key == "ARRAY_URBAN") arrayUrban.push_back(pos);
					else if (key == "ARRAY_SPECIAL_ZONE") arraySpecialZone.push_back(pos);
				}
			}
			else if (key == "UNIT_LIST") {
				vector<string>parts;
				stringstream ss(val);
				string part;
				while (getline(ss, part, '_')) {
					parts.push_back(part);
				}
				if (parts.size() != 6) continue;

				//tach cac phan tu side, type, name, position, lf, exp
				string side = parts[0].substr(parts[0].find('(') + 1, parts[0].find(')') - parts[0].find('(') - 1);
				string type = parts[1].substr(parts[1].find('(') + 1, parts[1].find(')') - parts[1].find('(') - 1);
				string name = parts[2].substr(parts[2].find('(') + 1, parts[2].find(')') - parts[2].find('(') - 1);
				string pos_str = parts[3].substr(parts[3].find('(') + 1, parts[3].find(')') - parts[3].find('(') - 1);
				int lf = stoi(parts[4].substr(parts[4].find('(') + 1, parts[4].find(')') - parts[4].find('(') - 1));
				int exp = stoi(parts[5].substr(parts[5].find('(') + 1, parts[5].find(')') - parts[5].find('(') - 1));

				//tach toa do tu position
				size_t colon = pos_str.find(':');
				if (colon == string::npos) continue;
				int r = stoi(pos_str.substr(0, colon));
				int c = stoi(pos_str.substr(colon + 1));
				Position* pos = new Position(r, c);

				//tao unit
				Unit* unit = nullptr;
				if (type == "VEHICLE")
					unit = new Vehicle(name, pos, lf, exp);
				else if (type == "INFANTRY")
					unit = new Infantry(name, pos, lf, exp);
				else {
					delete pos; //giai phong pos neu type khong hop le
					continue;
				}

				//them vao vector tuong ung
				if (side == "LIBERATION") liberationUnits.push_back(unit);
				else if (side == "ARVN") ARVNUnits.push_back(unit);
				else {
					delete unit; // giai phong unit neu side khong hop le
					delete pos;
				}
			}
		}
		catch (const std::exception& e) {
			continue;
		}
	}
	file.close();
}

Configuration::~Configuration() {
	for (Position* pos : arrayForest) {
		delete pos;
	}

	for (Position* pos : arrayRiver) {
		delete pos;
	}

	for (Position* pos : arrayFortification) {
		delete pos;
	}

	for (Position* pos : arrayUrban) {
		delete pos;
	}

	for (Position* pos : arraySpecialZone) {
		delete pos;
	}

	for (Unit* unit : liberationUnits) {
		delete unit;
	}

	for (Unit* unit : ARVNUnits) {
		delete unit;
	}
}

string Configuration::str() const {
	stringstream ss;
	ss << "Configuration[";

	//num row
	ss << "num_rows=" << num_rows << ",";

	//num col
	ss << "num_cols=" << num_cols << ",";

	//arrayForest
	ss << "arrayForest=[";
	for (size_t i = 0; i < arrayForest.size(); ++i) {
		if (arrayForest[i]) {
			ss << arrayForest[i]->getRow() << ":" << arrayForest[i]->getCol();
		}
		else {
			ss << "null";
		}
		if (i < arrayForest.size() - 1) {
			ss << ",";
		}
	}
	ss << "]";

	//arrayRiver
	ss << "arrayRiver=[";
	for (size_t i = 0; i < arrayRiver; ++i) {
		if (arrayRiver[i]) {
			ss << arrayRiver[i]->getRow() << ":" << arrayRiver[i]->getCol();
		}
		else {
			ss << "null";
		}
		if (i < arrayRiver.size() - 1) {
			ss << ",";
		}
	}
	ss << "]";

	//arrayFortification
	ss << "arrayFortification";
	for (size_t i = 0; i < arrayFortification.size(); ++i) {
		if (arrayFortification[i]) {
			ss << arrayFortification[i]->getRow() << ":" << arrayFortification[i]->getCol();
		}
		else {
			ss << "null";
		}
		if (i < arrayFortification.size() - 1) {
			ss << ",";
		}
	}
	ss << "]";

	//arrayUrban
	ss << "arrayUrban";
	for (size_t i = 0; i < arrayUrban.size(); ++i) {
		if (arrayUrban[i]) {
			ss << arrayUrban[i]->getRow() << ":" << arrayUrban[i]->getCol();
		}
		else {
			ss << "null";
		}
		if (i < arrayUrban.size() - 1) {
			ss << ",";
		}
	}
	ss << "]";

	//arraySpecialZone
	ss << "arraySpecialZone";
	for (size_t i = 0; i < arraySpecialZone.size(); ++i) {
		if (arraySpecialZone[i]) {
			ss << arraySpecialZone[i]->getRow() << ":" << arraySpecialZone[i]->getCol();
		}
		else {
			ss << "null";
		}
		if (i < arraySpecialZone.size() - 1) {
			ss << ",";
		}
	}
	ss << "]";

	// liberationUnits
	ss << "liberationUnits=[";
	for (size_t i = 0; i < liberationUnits.size(); ++i) {
		if (liberationUnits[i]) {
			ss << liberationUnits[i]->str();
		}
		else {
			ss << "null";
		}
		if (i < liberationUnits.size() - 1) ss << ",";
	}
	ss << "],";

	// ARVNUnits
	ss << "ARVNUnits=[";
	for (size_t i = 0; i < ARVNUnits.size(); ++i) {
		if (ARVNUnits[i]) {
			ss << ARVNUnits[i]->str();
		}
		else {
			ss << "null";
		}
		if (i < ARVNUnits.size() - 1) ss << ",";
	}
	ss << "],";

	// eventCode
	ss << "eventCode=" << eventCode;

	// Kết thúc chuỗi
	ss << "]";

	return ss.str();
}

//3.10 class HCMcampaign



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
