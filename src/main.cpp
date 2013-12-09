#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cstdlib>
#include <ctime>
#include "task.h"
using namespace std;

void read_subject();
void addTask(int argc, char *argv[]);
void listTask(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Usage: ezSchedule [add|list|del|mod|help [Options]]\nUse 'ezSchedule help' to see availiable options." << endl;
		exit(1);
	}

	read_subject();

	string command(argv[1]);
	if(command == "add")
		addTask(argc - 2, argv + 2);
	else if(command == "list")
		listTask(argc - 2, argv + 2);
	else if(command == "del")
		;
	else if(command == "mod")
		;
	else if(command == "help")
		;
	else {
		cout << "Usage: ezSchedule [add|list|del|mod|help [Options]]" << endl;
		exit(1);
	}

	return 0;
}

inline void read_subject() {
	ifstream subject(string(getenv("HOME")) + "/.ezSchedule/subject.data", ios::in);
	if(!subject) {
		cout << string("Fail on opening ") + getenv("HOME") + "/.ezSchedule/subject.data" << endl;
		exit(-1);
	}

	string newSub;
	subject >> newSub;
	while(subject) {
		Task::subject_list.push_back(newSub);
		subject >> newSub;
	}

	subject.close();
}

void addTask(int argc, char *argv[]) {
	ofstream taskFile(string(getenv("HOME")) + "/.ezSchedule/task.data", ofstream::out | ofstream::app);
	if(!taskFile) {
		cout << string("Fail on opening ") + getenv("HOME") + "/.ezSchedule/task.data" << endl;
		exit(-1);
	}

	string name;
	string description;
	int priority = -9999;
	int subject = -9999;
	int year = -9999;
	int month = -9999;
	int day = -9999;

	while(argc > 0) {
		string option(*(argv++));
		if(option == "-n")
			name = *(argv++);
		else if(option == "-p")
			priority = atoi(*(argv++));
		else if(option == "-s")
			subject = atoi(*(argv++));
		else if(option == "-y")
			year = atoi(*(argv++));
		else if(option == "-m")
			month = atoi(*(argv++));
		else if(option == "-d")
			day = atoi(*(argv++));
		else {
			cout << "Invalid option flag.\nUse 'ezSchedule help' to see availiable options." << endl;
			exit(1);
		}
		argc -= 2;
	}

	cout << "新增一項事情\n";
	if(name.empty()) {
		cout << "名稱：";
		cin >> name;
	}
	if(priority == -9999) {
		cout << "重要性(-100 ~ 100)：";
		cin >> priority;
	}
	if(subject == -9999) {
		for(int i=0; i<Task::subject_list.size(); i++)
			cout << i << ":" << Task::subject_list[i] << "  ";
		cout << "\n科目(輸入科目的編號)：";
		cin >> subject;
	}
	if(year == -9999) {
		cout << "死線_年(2013)：";
		cin >> year;
	}
	if(month == -9999) {
		cout << "死線_月(12)：";
		cin >> month;
	}
	if(day == -9999) {
		cout << "死線_日(24)：";
		cin >> day;
	}
	cout << "敘述：";
	cin.ignore(256, '\n');
	getline(cin, description);

	struct tm deadline;
	deadline.tm_sec = deadline.tm_min = deadline.tm_hour = 0;
	deadline.tm_mday = day;
	deadline.tm_mon = month - 1;
	deadline.tm_year = year - 1900;

	Task newTask(move(name), move(description), priority, subject, mktime(&deadline));
	taskFile << newTask;
	taskFile.close();
}

void listTask(int argc, char *argv[]) {
	ifstream taskFile(string(getenv("HOME")) + "/.ezSchedule/task.data", ios::in);
	if(!taskFile) {
		cout << string("Fail on opening ") + getenv("HOME") + "/.ezSchedule/task.data" << endl;
		exit(-1);
	}

	cout 	<< "\033[1;41m優先值\t"
		<< "科目\t"
		<< "名稱\t\t"
		<< "建立日期\t"
		<< "死線\t\t"
		<< "敘述\033[m\n";

	list<Task> taskList;
	Task task;
	taskFile >> task;
	while(taskFile) {
		taskList.push_back(task);
		taskFile >> task;
	}
	taskFile.close();
	if(argc > 0) {
		if(string(argv[0]) == "-p")
			taskList.sort(Task::compare_prio);
		else if(string(argv[0]) == "-s")
			taskList.sort(Task::compare_subj);
		else if(string(argv[0]) == "-d")
			taskList.sort(Task::compare_dead);
		else {
			cout << "Invalid option flag.\nUse 'ezSchedule help' to see availiable options." << endl;
			exit(1);
		}
	}

	int color = 32;
	for(auto &t : taskList) {
		cout << "\033[" << (color ^= 1) << 'm';
		t.display();
		cout << "\033[m\n";
	}
}
