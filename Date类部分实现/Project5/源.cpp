#include <iostream>
#include <ostream>
using namespace std;
class Date {
public:
    int GetMonthDay(int year, int month);
    Date(int year = 1900, int month = 1, int day = 1)
        : _year(year),
        _month(month),
        _day(day) {}
    Date(const Date& d) {
        _year = d._year;
        _month = d._month;
        _day = d._day;
    }
    Date& operator=(const Date& d);
    Date& operator+=(int day);
    Date operator+(int day);
    Date operator-(int day);
    Date& operator-=(int day);
    bool operator==(const Date& d);
    friend ostream& operator<<(ostream& out, const Date& d);

private:
    int _year;
    int _month;
    int _day;
};
int Date::GetMonthDay(int year, int month) {
    int m[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        return 29;
    }
    else {
        return m[month];
    }
}
Date& Date::operator=(const Date& d) {
    _year = d._year;
    _month = d._month;
    _day = d._day;
    return *this;
}
Date& Date::operator+=(int day) {
    if (day < 0) {
        *this -= (-day);
    }
    else {
        _day += day;
        while (_day >= GetMonthDay(_year, _month)) {
            _day -= GetMonthDay(_year, _month);
            _month += 1;
            if (_month > 12) {
                ++_year;
                _month -= 12;
            }
        }
    }
    return *this;
}
Date Date::operator+(int day) {
    Date d(*this);
    d += day;
    return d;
}
Date Date::operator-(int day) {
    Date d(*this);
    d -= day;
    return d;
}
Date& Date::operator-=(int day) {
    if (day < 0) {
        *this += (-day);
    }
    else {
        _day -= day;
        while (_day <= 0) {
            _month -= 1;
            if (_month <= 0) {
                --_year;
                _month += 12;
            }
            _day += GetMonthDay(_year, _month);
        }
    }
    return *this;
}
bool Date::operator==(const Date& d) {
    if (_year == d._year && _month == d._month && _day == d._day) {
        return true;
    }
    else {
        return false;
    }
}
ostream& operator<<(ostream& out, const Date& d) {
    out << d._year << '-' << d._month << '-' << d._day << endl;
    return out;
}
int main() {
    Date d1(2020, 2, 20);
    Date d2 = d1 - 10000;
    cout << d1;
    cout << d2;
    return 0;
}