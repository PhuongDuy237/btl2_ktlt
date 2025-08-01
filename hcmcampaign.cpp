#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////


//Bai 3.6: Vi trí
Position::Position(int r, int c) : r(r), c(c) {}
Position::Position(const string& str_pos) {
	string temp = str_pos;
	for (char& ch : temp) {
		if ((ch < '0' || ch > '9') && ch != '-' && ch != ',')
			ch = ' ';
	}
	for (char& ch : temp) {
		if (ch == ',') ch = ' ';
	}
	stringstream ss(temp);
	int row = 0, col = 0;
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

double Position::distance(const Position& a, const Position& b) {
	return sqrt((a.r - b.r) * (a.r - b.r) + (a.c - b.c) * (a.c - b.c));
}

//Bai 3.1: Don vi quan su
Unit::Unit(int quantity, int weight, const Position pos)
	:quantity(quantity), weight(weight), pos(pos) {}

Unit:: ~Unit() {}

Position Unit::getCurrentPosition() const {
	return pos;
}

int Unit::getQuantity() const {
	return quantity;
}

int Unit::getWeight() const {
	return weight;
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
	return (weight <= 0 || quantity <= 0);
}

void Unit::destroy() {
	this->weight = 0;
	this->quantity = 0;
}

//Bai 3.2 Phuong tien chien dau
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType) :Unit(quantity, weight, pos) {
	this->vehicleType = vehicleType;
}

int Vehicle::getAttackScore() {
	return ceil(1.0 * (static_cast<int>(vehicleType) * 304 + quantity * weight) / 30);
}

string Vehicle::str() const {
	static const string vehicleTypeNames[] = { "TRUCK", "MORTAR", "ANTIAIRCRAFT", "ARMOREDCAR", "APC", "ARTILLERY", "TANK" };
	stringstream ss;
	ss << "Vehicle[vehicleType=" << vehicleTypeNames[static_cast<int>(vehicleType)] << ",quantity=" << quantity
		<< ",weight=" << weight << ",position=" << pos.str() << "]";
	return ss.str();
}

VehicleType Vehicle::getVehicleType() const {
	return vehicleType;
}

bool Vehicle::isVehicleType() const {
	return true;
}

bool Vehicle::isInfantryType() const {
	return false;
}

int Vehicle::getQuantity() const {
	return quantity;
}

int Vehicle::getWeight() const {
	return weight;
}

void Vehicle::increaseQuantity(int num) {
	if (num >= 0) quantity += num;
}

void Vehicle::setWeight(int w) {
	weight = w;
}
Unit* Vehicle::clone() const {
	return new Vehicle(this->quantity, this->weight, this->pos, this->vehicleType);
}

//Bai 3.3 Luc luong bo binh
bool Infantry::isSquareNum(int n) {
	int sqrtn = ceil(sqrt(n));
	if (sqrtn * sqrtn == n) return true;
	return false;
}
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
	:Unit(quantity, weight, pos) {
	this->infantryType = infantryType;
}

int Infantry::getAttackScore() {
	int score = int(infantryType) * 56 + quantity * weight;
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

	if (personalNum > 7) {
		quantity = (int)ceil(1.2 * this->quantity);
	}
	else if (personalNum < 3) {
		quantity = this->quantity - ceil(0.1 * this->quantity);
		if (this->quantity < 0) this->quantity = 0;
	}
	return int(infantryType) * 56 + quantity * weight + ((infantryType == SPECIALFORCES && isSquareNum(weight)) ? 75 : 0);
}

InfantryType Infantry::getInfantryType() const {
	return infantryType;
}

bool Infantry::isInfantryType() const {
	return true;
}

bool Infantry::isVehicleType() const {
	return false;
}

string Infantry::str() const {
	static const string infantryTypeNames[] = { "SNIPER", "ANTIAIRCRAFTSQUAD", "MORTARSQUAD", "ENGINEER", "SPECIALFORCES", "REGULARINFANTRY" };
	stringstream ss;
	ss << "Infantry[infantryType=" << infantryTypeNames[static_cast<int>(infantryType)] << ",quantity=" << quantity
		<< ",weight=" << weight << ",position=" << pos.str() << "]";
	return ss.str();
}

int Infantry::getQuantity() const {
	return quantity;
}

int Infantry::getWeight() const {
	return weight;
}

void Infantry::increaseQuantity(int num) {
	if (num >= 0) quantity += num;
}

