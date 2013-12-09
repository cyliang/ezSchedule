#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include <vector>

class Task {
public:
	Task();
	Task(std::string &&task_name, std::string &&task_description, int task_priority, int task_subject, const time_t &task_deadline);
	void display() const;

	static bool compare_dead(const Task &, const Task &);
	static bool compare_prio(const Task &, const Task &);
	static bool compare_subj(const Task &, const Task &);

	friend std::ostream &operator<<(std::ostream &, const Task &);
	friend std::istream &operator>>(std::istream &, Task &);

	static std::vector<std::string> subject_list;
private:
	std::string name;
	std::string describe;
	int priority;
	int subject_num;
	time_t joindate;
	time_t deadline;
};
