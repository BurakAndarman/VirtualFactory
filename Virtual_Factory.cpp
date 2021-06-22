#include <iostream>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
using namespace std;

class Order {
public:
	int code,meters,oper,date;

    Order(int code,int meters,int oper,int date) {
		this->code = code;
		this->meters = meters;
		this->oper = oper;
		this->date = date;
	}

	Order() {
		this->code = 0;
		this->meters = 0;
		this->oper = 0;
		this->date = 0;
	}

	int giveCompletion(map<int, int> operation) {
		auto itr = operation.find(this->oper);
		float completion = static_cast<float>(this->meters) / static_cast<float>(itr->second);
		return round(completion);
	}
};

void createOperations(string fileName, map<int, int>& oper) {
	int operation, speed;
	string temp;
	string line;

	ifstream fileStream(fileName);

	if (fileStream.is_open()) {

		while (getline(fileStream, line)) {

			stringstream stringStream(line);
			getline(stringStream, temp,';');
			operation = stoi(temp);
			getline(stringStream, temp, '\n');
			speed = stoi(temp);

			oper.insert({ operation, speed });
		}
	}
	else {
		cout << "File couldn't open"<<endl;
	}

	fileStream.close();
}

void createSetupDurations(string fileName, map<int,map<int, int>>& setupDurations) {
	int oper1, oper2, duration;
	string temp;
	string line;

	ifstream fileStream(fileName);

	if (fileStream.is_open()) {

		while (getline(fileStream, line)) {

			stringstream stringStream(line);
			getline(stringStream, temp, ';');
			oper1 = stoi(temp);
			getline(stringStream, temp, ';');
			oper2 = stoi(temp);
			getline(stringStream, temp, '\n');
			duration = stoi(temp);

			setupDurations.insert({ oper1, map<int, int>() });
			setupDurations[oper1].insert({ oper2,duration });
		}
	}
	else {
		cout << "File couldn't open" << endl;
	}

	fileStream.close();

}

void createOrders(string fileName, list<Order>& orders) {
	int code, meters, oper, date = 0;
	string temp;
	string line;

	ifstream fileStream(fileName);

	if (fileStream.is_open()) {

		while (getline(fileStream, line)) {

			stringstream stringStream(line);
			getline(stringStream, temp, ';');
			code = stoi(temp);
			getline(stringStream, temp, ';');
			meters = stoi(temp);
			getline(stringStream, temp, ';');
			oper = stoi(temp);
			getline(stringStream, temp, '\n');
			date = stoi(temp);

			Order dummy(code, meters, oper, date);

			orders.push_back(dummy);
		}
	}
	else {
		cout << "File couldn't open" << endl;
	}

	fileStream.close();
}

void createSchedule(string fileName, int orderCode, int operationCode,int scheduleTime, int amountOfWork, int setupOverHead) {
	ofstream outStream(fileName, ios::app);

	if (outStream.is_open()) {
		outStream << scheduleTime<< ';' <<operationCode<< ';' <<orderCode<< ';' <<amountOfWork<< ';' <<setupOverHead<< '\n';
	}
	else {
		cout << "File couldn't open or create" << endl;
	}

	outStream.close();
}

int giveDuration(int oper1, int oper2, map<int,map<int,int>> setupDurations) {
	int duration=0;

	if (oper1 == oper2) {
		return duration;
	}

	for (auto iter = setupDurations.begin(); iter != setupDurations.end(); iter++) {
		for (auto iter_inner = iter->second.begin(); iter_inner != iter->second.end(); iter_inner++) {
			if (iter->first == oper1 && iter_inner->first == oper2) {
				duration = iter_inner->second;
			}
			else if (iter->first == oper2 && iter_inner->first == oper1) {
				duration = iter_inner->second;
			}
		}
	}

	return duration;
}