void Infantry::setWeight(int w) {
	weight = w;
}

Unit* Infantry::clone() const {
	return new Infantry(this->quantity, this->weight, this->pos, this->infantryType);
}

//Bai 3.5 Danh sach cac don vi quan su
UnitList::UnitList(int capacity) {
	this->head = nullptr;
	this->capacity = capacity;
}

UnitList::~UnitList() {
	Node* curr = head;
	while (curr) {
		Node* next = curr->next;
		delete curr;
		curr = next;
	}
}

Node* UnitList::MakeNode(Unit* unit) {
	if (!unit) return nullptr;
	Node* newNode = new Node;
	newNode->data = unit;
	newNode->next = nullptr;
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
			unit->setQuantity(0);
			return true;
		}
		current = current->next;
	}

	// Them don vi

	if (unit->isInfantryType()) {
		// Chen dau danh sach cho Infantry
		if (head == NULL) {
			head = MakeNode(unit);
			return true;
		}
		else {
			Node* newNode = MakeNode(unit);
			newNode->next = head;
			head = newNode;
			return true;
		}
	}
	else {
		if (!head) {
			head = MakeNode(unit);
			return true;
		}
		else {
			Node* tmp = head;
			while (tmp->next != NULL) {
				tmp = tmp->next;
			}
			Node* newNode = MakeNode(unit);
			tmp->next = newNode;
			return true;
		}
	}
	this->amount++;
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

void UnitList::set_capacity(int x) {
	this->capacity = x;
}

string UnitList::str()const {

	stringstream ss; //luu chuoi str can xuat
	int numOfVeh = 0, numOfInfan = 0;
	string units; //luu chuoi cua cac don vi Unit
	Node* current = head;

	while (current) {
		if (current->data->isVehicleType())
			numOfVeh++;

		if (current->data->isInfantryType())
			numOfInfan++;

		if (!units.empty())
			units += ",";

		units += current->data->str();
		current = current->next;
	}
	if (units.empty()) {
		return "UnitList[count_vehicle=0;count_infantry=0]";
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
		temp = temp->next;
	}
	return false;
}

void UnitList::remove(Unit* unit) {
	if (!head) return;
	while (head->next && head->next->data != unit) {
		head = head->next;
	}
	if (!head->next) return;
	Node* xoa = head->next;
	head->next = head->next->next;
	delete xoa;
}

// Bai 3.4 Quan doi
Army::Army() : unitList(nullptr), battleField(nullptr), name(""), LF(0), EXP(0) {}
Army::Army(Unit** unitArray, int size, string name, BattleField* battleField) {
	this->name = name;
	this->battleField = battleField;
	this->LF = 0;
	this->EXP = 0;

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
	unitList = new UnitList(size);

	for (int i = 0; i < size; ++i) {
		if (unitArray[i]) {
			Unit* clone = unitArray[i]->clone();
			this->unitList->insert(clone);
		}
	}
	Node* head = unitList->getHead();
	while (head) {
		head->data->getAttackScore();
		head = head->next;
	}
}

