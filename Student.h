// The following code is the base code given to me by my professor to run my program
// Please note that it is not mine

#pragma once
// DO NOT MODIFY THIS FILE
#include <string>

using namespace std;

// Class for an immutable Student object
class Student
{
private:
	int m_id;
	string m_name;

public:
	Student(int studentID, const char* studentName);
	Student(int studentID, string& studentName);
	Student(Student& copyMe);

	int GetID();
	int GetKey();
	string GetName();
	string ToString();
};