multimap<int,int> computeEfficienyOrder(int oper,map<int, map<int, int>>setupDurations, map<int, int>operations) {
	int efficiency ;
	multimap<int,int> efficiencyOrder;

	if (oper == 0) {
		for (auto itr = operations.begin(); itr != operations.end(); itr++) {
			efficiency = 0;
			for (auto iter = operations.begin(); iter != operations.end(); iter++) {
				if (itr->first != iter->first) {
					efficiency += giveDuration(itr->first, iter->first, setupDurations);
				}
			}
			efficiencyOrder.insert({ efficiency, itr->first });
		}
	}
	else {
		for (auto itr = operations.begin(); itr != operations.end(); itr++) {
			efficiency = 0;
			efficiency = giveDuration(itr->first, oper, setupDurations);
			efficiencyOrder.insert({ efficiency, itr->first });
			
		}
	}


	return efficiencyOrder;
}




int main()
{
	map<int,int>operations;
	createOperations("Operations.txt", operations);

	map<int, map<int, int>>setupDurations;
	createSetupDurations("SetupDuration.txt", setupDurations);

	list<Order> orders;
	createOrders("Orders.txt", orders);


	int setupOverHead = 0;
	int prev_oper = 0;
	int completion = 0;
	int prev_timer = 0;
	int timer = 0;

	list<Order>::iterator iter;
	Order order;
	int bound = orders.size();


	multimap<int,int>efficiencyOrder = computeEfficienyOrder(0,setupDurations, operations);
	list<Order>orderGroup;
	bool flag = false;


	for (int i = 0; i < bound; i++) {
		flag = false;

		if (i != 0) {
			createSchedule("Schedule.txt", order.code, order.oper, prev_timer, order.meters, setupOverHead);
			setupOverHead = 0;
			efficiencyOrder = computeEfficienyOrder(order.oper, setupDurations, operations);
			Order order;
		}

		if (i==0) {

			for (auto k = efficiencyOrder.rbegin(); k != efficiencyOrder.rend(); k++) {

				orderGroup.clear();


				for (auto j = orders.begin(); j != orders.end(); j++) {
					if (j->oper == k->second) {
						orderGroup.push_back(*(j));
					}
				}

				if (!orderGroup.empty()) {
					for (auto t = orderGroup.begin(); t != orderGroup.end(); t++) {
						flag = false;
						for (auto z = orders.begin(); z != orders.end(); z++) {
							if (t->code != z->code && 0 > (z->date - (z->giveCompletion(operations) + giveDuration(z->oper, t->oper, setupDurations) + t->giveCompletion(operations) + prev_timer))) {

								flag = true;
								break;
							}
							for (auto find = orders.begin(); find != orders.end(); find++) {
								if (find->code == t->code) {
									iter = find;
								}
							}
						}
						if (flag == false) {

							order = *(t);
							setupOverHead = giveDuration(prev_oper, order.oper, setupDurations);
							break;
						}
					}
					if (flag == false) {

						break;
					}
				}
			}
		}
		else {
			for (auto k = efficiencyOrder.begin(); k != efficiencyOrder.end(); k++) {

				orderGroup.clear();


				for (auto j = orders.begin(); j != orders.end(); j++) {
					if (j->oper == k->second) {
						orderGroup.push_back(*(j));
					}
				}

				if (!orderGroup.empty()) {
					for (auto t = orderGroup.begin(); t != orderGroup.end(); t++) {
						flag = false;
						for (auto z = orders.begin(); z != orders.end(); z++) {
							if (t->code != z->code && 0 > (z->date - (z->giveCompletion(operations) + giveDuration(z->oper, t->oper, setupDurations) + t->giveCompletion(operations) + prev_timer+ giveDuration(prev_oper, t->oper, setupDurations)))) {

								flag = true;
								break;
							}
							for (auto find = orders.begin(); find != orders.end(); find++) {
								if (find->code == t->code) {
									iter = find;
								}
							}
						}
						if (flag == false) {

							order = *(t);
							setupOverHead = giveDuration(prev_oper, order.oper, setupDurations);
							break;
						}
					}
					if (flag == false) {

						break;
					}
				}
			}
		}

	   	completion = order.giveCompletion(operations);

        prev_timer = timer + setupOverHead;
       	timer = timer+ completion + setupOverHead;
       	prev_oper = order.oper;
		orders.erase(iter);
		orderGroup.clear();
	}

	createSchedule("Schedule.txt", order.code, order.oper, prev_timer, order.meters, setupOverHead);

	return 0;

}

