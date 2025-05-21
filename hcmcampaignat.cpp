#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// 3.1
Unit::Unit(int quantity, int weight, Position pos) {
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
}

Unit :: ~Unit() {}

Position Unit::getCurrentPosition() const {
    return pos;
}

int Unit::get_quantity() {
    return quantity;
}
int Unit::get_weight() {
    return weight;
}
void Unit::set_quantity(int x) {
    this->quantity = x;
}
void Unit::set_weight(int x) {
    this->weight = x;
}
void Unit::set_pos(Position pos) {
    this->pos = pos;
}
// task 3.2
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType) : Unit(quantity, weight, pos) {
    this->vehicleType = vehicleType;
}
int Vehicle::getAttackScore() {
    return ceil(1.0 * ((int)vehicleType * 304 + quantity * weight) / 30);
}
bool Vehicle::isInfantry() const {
    return false;
}
bool Vehicle::isVehicle() const {
    return true;
}
string Vehicle::str() const {
    string vehicle = "";
    switch (vehicleType) {
    case TRUCK:
        vehicle += "TRUCK";
        break;
    case MORTAR:
        vehicle += "MORTAR";
        break;
    case ANTIAIRCRAFT:
        vehicle += "ANTIAIRCRAFT";
        break;
    case ARMOREDCAR:
        vehicle += "ARMOREDCAR";
        break;
    case APC:
        vehicle += "APC";
        break;
    case ARTILLERY:
        vehicle += "ARTILLERY";
        break;
    case TANK:
        vehicle += "TANK";
        break;
    default: break;
    }
    return "Vehicle[vehicleType=" + vehicle + ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) + ",position=" + pos.str() + "]";
}
VehicleType Vehicle::getVehicleType() const {
    return vehicleType;
}
InfantryType Vehicle::getInfantryType() const {
    throw std::logic_error("");
}
Unit* Vehicle::clone() const {
    return new Vehicle(this->quantity, this->weight, this->pos, this->vehicleType);
}

// task 3.3
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) : Unit(quantity, weight, pos) {
    this->infantryType = infantryType;
}
bool Infantry::isInfantry() const {
    return true;
}
bool Infantry::isVehicle() const {
    return false;
}
int Infantry::sum_digit(int n) {
    int sum = 0;
    while (n) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}
int Infantry::getScore(int quantity, int weight) {
    return (int)infantryType * 56 + quantity * weight;
}
int Infantry::getAttackScore() {
    int score = getScore(quantity, weight);
    int res = ceil(sqrt(weight));
    bool check = (res * res == weight) ? true : false;
    if (infantryType == SPECIALFORCES && check == true) {
        score += 75;
    }
    int tmp = score + 4;
    while (tmp >= 10) {
        tmp = sum_digit(tmp);
    }
    if (tmp > 7) this->quantity = ceil(1.2 * this->quantity);
    else if (tmp < 3)
    {
        this->quantity = this->quantity - ceil(0.1 * this->quantity);
        if (quantity <= 0) quantity = 0;
    }
    return getScore(quantity, weight) + ((infantryType == SPECIALFORCES && check == true) ? 75 : 0);
}
string Infantry::str() const {
    string soldier = "";
    switch (infantryType) {
    case SNIPER:
        soldier += "SNIPER";
        break;
    case ANTIAIRCRAFTSQUAD:
        soldier += "ANTIAIRCRAFTSQUAD";
        break;
    case MORTARSQUAD:
        soldier += "MORTARSQUAD";
        break;
    case ENGINEER:
        soldier += "ENGINEER";
        break;
    case SPECIALFORCES:
        soldier += "SPECIALFORCES";
        break;
    case REGULARINFANTRY:
        soldier += "REGULARINFANTRY";
        break;
    default: break;
    }
    return "Infantry[infantryType=" + soldier + ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) + ",position=" + pos.str() + "]";
}
InfantryType Infantry::getInfantryType() const {
    return infantryType;
}
VehicleType Infantry::getVehicleType() const {
    throw std::logic_error("");
}
Unit* Infantry::clone() const {
    return new Infantry(this->quantity, this->weight, this->pos, this->infantryType);
}

