#include <iostream>
#include <string>

using namespace std;

class person {
    public:
    person(string init_name, int init_ssn) : 
        name(init_name),
        ssn(init_ssn) {}

    void printName() const { cout << getName() << endl; }
    virtual string getName() const { return name; } // virtual keyword so compiler uses overwritten functions

    private:
    string name;
    int ssn;
};

class student : public person {
    public:
    student(string init_name, int init_ssn, double init_gpa) :
        person(init_name, init_ssn),
        gpa(init_gpa) {}

    void printName() const { cout << getName() << endl; }
    string getName() const { return "Student name is " + person::getName(); } // virtual needed here if printName overwritten
    private:
    double gpa;
};

class csen_student : public student {
    public:
    csen_student(string init_name, int init_ssn, double init_gpa, string init_lang) :
        student(init_name, init_ssn, init_gpa),
        prog_lang(init_lang) {}

    string getName() const { return "CSEN student name is " + person::getName(); }

    private:
    string prog_lang;
};

int main() {
    person obj1("Mary Johnson", 1234);
    obj1.printName();                                       // Output: Mary Johnson

    student obj2("Daniel Jones", 4321, 2.6);
    obj2.printName();                                       // Output: student name is Daniel Jones

    csen_student obj3("Xavier Kuehn", 0, 4.0, "C++");
    obj3.printName();                                       // Output: CSEN student name is Xavier Kuehn
}