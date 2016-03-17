#include <iostream>
#include <string>
using namespace std;
class Date
{
public:
    Date() {}
    Date(int year,int moon,int day):year(year),moon(moon),day(day){}
    Date& operator=(const Date& d)
    {
        this->year = d.year;
        this->moon = d.moon;
        this->day = d.day;
        return *this;
    }

private:
    int year;
    int moon;
    int day;
};

class Student
{
public:
    Student(){}
    Student(string name,int age,Date birthday):name(name),age(age),birthday(birthday){}
    Student(const Student& st)
    {
        this->name = st.name;
        this->age = st.age;
        this->birthday = st.birthday;
    }
    ~Student(){}
    void setName(string name)
    {
        this->name = name;
    }
    string getNmae()
    {
        return this->name;
    }
    friend ostream& operator <<(ostream& cout,const Student& st)
    {
        cout<<st.name<<endl;
        return cout;
    }
    friend istream& operator >>(istream& cin,Student& st)
    {
        cin>>st.name;
        return cin;
    }
private:
    string name;
    int age;
    Date birthday;
};