// task 3.4
Army::Army() : unitList(nullptr), battleField(nullptr), name(""), LF(0), EXP(0) {}
Army::Army(Unit** unitArray, int size, string name, BattleField* battlefield)
    : battleField(battlefield), name(name), unitList(new UnitList(size)), LF(0), EXP(0) {
    int sum_scoreI = 0, sum_scoreV = 0;
    for (int i = 0; i < size; i++) {
        if (unitArray[i]->isInfantry()) sum_scoreI += unitArray[i]->getAttackScore();
        else sum_scoreV += unitArray[i]->getAttackScore();
    }
    LF = (sum_scoreV > 1000) ? 1000 : sum_scoreV;
    EXP = (sum_scoreI > 500) ? 500 : sum_scoreI;

    for (int i = 0; i < size; i++) {
        Unit* cloned = unitArray[i]->clone();
        this->unitList->insert(cloned);
    }
    Node* head = unitList->get_head();
    while (head) {
        head->data->getAttackScore();
        head = head->next;
    }
}
void Army::make() {
    if (!unitList || !unitList->get_head()) {
        LF = 0;
        EXP = 0;
        return;
    }
    Node* tmp = this->unitList->get_head();
    int sum_score_I = 0, sum_score_V = 0;
    while (tmp != NULL) {
        if (tmp->data->isInfantry()) {
            sum_score_I += tmp->data->getAttackScore();
        }
        else {
            sum_score_V += tmp->data->getAttackScore();
        }
        tmp = tmp->next;
    }
    LF = (sum_score_V > 1000) ? 1000 : sum_score_V;
    EXP = (sum_score_I > 500) ? 500 : sum_score_I;
    int S = LF + EXP;
    bool check = false;
    for (int i = 2; i <= (int)sqrt(S); i++) {
        if (special_num(S, i)) {
            check = true;
            break;
        }
    }
    if (check) {
        unitList->set_capacity(12);
    }
    else {
        unitList->set_capacity(8);
    }
}
bool Army::special_num(int n, int x) {
    while (n > 0) {
        int du = n % x;
        if (du > 1) {
            // Nếu dư > 1 thì phải dùng lặp lại lũy thừa → không hợp lệ
            return false;
        }
        n /= x;
    }
    return true;
}
int Army::get_EXP() {
    return EXP;
}
int Army::get_LF() {
    return LF;
}
UnitList* Army::get_unitList() {
    return unitList;
}
void Army::set_LF(int x) {
    if (x > 1000) return;
    this->LF = x;
}
void Army::set_EXP(int x) {
    if (x > 500) return;
    this->EXP = x;
}

