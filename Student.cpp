// The following code is the base code given to me by my professor to run my program
// Please note that it is not mine

#include "Student.h"
// DO NOT MODIFY THIS FILE
Student::Student(int studentID, const char* studentName)
{
	m_id = studentID;
	m_name = studentName;
}

Student::Student(int studentID, string& studentName)
{
	m_id = studentID;
	m_name = studentName;
}

Student::Student(Student& copyMe)
{
	m_id = copyMe.m_id;
	m_name = copyMe.m_name;
}

int Student::GetID()
{
	return m_id;
}

// The "key" for an object is its unique identifier. In this case the student ID 
// is used as the key. Two students could potentially have the same name, but all 
// should have unique IDs.
int Student::GetKey()
{
	return m_id;
}

string Student::GetName()
{
	return m_name;
}

string Student::ToString()
{
	char* buf = new char[m_name.length() + 128];
	sprintf(buf, "Name=\"%s\", ID=%d", m_name.c_str(), m_id);
	string theString = buf;
	delete[] buf;
	return theString;
}