void Army::updateState() {
	if (!unitList || !unitList->getHead()) {
		LF = 0;
		EXP = 0;
		return;
	}
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
	int sum = LF + EXP;
	this->unitList->set_capacity(UnitList::isSpecialNum(sum) ? 12 : 8);
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

UnitList* Army::getUnitlist() const {
	return unitList;
}

//3.4.1 Quan giai phong
LiberationArmy::LiberationArmy(Unit** unitArray, int size, string name,
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
	bool win = false, war = false;
	if (!enemy || !this->getUnitlist() || !enemy->getUnitlist()) {
		win = false;
		war = false;
		return;
	}

	if (defense) {
		LF = ceil(1.3 * LF);
		EXP = ceil(1.3 * EXP);

		bool isLFgreater = LF >= enemy->getLF();
		bool isEXPgreater = EXP >= enemy->getEXP();

		if (isLFgreater && isEXPgreater) {
			win = true;
			return;
		}

		else if ((isLFgreater && !isEXPgreater) || (isEXPgreater && !isLFgreater)) {
			win = true;
			war = true;
			Node* current = unitList->getHead();
			while (current != nullptr) {
				int q = current->data->getQuantity();
				current->data->setQuantity(ceil(static_cast<int>(1.0 * q * 9 / 10)));
				current = current->next;
			}
			this->updateState();
		}

		else if (!isLFgreater && !isEXPgreater) {
			Node* current = unitList->getHead();
			while (current != nullptr) {
				int q = current->data->getQuantity();
				current->data->setQuantity(increaseToNearestFibo(q));
				current = current->next;
			}
			this->updateState();
		}

	}

	else {
		LF = ceil(1.5 * LF);
		EXP = ceil(1.5 * EXP);

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
		}
		else if (!subsetA.empty() || !subsetB.empty()) {
			if (!subsetA.empty() && subsetB.empty()) {
				int sum_LF = 0;
				Node* tmp_2 = unitList->getHead();
				while (tmp_2 != NULL) {
					sum_LF += tmp_2->data->getAttackScore();
					tmp_2 = tmp_2->next;
				}
				if (sum_LF > enemyLF) {
					win = true;
					war = true;
					for (Unit* unit : subsetA) {
						unitList->remove(unit);
					}
					updateLFandEXP();
				}
				else {
					win = false;
					war = false;
				}
			}
			else {
				int sum_EXP = 0;
				Node* tmp_3 = unitList->getHead();
				while (tmp_3 != NULL) {
					sum_EXP += tmp_3->data->getAttackScore();
					tmp_3 = tmp_3->next;
				}
				if (sum_EXP > enemyEXP) {
					win = true;
					war = true;
					for (Unit* unit : subsetB) {
						unitList->remove(unit);
					}
				}
				else {
					win = false;
					war = false;
				}
			}
		}
		else {
			win = false;
			war = false;
		}

		if (win && war) {
			Node* head1 = enemy->getUnitlist()->getHead();
			vector <Unit*> dao_list;
			while (head1 != nullptr) {
				Unit* unitClone = head1->data->clone();
				dao_list.push_back(unitClone);
				head1 = head1->next;
			}
			for (int i = dao_list.size() - 1; i >= 0; i--) {
				this->unitList->insert(dao_list[i]);
			}
			enemy->getUnitlist()->setHead(nullptr);
			this->updateState();
		}
		else if (war == false) {
			Node* tmp = unitList->getHead();
			while (tmp != NULL) {
				int x = tmp->data->getWeight();
				tmp->data->setWeight(ceil(1.0 * x * 9 / 10));
				tmp = tmp->next;
			}
			this->updateState();
		}
	}
}

bool LiberationArmy::isLiberationArmy() const {
	return true;
}

bool LiberationArmy::isARVN() const {
	return false;
}

