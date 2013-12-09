#include "task.h"

std::vector<std::string> Task::subject_list;

Task::Task(){
}

Task::Task(std::string &&task_name, std::string &&task_description, int task_priority, int task_subject, const time_t &task_deadline):
	name(std::move(task_name)),
	describe(std::move(task_description)),
	priority(task_priority),
	subject_num(task_subject) {
	joindate = time(NULL);
	deadline = task_deadline;
}

void Task::display() const {
	using std::cout;
	char dateBuf[11];

	cout 	<< priority << '\t'
		<< subject_list[subject_num] << '\t'
		<< name << '\t';

	strftime(dateBuf, 11, "%F", localtime(&joindate));
	cout << dateBuf << '\t';

	strftime(dateBuf, 11, "%F", localtime(&deadline));
	cout << dateBuf << '\t';

	cout << describe;
}

bool Task::compare_dead(const Task &lhs, const Task &rhs) {
	return lhs.deadline < rhs.deadline;
}

bool Task::compare_prio(const Task &lhs, const Task &rhs) {
	return lhs.priority > rhs.priority;
}

bool Task::compare_subj(const Task &lhs, const Task &rhs) {
	return lhs.subject_num < rhs.subject_num;
}

std::ostream &operator<<(std::ostream &os, const Task &task) {
	os 	<< task.name << ' '
		<< task.priority << ' '
		<< task.subject_num << ' '
		<< task.joindate << ' '
		<< task.deadline << ' '
		<< task.describe << '\n';

	return os;
}

std::istream &operator>>(std::istream &is, Task &task) {
	is 	>> task.name
		>> task.priority
		>> task.subject_num
		>> task.joindate
		>> task.deadline;

	getline(is, task.describe);
	return is;
}