//task 3.4.1
LiberationArmy::LiberationArmy(Unit** unitArray, int size, string name, BattleField* battlefield) : Army(unitArray, size, name, battlefield) {}
void LiberationArmy::fight(Army* enemy, bool defense) {
    bool war = false;
    bool win = false;
    if (defense == false) {
        LF = ceil(3.0 * LF / 2);
        EXP = ceil(1.0 * EXP * 3 / 2);
        Node* tmp_1 = unitList->get_head();
        vector <Unit*> v1; // lưu điểm vehicle - LF
        vector <Unit*> v2; // lưu điểm infantry - EXP
        while (tmp_1 != NULL) {
            if (tmp_1->data->isInfantry()) {
                v2.push_back(tmp_1->data);
            }
            else v1.push_back(tmp_1->data);
            tmp_1 = tmp_1->next;
        }
        int EXP_enemy = enemy->get_EXP();
        int LF_enemy = enemy->get_LF();
        // sắp xếp lại vector <int> 
        vector <Unit*> tmp1 = sort_vector(v1);
        vector <Unit*> tmp2 = sort_vector(v2);

        vector <Unit*> infantry_list;
        vector <Unit*> vehicle_list;
        int sum1 = 0, sum2 = 0;
        // tìm tập hợp nhỏ nhất lớn hơn 1 số

        int left = 0;
        for (int right = 0; right < tmp1.size(); right++) {
            sum1 += tmp1[right]->getAttackScore();
            vehicle_list.push_back(tmp1[right]);
            if (sum1 > enemy->get_LF())
            {
                while (sum1 - tmp1[left]->getAttackScore() > enemy->get_LF()) {
                    sum1 -= tmp1[left]->getAttackScore();
                    vehicle_list.erase(vehicle_list.begin());
                    left++;
                }
                break;
            }
        }
        left = 0;
        for (int right = 0; right < tmp2.size(); right++) {
            sum2 += tmp2[right]->getAttackScore();
            infantry_list.push_back(tmp2[right]);
            if (sum2 > enemy->get_EXP()) {
                while (sum2 - tmp2[left]->getAttackScore() > enemy->get_EXP()) {
                    sum2 -= tmp2[left]->getAttackScore();
                    infantry_list.erase(infantry_list.begin());
                    left++;
                }
                break;
            }
        }

        if (sum1 > LF_enemy && sum2 > EXP_enemy) {
            // xoá khỏi danh sách
            // xoá tổ hợp A
            delete_set(vehicle_list);
            // xoá tổ hợp B
            delete_set(infantry_list);
            war = true;
            win = true;
        }
        else if (sum1 <= LF_enemy && sum2 > EXP_enemy) {
            int sum_LF = 0;
            Node* tmp_2 = unitList->get_head();
            while (tmp_2 != NULL) {
                sum_LF += tmp_2->data->getAttackScore();
                tmp_2 = tmp_2->next;
            }
            if (sum_LF > enemy->get_LF()) {
                delete_set(infantry_list);
                war = true;
                win = true;
            }
            else {
                war = false;
                win = false;
            }
        }
        else if (sum1 > LF_enemy && sum2 <= EXP_enemy) {
            int sum_EXP = 0;
            Node* tmp_3 = unitList->get_head();
            while (tmp_3 != NULL) {
                sum_EXP += tmp_3->data->getAttackScore();
                tmp_3 = tmp_3->next;
            }
            if (sum_EXP > enemy->get_EXP()) {
                delete_set(vehicle_list);
                war = true;
                win = true;
            }
            else {
                war = false;
                win = false;
            }
        }
        else if (sum1 <= LF_enemy && sum2 <= EXP_enemy) {
            war = false;
            win = false;
        }

        if (win && war) {
            Node* head1 = enemy->get_unitList()->get_head();
            vector <Unit*> list_reverse;
            while (head1 != nullptr) {
                //cout << head1->data->str() << endl;
                Unit* unitClone = head1->data->clone();
                list_reverse.push_back(unitClone);
                head1 = head1->next;
            }
            for (int i = list_reverse.size() - 1; i >= 0; i--) {
                this->unitList->insert(list_reverse[i]);
            }
            enemy->get_unitList()->set_head(nullptr);
            this->make();
        }
        else if (war == false) {
            Node* tmp = unitList->get_head();
            while (tmp != NULL) {
                int x = tmp->data->get_weight();
                tmp->data->set_weight(ceil(1.0 * x * 9 / 10));
                tmp = tmp->next;
            }
            this->make();
        }
    }
    else if (defense == true) {
        LF = ceil(1.0 * LF * 13 / 10);
        EXP = ceil(1.0 * EXP * 13 / 10);
        if (LF >= enemy->get_LF() && EXP >= enemy->get_EXP()) {
            win = true;
        }
        else if ((LF < enemy->get_LF() && EXP >= enemy->get_EXP()) || (EXP < enemy->get_EXP() && LF >= enemy->get_LF())) {
            win = true;
            Node* head = unitList->get_head();
            while (head != NULL) {
                int x = head->data->get_quantity();
                head->data->set_quantity(ceil(1.0 * x * 9 / 10));
                head = head->next;
            }
            this->make();
        }
        else if (LF < enemy->get_LF() && EXP < enemy->get_EXP()) {
            Node* head = unitList->get_head();
            while (head != NULL) {
                int x = updateToFibo(head->data->get_quantity());
                head->data->set_quantity(x);
                head = head->next;
            }
            this->make();
        }
    }
}
long long LiberationArmy::lowerbound(int x, long long f[]) {
    int l = 0, r = 92;
    long long res = 0;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (f[mid] < x) {
            l = mid + 1;
        }
        else {
            res = f[mid];
            r = mid - 1;
        }
    }
    return res;
}
long long LiberationArmy::updateToFibo(int x) {
    long long f[100];
    f[0] = 0;
    f[1] = 1;
    for (int i = 2; i <= 92; i++) {
        f[i] = f[i - 1] + f[i - 2];
    }
    return lowerbound(x, f);
}
vector <Unit*> LiberationArmy::sort_vector(vector <Unit*> v) {
    vector <Unit*> result;
    for (int i = 1; i < v.size(); i++) {
        Unit* x = v[i];
        int pos = i - 1;
        while (pos >= 0 && x->getAttackScore() < v[pos]->getAttackScore()) {
            v[pos + 1] = v[pos];
            pos--;
        }
        v[pos + 1] = x;
    }
    result = v;
    return result;
}
void LiberationArmy::delete_set(vector <Unit*> v) {
    if (v.empty()) return;
    for (Unit* target : v) {
        Node* curr = unitList->get_head();
        Node* prev = nullptr;
        while (curr != nullptr) {
            if (curr->data->str() == target->str()) {
                Node* toDelete = curr;
                if (prev == nullptr) { // curr == head
                    unitList->set_head(curr->next);
                    curr = curr->next;
                }
                else {
                    prev->next = curr->next;
                    curr = curr->next;
                }
                delete toDelete;
                break;  // chỉ xóa 1 node khớp
            }
            else {
                prev = curr;
                curr = curr->next;
            }
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
        ",battleField=" + ((this->battleField != NULL) ? this->battleField->str() : "]");
}

// task 3.4.2
ARVN::ARVN(Unit** unitArray, int size, string name, BattleField* battleField) : Army(unitArray, size, name, battleField) {}
void ARVN::fight(Army* enemy, bool defense) {
    if (defense == false) {
        Node* head = unitList->get_head();
        Node* prev = NULL;
        while (head != NULL) {
            int x = head->data->get_quantity();
            head->data->set_quantity(ceil(0.8 * x));
            if (head->data->get_quantity() <= 1) {
                Node* xoa = head;
                if (!prev) {
                    unitList->set_head(head->next);
                    head = head->next;
                }
                else {
                    prev->next = head->next;
                    head = head->next;
                }
                delete xoa;
                continue;
            }
            prev = head;
            head = head->next;
        }
        this->make();
    }
    else if (defense == true) {
        if (!unitList || !unitList->get_head()) {
            // tức là đã xả ra chiến tranh và thua => bị tịch thu hết đơn vị quân sự
            this->set_EXP(0);
            this->set_LF(0);
            return;
        }
        Node* head = unitList->get_head();
        UnitList* unitList1 = enemy->get_unitList(); // lấy ra danh sách quân sự của quân giải phóng
        if (unitList1->Unit_exist(head->data)) {
            int x = head->data->get_weight();
            head->data->set_weight(ceil(0.8 * x));
        }
        else {
            Node* xoa = head;
            head = head->next;
            delete xoa;
            unitList->set_head(head);
        }
        while (head != NULL && head->next != NULL) {
            if (!unitList1->Unit_exist(head->next->data)) {
                // chưa tổn tại trong quân giải phóng -> quân giải phóng lấy -> xoá khỏi CQSG
                Node* xoa = head->next;
                head->next = head->next->next;
                delete xoa;
            }
            else {
                // tồn tại trong quân giải phóng => trừ 20% trọng số weight;
                int x = head->next->data->get_weight();
                head->next->data->set_weight(ceil(0.8 * x));
            }
            head = head->next;
        }
        this->make();
    }
}
string ARVN::str() const {
    string LF = to_string(this->LF);
    string EXP = to_string(this->EXP);
    return "ARVN[LF=" + LF +
        ",EXP=" + EXP +
        ",unitList=" + ((this->unitList != nullptr) ? this->unitList->str() : "UnitList[count_vehicle=0;count_infantry=0]") +
        ",battleField=" + ((this->battleField != NULL) ? this->battleField->str() : "]");
};
bool ARVN::isARVN() const {
    return true;
}
bool ARVN::isLiberationArmy() const {
    return false;
}

// task 3.5
UnitList::UnitList() {
    this->head = NULL;
}
void UnitList::set_head(Node* newHead) {
    this->head = newHead;
}
UnitList::UnitList(int capacity) {
    this->head = NULL;
    this->capacity = capacity;
}
Node* UnitList::makeNode(Unit* unit) {
    Node* newNode = new Node;
    newNode->data = unit;
    newNode->next = NULL;
    return newNode;
}
bool UnitList::Unit_exist(Unit* unit) {
    Node* tmp = head;
    while (tmp != NULL) {
        if (tmp->data == unit) return true;
        tmp = tmp->next;
    }
    return false;
}
bool UnitList::insert(Unit* unit) {
    if (unit->isInfantry()) {
        auto type = unit->getInfantryType();
        if (isContain(type)) {
            Node* curr = head;
            while (curr != nullptr) {
                if (curr->data->isInfantry() && curr->data->getInfantryType() == type) {
                    curr->data->set_quantity(curr->data->get_quantity() + unit->get_quantity());
                    unit->set_quantity(0);
                    return true;
                }
                curr = curr->next;
            }
        }
        else {
            if (this->size_of_list() >= this->get_capacity()) return false;
            if (head == NULL) {
                head = makeNode(unit);
                return true;
            }
            else {
                Node* newNode = makeNode(unit);
                newNode->next = head;
                head = newNode;
                return true;
            }
        }
    }
    else {
        VehicleType type = unit->getVehicleType();
        if (isContain(type)) {
            Node* curr = head;
            while (curr != nullptr) {
                if (curr->data->isVehicle() && curr->data->getVehicleType() == type) {
                    curr->data->set_quantity(curr->data->get_quantity() + unit->get_quantity());
                    curr->data->set_weight(max(curr->data->get_weight(), unit->get_weight()));
                    return true;
                }
                curr = curr->next;
            }
        }
        else {
            if (this->size_of_list() >= this->get_capacity()) return false;
            if (head == NULL) {
                head = makeNode(unit);
                return true;
            }
            else {
                Node* tmp = head;
                while (tmp->next != NULL) {
                    tmp = tmp->next;
                }
                Node* newNode = makeNode(unit);
                tmp->next = newNode;
                return true;
            }
        }
    }
    return false;
}
bool UnitList::isContain(VehicleType vehicleType) {
    Node* tmp = head;
    while (tmp != NULL) {
        if (tmp->data->isVehicle() && tmp->data->getVehicleType() == vehicleType) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}
bool UnitList::isContain(InfantryType infantryType) {
    Node* tmp = head;
    while (tmp != NULL) {
        if (tmp->data->isInfantry() && tmp->data->getInfantryType() == infantryType) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}
string UnitList::str() const {
    int cnt_In = 0, cnt_Ve = 0;
    Node* tmp = head;
    while (tmp != NULL) {
        if (tmp->data->isVehicle()) cnt_Ve++;
        else cnt_In++;
        tmp = tmp->next;
    }
    string result = "UnitList[count_vehicle=";
    result += to_string(cnt_Ve);
    result += ";count_infantry=";
    result += to_string(cnt_In);
    if (cnt_In == 0 && cnt_Ve == 0) return result + "]";
    result += ";";
    Node* tmp1 = head;
    while (tmp1 != NULL) {
        result += tmp1->data->str();
        if (tmp1->next != NULL) result += ',';
        tmp1 = tmp1->next;
    }
    return result + ']';
}
int UnitList::size_of_list() {
    Node* tmp = head;
    int count = 0;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    return count;
}
Node* UnitList::get_head() {
    return head;
}
void UnitList::set_capacity(int x) {
    capacity = x;
}
int UnitList::get_capacity() {
    return capacity;
}

// task 3.6
Position::Position(int r, int c) {
    this->r = r;
    this->c = c;
}
Position::Position(const string& str_pos) {
    vector <int> v;
    int n = 0;
    bool flag = false;
    int sign = 1;
    for (char x : str_pos) {
        if (isdigit(x)) {
            flag = true;
            n = n * 10 + (x - '0') * sign;
        }
        else {
            if (x == '-') sign = -1;
            if (flag) {
                v.push_back(n);
                n = 0;
                sign = 1;
                flag = false;
            }
        }
    }
    r = v[0];
    c = v[1];
}
int Position::getRow() const {
    return r;
}
int Position::getCol() const {
    return c;
}
void Position::setRow(int r) {
    this->r = r;
}
void Position::setCol(int c) {
    this->c = c;
}
string Position::str() const {
    string result = '(' + to_string(r) + ',' + to_string(c) + ')';
    return result;
}
double Position::distance(Position a, Position b) {
    double x1 = a.r;
    double x2 = b.r;
    double y1 = a.c;
    double y2 = b.c;
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// task 3.7
TerrainElement::TerrainElement() {}
TerrainElement :: ~TerrainElement() {};

void Road::getEffect(Army* army) {}
string Road::str() const {
    return "Road";
}
Position Road::get_pos() const {
    return this->pos;
}

void Mountain::getEffect(Army* army) {
    UnitList* unitList = army->get_unitList();
    if (!unitList || unitList->get_head() == nullptr) return;
    Node* head = unitList->get_head();
    if (army->isLiberationArmy()) {
        while (head != NULL) {
            Position pos_of_unit = head->data->getCurrentPosition();
            if (pos_of_unit.distance(pos_of_unit, this->pos) <= 2.0) {
                int x = head->data->getAttackScore();
                if (head->data->isInfantry()) {
                    army->set_EXP(army->get_EXP() + ceil(0.3 * x));
                }
                else {
                    army->set_LF(army->get_LF() - ceil(0.1 * x));
                }
            }
            head = head->next;
        }
    }
    else {
        while (head != NULL) {
            Position pos_of_unit = head->data->getCurrentPosition();
            if (pos_of_unit.distance(pos_of_unit, this->pos) <= 4.0) {
                int x = head->data->getAttackScore();
                if (head->data->isInfantry()) {
                    army->set_EXP(army->get_EXP() + ceil(0.2 * x));
                }
                else {
                    army->set_LF(army->get_LF() - ceil(0.05 * x));
                }
            }
            head = head->next;
        }
    }
}
Position Mountain::get_pos() const {
    return pos;
}
string Mountain::str() const {
    return "Mountain";
}

void River::getEffect(Army* army) {
    UnitList* unitList = army->get_unitList();
    if (!unitList || unitList->get_head() == nullptr) return;
    Node* head = unitList->get_head();
    while (head != NULL) {
        Position pos_of_unit = head->data->getCurrentPosition();
        if (pos_of_unit.distance(pos_of_unit, this->pos) <= 2.0) {
            if (head->data->isInfantry()) {
                int current_score = head->data->getAttackScore(),
                    new_score = ceil(0.9 * current_score),
                    quantity = head->data->get_quantity();
                if (quantity > 0) {
                    int new_weight = ceil(1.0 * (new_score - (int)head->data->getInfantryType() * 56) / quantity);
                    head->data->set_weight(new_weight);
                }
            }
        }
        head = head->next;
    }
    army->make();
}
Position River::get_pos() const {
    return pos;
}
string River::str() const {
    return "River";
}

void Urban::getEffect(Army* army) {
    UnitList* unitList = army->get_unitList();
    if (!unitList || unitList->get_head() == nullptr) return;
    Node* head = unitList->get_head();
    if (army->isLiberationArmy()) {
        while (head != NULL) {
            if (head->data->isInfantry()) {
                InfantryType infantry = head->data->getInfantryType();
                Position pos_of_unit = head->data->getCurrentPosition();
                double distance = pos_of_unit.distance(pos_of_unit, this->pos);
                int extra_score = ceil(2.0 * head->data->getAttackScore() / distance);
                if ((infantry == SPECIALFORCES || infantry == REGULARINFANTRY) && distance <= 5.0) {
                    int current_score = head->data->getAttackScore(),
                        new_score = ceil(current_score + extra_score),
                        quantity = head->data->get_quantity(),
                        newWeight = ceil(1.0 * (new_score - (int)head->data->getInfantryType() * 56) / quantity);
                    head->data->set_weight(newWeight);
                }
            }
            else if (head->data->isVehicle()) {
                VehicleType vehicleType = head->data->getVehicleType();
                Position pos_of_unit = head->data->getCurrentPosition();
                double distance = pos_of_unit.distance(pos_of_unit, this->pos);
                if (vehicleType == ARTILLERY && distance <= 2.0) {
                    int current_score = head->data->getAttackScore(),
                        new_score = ceil(1.0 * current_score / 2),
                        quantity = head->data->get_quantity(),
                        newWeight = ceil(1.0 * (new_score * 30 - (int)head->data->getVehicleType() * 304) / quantity);
                    head->data->set_weight(newWeight);
                }
            }
            head = head->next;
        }
    }
    else {
        while (head != NULL) {
            if (head->data->isInfantry()) {
                InfantryType infantryType = head->data->getInfantryType();
                Position current_pos = head->data->getCurrentPosition();
                double distance = current_pos.distance(current_pos, this->pos);
                if (infantryType == REGULARINFANTRY && distance <= 3.0) {
                    int current_score = head->data->getAttackScore(),
                        extra_score = ceil(3.0 * current_score / (2.0 * distance)),
                        new_score = current_score + extra_score,
                        quantity = head->data->get_quantity(),
                        new_weight = ceil(1.0 * (new_score - (int)head->data->getInfantryType() * 56) / quantity);
                    head->data->set_weight(new_weight);
                }
            }
            head = head->next;
        }
    }
    army->make();
}
Position Urban::get_pos() const {
    return pos;
}
string Urban::str() const {
    return "Urban";
}

void Fortification::getEffect(Army* army) {
    UnitList* unitList = army->get_unitList();
    if (!unitList || unitList->get_head() == nullptr) return;
    Node* head = unitList->get_head();
    if (army->isLiberationArmy()) {
        while (head != NULL) {
            Position current_pos = head->data->getCurrentPosition();
            double distance = current_pos.distance(current_pos, this->pos);
            if (head->data->isInfantry()) {
                if (distance <= 2.0) {
                    int current_score = head->data->getAttackScore(),
                        new_score = ceil(0.8 * current_score),
                        quantity = head->data->get_quantity(),
                        new_weight = ceil(1.0 * (new_score - (int)head->data->getInfantryType() * 56) / quantity);
                    head->data->set_weight(new_weight);
                }
            }
            else {
                if (distance <= 2.0) {
                    int current_score = head->data->getAttackScore(),
                        new_score = ceil(0.8 * current_score),
                        quantity = head->data->get_quantity(),
                        new_weight = ceil(1.0 * (new_score * 30 - (int)head->data->getInfantryType() * 304) / quantity);
                    head->data->set_weight(new_weight);
                }
            }
            head = head->next;
        }
    }
    else {
        while (head != NULL) {
            Position current_pos = head->data->getCurrentPosition();
            double distance = current_pos.distance(current_pos, this->pos);
            if (head->data->isInfantry()) {
                if (distance <= 2.0) {
                    int current_score = head->data->getAttackScore(),
                        new_score = ceil(1.2 * current_score),
                        quantity = head->data->get_quantity(),
                        new_weight = ceil(1.0 * (new_score - (int)head->data->getInfantryType() * 56) / quantity);
                    head->data->set_weight(new_weight);
                }
            }
            else {
                if (distance <= 2.0) {
                    int current_score = head->data->getAttackScore(),
                        new_score = ceil(1.2 * current_score),
                        quantity = head->data->get_quantity(),
                        new_weight = ceil(1.0 * (new_score * 30 - (int)head->data->getInfantryType() * 304) / quantity);
                    head->data->set_weight(new_weight);
                }
            }
            head = head->next;
        }
    }
    army->make();
}
Position Fortification::get_pos() const {
    return pos;
}
string Fortification::str() const {
    return "Fortification";
}

void SpecialZone::getEffect(Army* army) {
    UnitList* unitList = army->get_unitList();
    if (!unitList || unitList->get_head() == nullptr) return;
    Node* head = unitList->get_head();
    while (head != NULL) {
        Position current_pos = head->data->getCurrentPosition();
        double distance = current_pos.distance(current_pos, this->pos);
        if (distance <= 1.0) {
            head->data->set_weight(0);
        }
        head = head->next;
    }
}
Position SpecialZone::get_pos() const {
    return pos;
}
string SpecialZone::str() const {
    return "SpecialZone";
}
// task 3.8
BattleField::BattleField(int n_rows, int n_cols, vector<Position*> arrayForest,
    vector<Position*> arrayRiver, vector<Position*> arrayFortification,
    vector<Position*> arrayUrban, vector<Position*> arraySpecialZone) {
    this->n_cols = n_cols;
    this->n_rows = n_rows;
    terrain = new TerrainElement * *[n_rows];
    for (int i = 0; i < n_rows; i++) {
        terrain[i] = new TerrainElement * [n_cols];
        for (int j = 0; j < n_cols; j++) {
            terrain[i][j] = nullptr;  // Initialize to nullptr
        }
    }

    for (Position* x : arrayForest) {
        int r = x->getRow();
        int c = x->getCol();
        terrain[r][c] = new Mountain();
    }
    for (Position* x : arrayRiver) {
        int r = x->getRow();
        int c = x->getCol();
        terrain[r][c] = new River();
    }
    for (Position* x : arrayFortification) {
        int r = x->getRow();
        int c = x->getCol();
        terrain[r][c] = new Fortification();
    }
    for (Position* x : arrayUrban) {
        int r = x->getRow();
        int c = x->getCol();
        terrain[r][c] = new Urban();
    }
    for (Position* x : arraySpecialZone) {
        int r = x->getRow();
        int c = x->getCol();
        terrain[r][c] = new SpecialZone();
    }

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            if (terrain[i][j] == nullptr) {
                terrain[i][j] = new Road();
            }
        }
    }
}
BattleField :: ~BattleField() {
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            if (terrain[i][j]) delete terrain[i][j];
        }
        delete[] terrain[i];
    }
    delete[]terrain;
}
string BattleField::str() {
    return "BattleField[n_rows=" + to_string(n_rows) + ",n_cols=" + to_string(n_cols) + "]";
}
int BattleField::getRow() {
    return this->n_rows;
}
int BattleField::getCols() {
    return this->n_cols;
}
TerrainElement*** BattleField::getTerrain() {
    return this->terrain;
}

//task 3.9
Configuration::Configuration(const string& filepath) {
    ifstream in(filepath);
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            int pos = line.find('=');
            string row = string(line.begin() + pos + 1, line.end());
            if (line.find("NUM_ROWS") != string::npos) {
                this->num_rows = stoi(row);
            }
            else if (line.find("NUM_COLS") != string::npos) {
                this->num_cols = stoi(row);
            }
            else if (line.find("UNIT_LIST") != string::npos) {
                for (int i = 0; i < row.size(); i++) {
                    if (!isdigit(row[i]) && !isalpha(row[i])) {
                        row[i] = ' ';
                    }
                }
                string type, quantity, weight, r, c, armyBelong;
                stringstream ss(row);
                vector <Unit*> v;
                while (ss >> type >> quantity >> weight >> r >> c >> armyBelong) {
                    Position pos(stoi(r), stoi(c));
                    if (type == "TANK" || type == "ARTILLERY" ||
                        type == "APC" || type == "ARMOREDCAR" ||
                        type == "ANTIAIRCRAFT" || type == "MORTAR" || type == "TRUCK") {
                        Unit* unit = new Vehicle(stoi(quantity), stoi(weight), pos, string_to_enumV(type));
                        if (armyBelong == "0") {
                            liberationUnits.push_back(unit);
                        }
                        else {
                            ARVNUnits.push_back(unit);
                        }
                    }
                    else {
                        Unit* unit = new Infantry(stoi(quantity), stoi(weight), pos, string_to_enumI(type));
                        if (armyBelong == "0") {
                            liberationUnits.push_back(unit);
                        }
                        else {
                            ARVNUnits.push_back(unit);
                        }
                    }
                }
            }
            else if (line.find("EVENT_CODE") != string::npos) {
                if (stoi(row) > 99) row = string(row.begin() + row.size() - 2, row.end());
                else if (stoi(row) < 0) row = "00";
                this->eventCode = stoi(row);
            }
            else {
                string res = "";
                for (char& x : row) {
                    if (isdigit(x)) {
                        res += x;
                    }
                }
                stringstream ss(res);
                pair <char, char> p;
                while (ss >> p.first >> p.second) {
                    int r = p.first - '0';
                    int c = p.second - '0';
                    Position* take = new Position(r, c);
                    if (line.find("FOREST") != string::npos) arrayForest.push_back(take);
                    else if (line.find("RIVER") != string::npos) arrayRiver.push_back(take);
                    else if (line.find("FORTIFICATION") != string::npos) arrayFortification.push_back(take);
                    else if (line.find("URBAN") != string::npos) arrayUrban.push_back(take);
                    else if (line.find("SPECIAL_ZONE") != string::npos) arraySpecialZone.push_back(take);
                }
            }

        }
        in.close();
    }
}
Configuration :: ~Configuration() {
    for (Position* x : arrayForest) delete x;
    arrayForest.clear();

    for (Position* x : arrayRiver) delete x;
    arrayRiver.clear();

    for (Position* x : arrayFortification) delete x;
    arrayFortification.clear();

    for (Position* x : arrayUrban) delete x;
    arrayUrban.clear();

    for (Position* x : arraySpecialZone) delete x;
    arraySpecialZone.clear();

    for (Unit* x : liberationUnits) delete x;
    liberationUnits.clear();

    for (Unit* x : ARVNUnits) delete x;
    ARVNUnits.clear();
}
InfantryType Configuration::string_to_enumI(string type) {
    if (type == "SNIPER") return SNIPER;
    if (type == "ANTIAIRCRAFTSQUAD") return ANTIAIRCRAFTSQUAD;
    if (type == "MORTARSQUAD") return MORTARSQUAD;
    if (type == "ENGINEER") return ENGINEER;
    if (type == "SPECIALFORCES") return SPECIALFORCES;
    return REGULARINFANTRY;
}
VehicleType Configuration::string_to_enumV(string type) {
    if (type == "TRUCK") return TRUCK;
    if (type == "MORTAR") return MORTAR;
    if (type == "ANTIAIRCRAFT") return ANTIAIRCRAFT;
    if (type == "ARMOREDCAR") return ARMOREDCAR;
    if (type == "APC") return APC;
    if (type == "ARTILLERY") return ARTILLERY;
    return TANK;
}
string Configuration::str() const {
    string result = "[num_rows=" + to_string(this->num_rows) + ",num_cols=" + to_string(this->num_cols) + ",";
    auto chuoi = [](vector <Position*> v) {
        string res = "";
        for (int i = 0; i < v.size(); i++) {
            res += v[i]->str();
            if (i < v.size() - 1) res += ',';
        }
        return res;
        };
    result += "arrayForest=[" + chuoi(arrayForest) + "]," +
        "arrayRiver=[" + chuoi(arrayRiver) + "]," +
        "arrayFortification=[" + chuoi(arrayFortification) + "]," +
        "arrayUrban=[" + chuoi(arrayUrban) + "]," +
        "arraySpecialZone=[" + chuoi(arraySpecialZone) + "],";

    auto chuoi2 = [](vector <Unit*> v) {
        string res = "";
        for (int i = 0; i < v.size(); i++) {
            res += v[i]->str();
            if (i < v.size() - 1) res += ',';
        }
        return res;
        };

    result += "liberationUnits=[" + chuoi2(liberationUnits) + "],ARVNUnits=[" + chuoi2(ARVNUnits) + "],eventCode=" + to_string(eventCode) + ']';
    return result;
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
const vector<Position*>& Configuration::getArrayForest() const {
    return arrayForest;
}
const vector<Position*>& Configuration::getArrayRiver() const {
    return arrayRiver;
}
const vector<Position*>& Configuration::getArrayFortification() const {
    return arrayFortification;
}
const vector<Position*>& Configuration::getArrayUrban() const {
    return arrayUrban;
}
const vector<Position*>& Configuration::getArraySpecialZone() const {
    return arraySpecialZone;
}
const vector<Unit*>& Configuration::getLiberationUnits() const {
    return liberationUnits;
}
const vector<Unit*>& Configuration::getARVNUnits() const {
    return ARVNUnits;
}

// task 3.10
HCMCampaign::HCMCampaign(const string& config_file_path) {
    // ifstream in(config_file_path);
    // if (in.is_open()){
    this->config = new Configuration(config_file_path);
    int n_rows = config->getNumRows();
    int n_cols = config->getNumCols();
    vector<Position*>  arrayForest = config->getArrayForest(),
        arrayRiver = config->getArrayRiver(),
        arrayFortification = config->getArrayFortification(),
        arrayUrban = config->getArrayUrban(),
        arraySpecialZone = config->getArraySpecialZone();
    this->battleField = new BattleField(n_rows, n_cols,
        arrayForest,
        arrayRiver,
        arrayFortification,
        arrayUrban,
        arraySpecialZone);

    auto convert = [](vector <Unit*> v) {
        auto* x = new Unit * [v.size()];
        for (int i = 0; i < v.size(); i++) {
            x[i] = v[i];
        }
        return x;
        };

    Unit** liberArray = convert(config->getLiberationUnits());
    int size1 = config->getLiberationUnits().size();
    string name1 = "LiberationArmy";
    this->liberationArmy = new LiberationArmy(liberArray, size1, name1, this->battleField);

    Unit** ArvnArray = convert(config->getARVNUnits());
    int size2 = config->getARVNUnits().size();
    string name2 = "ARVN";
    this->arvn = new ARVN(ArvnArray, size2, name2, this->battleField);
}
void HCMCampaign::run() {
    int r = this->battleField->getRow();
    int c = this->battleField->getCols();
    TerrainElement*** terrain = this->battleField->getTerrain();
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            terrain[i][j]->getEffect(this->liberationArmy);
            terrain[i][j]->getEffect(this->arvn);
        }
    }

    liberationArmy->make();
    arvn->make();

    int eventCode = this->config->getEventCode();
    if (eventCode < 75) {
        liberationArmy->fight(arvn, false);
        arvn->fight(liberationArmy, true);
        this->arvn->make();
        this->liberationArmy->make();
    }
    else {
        arvn->fight(liberationArmy, false);
        liberationArmy->fight(arvn, true);

        liberationArmy->make();
        arvn->make();

        liberationArmy->fight(arvn, false);
        arvn->make();
        liberationArmy->make();
    }

    Node* head1 = (liberationArmy->get_unitList()) ? liberationArmy->get_unitList()->get_head() : nullptr;
    Node* prev = nullptr;
    while (head1) {
        if (head1->data->getAttackScore() <= 5) {
            if (!prev) {
                Node* xoa = head1;
                head1 = head1->next;
                delete xoa;
                continue;
            }
            else {
                Node* xoa = head1;
                prev->next = head1->next;
                head1 = head1->next;
                delete xoa;
                continue;
            }
        }
        prev = head1;
        head1 = head1->next;
    }

    Node* head2 = (arvn->get_unitList()) ? arvn->get_unitList()->get_head() : nullptr;
    Node* prev2 = nullptr;
    while (head2) {
        if (head2->data->getAttackScore() <= 5) {
            if (!prev2) {
                Node* xoa = head2;
                head2 = head2->next;
                delete xoa;
                continue;
            }
            else {
                Node* xoa = head2;
                prev2->next = head2->next;
                head2 = head2->next;
                delete xoa;
                continue;
            }
        }
        prev2 = head2;
        head2 = head2->next;
    }
    this->arvn->make();
    this->liberationArmy->make();
}
string HCMCampaign::printResult() {
    return "LIBERATIONARMY[LF=" + to_string(this->liberationArmy->get_LF()) +
        ",EXP=" + to_string(this->liberationArmy->get_EXP()) +
        "]-ARVN[LF=" + to_string(this->arvn->get_LF()) +
        ",EXP=" + to_string(this->arvn->get_EXP()) + "]";
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////