string LiberationArmy::str() const {
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
	int sum = 0;

	int left = 0;
	for (int right = 0; right < n; right++) {
		sum += units[right]->getAttackScore();
		Unit* unit_clone = units[right]->clone();
		bestSubset.push_back(unit_clone);
		if (sum > target) {
			while (sum - units[left]->getAttackScore() > target) {
				sum -= units[left++]->getAttackScore();
				bestSubset.erase(bestSubset.begin());
			}
			break;
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

void LiberationArmy::setLF(int LF) {
	this->LF = LF;
}

void LiberationArmy::setEXP(int EXP) {
	this->EXP = EXP;
}

UnitList* LiberationArmy::getUnitlist() const {
	return unitList;
}

//3.4.2 Quan doi Sai Gon
ARVN::ARVN(Unit** unitArray, int size, string name, BattleField* battleField)
	: Army(unitArray, size, name, battleField) {}

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

void ARVN::setLF(int LF) {
	this->LF = LF;
}

void ARVN::setEXP(int EXP) {
	this->EXP = EXP;
}

UnitList* ARVN::getUnitlist() const {
	return unitList;
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
			if (Position::distance(posOfUnit, this->pos) <= 2.0) {
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
		army->updateState();
	}

	else if (army->isARVN()) {
		current = units->getHead();
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			if (Position::distance(posOfUnit, this->pos) <= 4.0) {
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
		army->updateState();
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
		if (Position::distance(posOfUnit, this->pos) <= 2.0 && current->data->isInfantryType()) {
			int oldScore = current->data->getAttackScore();
			int newScore = static_cast<int>(ceil(1.0 * oldScore * 0.9));
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

string Urban::str() const {
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
			double D = Position::distance(posOfUnit, this->pos);
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
					int newWeight = ceil(1.0 * (newScore * 30 - ((int)vehicle * 304)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			current = current->next;
		}
		army->updateState();
	}

	else if (army->isARVN()) {
		while (current != nullptr) {
			Position posOfUnit = current->data->getCurrentPosition();
			double D = Position::distance(posOfUnit, this->pos);
			InfantryType infantry = current->data->getInfantryType();
			if (current->data->isInfantryType() && infantry == REGULARINFANTRY && D <= 3.0) {
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
		army->updateState();
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
			int newWeight = 0;
			Position posOfUnit = current->data->getCurrentPosition();
			InfantryType infantry = current->data->getInfantryType();
			VehicleType vehicle = current->data->getVehicleType();
			bool flag = current->data->isInfantryType();
			if (Position::distance(posOfUnit, this->pos) <= 2) {
				int currentScore = current->data->getAttackScore();
				int newScore = static_cast<int>(ceil(currentScore * 1.0 * 0.8));
				int quantity = current->data->getQuantity();
				if (quantity > 0) {
					if (flag) // Infantry
						newWeight = ceil(1.0 * (newScore - ((int)infantry * 56)) / quantity);
					else // Vehicle
						newWeight = ceil(1.0 * (newScore * 30 - ((int)vehicle * 304)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			current = current->next;
		}
		army->updateState();
	}

	else if (army->isARVN()) {
		while (current != nullptr) {
			int newWeight = 0;
			Position posOfUnit = current->data->getCurrentPosition();
			InfantryType infantry = current->data->getInfantryType();
			VehicleType vehicle = current->data->getVehicleType();
			bool flag = current->data->isInfantryType();
			if (Position::distance(posOfUnit, this->pos) <= 2) {
				int currentScore = current->data->getAttackScore();
				int newScore = static_cast<int>(ceil(currentScore * 1.0 * 1.2));
				int quantity = current->data->getQuantity();
				if (quantity > 0) {
					if (flag) // Infantry
						newWeight = ceil(1.0 * (newScore - ((int)infantry * 56)) / quantity);
					else // Vehicle
						newWeight = ceil(1.0 * (newScore * 30 - ((int)vehicle * 304)) / quantity);
					current->data->setWeight(newWeight);
				}
			}
			current = current->next;
		}
		army->updateState();
	}
}

string SpecialZone::str() const {
	return "SpecialZone";
}

void SpecialZone::getEffect(Army* army) const {
	UnitList* units = army->getUnitlist();
	Node* current = units->getHead();
	while (current != nullptr) {
		Position posOfUnit = current->data->getCurrentPosition();
		if (Position::distance(posOfUnit, this->pos) <= 1.0) {
			int newScore = 0;
			current->data->setWeight(0);
		}
		current = current->next;
	}
	army->updateState();
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
		if (pos == nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new Mountain();
		}
	}

	for (Position* pos : arrayRiver) {
		if (pos == nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new River();
		}
	}

	for (Position* pos : arrayFortification) {
		if (pos == nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new Fortification();
		}
	}

	for (Position* pos : arrayUrban) {
		if (pos == nullptr) continue;
		int r = pos->getRow();
		int c = pos->getCol();
		if (r >= 0 && c >= 0 && r < n_rows && c < n_cols) {
			delete terrain[r][c];
			terrain[r][c] = new Urban();
		}
	}

	for (Position* pos : arraySpecialZone) {
		if (pos == nullptr) continue;
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
	}
}

string BattleField::str() const {
	stringstream ss;
	ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
	return ss.str();
}

vector<vector<TerrainElement*>>& BattleField::getTerrain() {
	return terrain;
}

const vector<vector<TerrainElement*>>& BattleField::getTerrain() const {
	return terrain;
}

int BattleField::get_n_rows() const {
	return n_rows;
}

int BattleField::get_n_cols() const {
	return n_cols;
}

//3.9 Thiet lap
Configuration::Configuration(const string& filepath) {
	num_rows = 0;
	num_cols = 0;
	eventCode = 0;

	ifstream in(filepath);
	if (!in.is_open()) return;

	string line;
	while (getline(in, line)) {
		if (line.empty()) continue;

		// Tach key va value
		size_t pos = line.find('=');
		if (pos == string::npos) continue;
		string key = line.substr(0, pos);
		string val = line.substr(pos + 1);

		// Xu ly so hang va so cot
		if (key == "NUM_ROWS") {
			num_rows = stoi(val);
		}
		else if (key == "NUM_COLS") {
			num_cols = stoi(val);
		}
		else if (key == "EVENT_CODE") {
			int code = stoi(val);
			if (code < 0) eventCode = 0;
			else if (code > 99) eventCode = code % 100;
			else eventCode = code;
		}
		// Xu ly cac mang vi tri dia hinh
		else if (key == "ARRAY_FOREST" || key == "ARRAY_RIVER" ||
			key == "ARRAY_FORTIFICATION" || key == "ARRAY_URBAN" ||
			key == "ARRAY_SPECIAL_ZONE") {
			if (!val.empty() && val.front() == '[') val = val.substr(1);
			if (!val.empty() && val.back() == ']') val.pop_back();

			size_t start = 0;
			while (start < val.size()) {
				size_t l = val.find('(', start);
				size_t r = val.find(')', l);
				if (l == string::npos || r == string::npos) break;
				string pair = val.substr(l + 1, r - l - 1);
				size_t comma = pair.find(',');
				if (comma != string::npos) {
					int row = stoi(pair.substr(0, comma));
					int col = stoi(pair.substr(comma + 1));
					Position* p = new Position(row, col);
					if (key == "ARRAY_FOREST") arrayForest.push_back(p);
					else if (key == "ARRAY_RIVER") arrayRiver.push_back(p);
					else if (key == "ARRAY_FORTIFICATION") arrayFortification.push_back(p);
					else if (key == "ARRAY_URBAN") arrayUrban.push_back(p);
					else if (key == "ARRAY_SPECIAL_ZONE") arraySpecialZone.push_back(p);
				}
				start = r + 1;
			}
		}
		// Xu ly danh sach Unit
		else if (key == "UNIT_LIST") {
			if (!val.empty() && val.front() == '[') val = val.substr(1);
			if (!val.empty() && val.back() == ']') val.pop_back();

			size_t start = 0;
			while (start < val.size()) {
				size_t lparen = val.find('(', start);
				if (lparen == string::npos) break;
				string type = val.substr(start, lparen - start);
				size_t rparen = lparen;
				int paren_count = 0;
				// Tim dung dau ngoac dong cua unit hien tai
				for (; rparen < val.size(); ++rparen) {
					if (val[rparen] == '(') paren_count++;
					else if (val[rparen] == ')') {
						paren_count--;
						if (paren_count == 0) break;
					}
				}
				if (rparen == val.size()) break; // khong tim thay ngoac dong
				string params = val.substr(lparen + 1, rparen - lparen - 1);

				// Xu ly tach params nhu cu
				vector<string> fields;
				size_t field_start = 0;
				for (int field = 0; field < 4; ++field) {
					if (field == 2) {
						// truong vi tri (x,y)
						size_t lpos = params.find('(', field_start);
						size_t rpos = params.find(')', lpos);
						fields.push_back(params.substr(lpos, rpos - lpos + 1));
						field_start = rpos + 2;
					}
					else {
						size_t comma = params.find(',', field_start);
						if (comma == string::npos || field == 3) {
							fields.push_back(params.substr(field_start));
							break;
						}
						fields.push_back(params.substr(field_start, comma - field_start));
						field_start = comma + 1;
					}
				}

				if (fields.size() == 4) {
					int quantity = stoi(fields[0]);
					int weight = stoi(fields[1]);
					int row = 0, col = 0;
					string posstr = fields[2];
					size_t lpos = posstr.find('(');
					size_t comma = posstr.find(',', lpos);
					size_t rpos = posstr.find(')', comma);
					if (lpos != string::npos && comma != string::npos && rpos != string::npos) {
						row = stoi(posstr.substr(lpos + 1, comma - lpos - 1));
						col = stoi(posstr.substr(comma + 1, rpos - comma - 1));
					}
					int army = stoi(fields[3]);
					Position* p = new Position(row, col);
					Unit* u = nullptr;
					// Vehicle types
					if (type == "TANK") u = new Vehicle(quantity, weight, *p, TANK);
					else if (type == "MORTAR") u = new Vehicle(quantity, weight, *p, MORTAR);
					else if (type == "ANTIAIRCRAFT") u = new Vehicle(quantity, weight, *p, ANTIAIRCRAFT);
					else if (type == "ARMOREDCAR") u = new Vehicle(quantity, weight, *p, ARMOREDCAR);
					else if (type == "APC") u = new Vehicle(quantity, weight, *p, APC);
					else if (type == "ARTILLERY") u = new Vehicle(quantity, weight, *p, ARTILLERY);
					else if (type == "TRUCK") u = new Vehicle(quantity, weight, *p, TRUCK);
					// Infantry types
					else if (type == "REGULARINFANTRY") u = new Infantry(quantity, weight, *p, REGULARINFANTRY);
					else if (type == "SNIPER") u = new Infantry(quantity, weight, *p, SNIPER);
					else if (type == "ANTIAIRCRAFTSQUAD") u = new Infantry(quantity, weight, *p, ANTIAIRCRAFTSQUAD);
					else if (type == "MORTARSQUAD") u = new Infantry(quantity, weight, *p, MORTARSQUAD);
					else if (type == "ENGINEER") u = new Infantry(quantity, weight, *p, ENGINEER);
					else if (type == "SPECIALFORCES") u = new Infantry(quantity, weight, *p, SPECIALFORCES);

					if (!u) delete p;
					else if (army == 0) liberationUnits.push_back(u);
					else if (army == 1) ARVNUnits.push_back(u);
					else delete u;
				}
				// Bo qua dau phay va space sau ngoac dong
				start = rparen + 1;
				while (start < val.size() && (val[start] == ',' || isspace(val[start]))) ++start;
			}
		}
	}
	in.close();
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
	ss << "[num_rows=" << num_rows << ",";
	ss << "num_cols=" << num_cols << ",";

	// arrayForest
	ss << "arrayForest=[";
	for (size_t i = 0; i < arrayForest.size(); ++i) {
		if (arrayForest[i]) ss << "(" << arrayForest[i]->getRow() << "," << arrayForest[i]->getCol() << ")";
		else ss << "null";
		if (i < arrayForest.size() - 1) ss << ",";
	}
	ss << "],";

	// arrayRiver
	ss << "arrayRiver=[";
	for (size_t i = 0; i < arrayRiver.size(); ++i) {
		if (arrayRiver[i]) ss << "(" << arrayRiver[i]->getRow() << "," << arrayRiver[i]->getCol() << ")";
		else ss << "null";
		if (i < arrayRiver.size() - 1) ss << ",";
	}
	ss << "],";

	// arrayFortification
	ss << "arrayFortification=[";
	for (size_t i = 0; i < arrayFortification.size(); ++i) {
		if (arrayFortification[i]) ss << "(" << arrayFortification[i]->getRow() << "," << arrayFortification[i]->getCol() << ")";
		else ss << "null";
		if (i < arrayFortification.size() - 1) ss << ",";
	}
	ss << "],";

	// arrayUrban
	ss << "arrayUrban=[";
	for (size_t i = 0; i < arrayUrban.size(); ++i) {
		if (arrayUrban[i]) ss << "(" << arrayUrban[i]->getRow() << "," << arrayUrban[i]->getCol() << ")";
		else ss << "null";
		if (i < arrayUrban.size() - 1) ss << ",";
	}
	ss << "],";

	// arraySpecialZone
	ss << "arraySpecialZone=[";
	for (size_t i = 0; i < arraySpecialZone.size(); ++i) {
		if (arraySpecialZone[i]) ss << "(" << arraySpecialZone[i]->getRow() << "," << arraySpecialZone[i]->getCol() << ")";
		else ss << "null";
		if (i < arraySpecialZone.size() - 1) ss << ",";
	}
	ss << "],";

	// liberationUnits
	ss << "liberationUnits=[";
	for (size_t i = 0; i < liberationUnits.size(); ++i) {
		if (liberationUnits[i]) ss << liberationUnits[i]->str();
		else ss << "null";
		if (i < liberationUnits.size() - 1) ss << ",";
	}
	ss << "],";

	// ARVNUnits
	ss << "ARVNUnits=[";
	for (size_t i = 0; i < ARVNUnits.size(); ++i) {
		if (ARVNUnits[i]) ss << ARVNUnits[i]->str();
		else ss << "null";
		if (i < ARVNUnits.size() - 1) ss << ",";
	}
	ss << "],";

	ss << "eventCode=" << eventCode;
	ss << "]";
	return ss.str();
}

int Configuration::getNumRows() const {
	return num_rows;
}

int Configuration::getNumCols() const {
	return num_cols;
}

int Configuration::getEventCode() const {
	return eventCode;
}

vector<Position*> Configuration::getArrayForest() const {
	return arrayForest;
}

vector<Position*> Configuration::getArrayRiver() const {
	return arrayRiver;
}

vector<Position*> Configuration::getArrayFortification() const {
	return arrayFortification;
}

vector<Position*> Configuration::getArrayUrban() const {
	return arrayUrban;
}

vector<Position*> Configuration::getArraySpecialZone() const {
	return arraySpecialZone;
}

vector<Unit*> Configuration::getLiberationUnits() const {
	return liberationUnits;
}

vector<Unit*> Configuration::getARVNUnits() const {
	return ARVNUnits;
}

//3.10 class HCMcampaign
HCMCampaign::HCMCampaign(const string& config_file_path) {
	//khoi tao cau hinh
	this->config = new Configuration(config_file_path);

	//lay thong tin ban do - dia hinh
	int n_rows = config->getNumRows();
	int n_cols = config->getNumCols();
	vector<Position*> arrayForest = config->getArrayForest();
	vector<Position*> arrayRiver = config->getArrayRiver();
	vector<Position*> arrayFortification = config->getArrayFortification();
	vector<Position*> arrayUrban = config->getArrayUrban();
	vector<Position*> arraySpecialZone = config->getArraySpecialZone();

	//khoi tao BattleField
	this->battleField = new BattleField(n_rows, n_cols, arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone);

	//chuyen vecto <Unit*> sang Unit** -> de xu li
	auto toPointerArray = [](const vector<Unit*>& vect)->Unit** {
		if (vect.empty()) return nullptr;
		Unit** arr = new Unit * [vect.size()];
		for (size_t i = 0; i < vect.size(); ++i) {
			arr[i] = vect[i];
		}
		return arr;
		};

	//lay cac don vi qsu
	vector<Unit*> liberationUnits = config->getLiberationUnits();
	vector<Unit*> ARVNUnits = config->getARVNUnits();

	//chuyen sang mang 2 chieu
	Unit** liberationArray = toPointerArray(liberationUnits);
	Unit** ARVNArray = toPointerArray(ARVNUnits);

	//khoi tao hai quan doi
	this->liberationArmy = new LiberationArmy(liberationArray, liberationUnits.size(), "LiberationArmy", this->battleField);
	this->arvn = new ARVN(ARVNArray, ARVNUnits.size(), "ARVN", this->battleField);

	delete[] liberationArray;
	delete[] ARVNArray;
}

void HCMCampaign::run() {
	int r = this->battleField->get_n_rows();
	int c = this->battleField->get_n_cols();
	vector<vector<TerrainElement*>>& terrain = this->battleField->getTerrain();

	//ap dung hieu ung dia hinh
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			terrain[i][j]->getEffect(this->liberationArmy);
			terrain[i][j]->getEffect(this->arvn);
		}
	}

	//cap nhat lai luc luong
	liberationArmy->updateState();
	arvn->updateState();

	//xu li eventCode
	int eventCode = this->config->getEventCode();
	if (eventCode < 75) {
		liberationArmy->fight(arvn, false);
		arvn->fight(liberationArmy, true);
		arvn->updateState();
		liberationArmy->updateState();
	}
	else {
		arvn->fight(liberationArmy, false);
		liberationArmy->fight(arvn, true);

		liberationArmy->updateState();
		arvn->updateState();

		liberationArmy->fight(arvn, false);
		arvn->updateState();
		liberationArmy->updateState();
	}

	//xoa cac don vi yeu
	auto removeWeakUnits = [](Army* army) {
		UnitList* unitList = army->getUnitlist();
		Node* current = (army->getUnitlist()) ? army->getUnitlist()->getHead() : nullptr;
		Node* prev = nullptr;
		while (current) {
			if (current->data->getAttackScore() <= 5.0) {
				Node* toDelete = current;
				if (!prev) {
					current = current->next;
					unitList->setHead(current);
					delete toDelete;
					continue;
				}
				else {
					prev->next = current->next;
					current = current->next;
					delete toDelete;
					continue;
				}
			}
			prev = current;
			current = current->next;
		}
	};

	removeWeakUnits(liberationArmy);
	removeWeakUnits(arvn);

	//cap nhat trang thai
	liberationArmy->updateState();
	arvn->updateState();
}

string HCMCampaign::printResult() const {
	stringstream ss;
	ss << "LIBERATIONARMY[LF=" << to_string(liberationArmy->getLF())
		<< ",EXP=" << to_string(liberationArmy->getEXP()) << "]"
		<< "-ARVN[LF=" << to_string(arvn->getLF())
		<< ",EXP=" << to_string(arvn->getEXP()) << "]";
	return ss.str();
}



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
