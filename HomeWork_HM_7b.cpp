// HomeWork_HM_7b.cpp : Defines the entry point for the application.
//

#include "HomeWork_HM_7b.h"
#include "studentsGroupProto.pb.h"
#include <algorithm>
#include <tuple>
#include <fstream>

using namespace std;

class IRepository {
	virtual void Open() = 0; // бинарная десериализация в файл
	virtual void Save() = 0; // бинарная сериализация в файл
};

class IMethods {
	virtual double GetAverageScore(const studentsGroupProto::FullName & name) = 0;
	virtual string GetAllInfo(const studentsGroupProto::FullName & name) = 0;
	virtual string GetAllInfo() = 0;
};

class StudentsGroup : public IRepository, public IMethods {
private:
	studentsGroupProto::StudentsGroup _students;

	static const string& _GetStudentInfo(const studentsGroupProto::Student& student) {
		string result = ""s;
		result.append("Name: " + student.fullname().name() + " | ");
		result.append("Last name: " + student.fullname().lastname() + " | ");
		if (student.fullname().has_surname())
			result.append("Surname: " + student.fullname().surname() + " | ");
		result.append("Marks: [ ");
		for_each(student.marks().begin(), student.marks().end(), [&result](auto row) { result.append(to_string(row) + " "); });
		result.append("] | ");
		result.append("Average mark: " + to_string(student.averagemarks()) + " | ");
		return result;
	};
public:
	StudentsGroup() {};

	StudentsGroup(initializer_list<studentsGroupProto::Student> data) {
		for_each(data.begin(), data.end(), [&](const auto& element) {
			auto student = _students.add_students();
			*student = element;
		});
	};

	virtual double GetAverageScore(const studentsGroupProto::FullName& name) override {
		auto foundStudent = find_if(_students.students().begin(), _students.students().end(), [&name](const auto& element) {
			return tie(element.fullname().name(), element.fullname().surname(), element.fullname().lastname()) == tie(name.name(), name.surname(), name.lastname());
		});
		if (foundStudent == _students.students().end())
			return 0;
		return foundStudent->averagemarks();
	};

	virtual string GetAllInfo(const studentsGroupProto::FullName& name) override {
		string result = ""s;
		auto foundStudent = find_if(_students.students().begin(), _students.students().end(), [&name](const auto& element) {
				return tie(element.fullname().name(), element.fullname().surname(), element.fullname().lastname()) == tie(name.name(), name.surname(), name.lastname());
			});
		if (foundStudent == _students.students().end())
			return result;
		return _GetStudentInfo(*foundStudent);
	};

	virtual string GetAllInfo() override {
		string result = ""s;
		for_each(_students.students().begin(), _students.students().end(), [&result](const auto& element) {
			result.append(_GetStudentInfo(element));
			result.append("\n");
		});
		return result;
	};

	virtual void Open() override {
		ifstream ifs("studentsGroup.txt", std::ios_base::in | std::ios_base::binary);
		_students.ParseFromIstream(&ifs);
	};

	virtual void Save() override {
		ofstream ofs("studentsGroup.txt", std::ios_base::out | std::ios_base::binary);
		_students.SerializeToOstream(&ofs);
	};
};

int main()
{
	
	studentsGroupProto::Student student;
	auto studentName = student.fullname().New();
	studentName->set_name("John");
	studentName->set_lastname("Daw");
	studentName->set_surname("Johnovich");
	student.set_allocated_fullname(studentName);
	
	student.add_marks(1);
	student.add_marks(2);
	student.add_marks(3);
	student.add_marks(4);
	student.add_marks(5);
	student.set_averagemarks(3);

	StudentsGroup myGroup{ student };
	cout << myGroup.GetAllInfo() << endl;
	myGroup.Save();

	StudentsGroup fromFile;
	fromFile.Open();
	cout << fromFile.GetAllInfo() << endl;

	return 0;
}
