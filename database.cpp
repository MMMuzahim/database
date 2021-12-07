#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
    Date (string s) {
        istringstream ss(s);
        ss >> year;
        char c;
        ss >> c;
        if (c != '-') {
            throw invalid_argument("Wrong date format: " + s);
        }
        ss >> month;
        ss >> c;
        if (c != '-') {
            throw invalid_argument("Wrong date format: " + s);
        }
        if (ss.peek() == -1) {
            throw invalid_argument("Wrong date format: " + s);
        }
        ss >> day;
        if ((month < 1) || (month > 12)) {
            throw invalid_argument("Month value is invalid: " + to_string(month));
        }
        if ((day < 1) || (day > 31)) {
            throw invalid_argument("Day value is invalid: " + to_string(day));
        }
        if (!ss.eof()) {
            throw invalid_argument("Wrong date format: " + s);
        }
    }

    int GetYear() const { return year; }
    int GetMonth() const { return month; }
    int GetDay() const {return day; };

private:
    int year;
    int month;
    int day;
};

ostream& operator<< (ostream& stream, const Date& d) {
    stream << setfill('0');
    stream << setw(4) << d.GetYear() << '-'
           << setw(2) << d.GetMonth() << '-'
           << setw(2) << d.GetDay();
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() < rhs.GetYear()) {
        return true;
    }
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() < rhs.GetMonth()) {
            return true;
        }
    }
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            if (lhs.GetDay() < rhs.GetDay()) {
                return true;
            }
        }
    }
    return false;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        data[date].insert(event);
        //sort(data[date].begin(), data[date].end());
    }

    bool DeleteEvent(const Date& date, const string& event) {
        if (!data.count(date)) {
            return false;
        }
        if (data[date].count(event)) {
            data[date].erase(event);
            return true;
        }
        return false;
    }
    
    int  DeleteDate(const Date& date) {
        int n = 0;
        if (data.count(date)) {
            n = data[date].size();
            data.erase(date);
        }
        return n;
    }

    vector<string> Find(const Date& date) const {
        vector<string> r;
        for (const string & e : data.at(date)){
            r.push_back(e);
        }
        return r;
    }
    
    void Print() const {
        for (const auto& d : data) {
            for (const string& event : d.second) {
                cout << d.first << ' ' << event << endl;
            }
        }
    }

private:
    map<Date, set<string>> data;
};

int main() {
    Database db;
    
    string command;
    while (getline(cin, command)) {
        istringstream ss(command);
        string request;
        ss >> request;
        if (request == "Print") {
            db.Print();
        } else if (request == "Find") {
            try {
                string s_date;
                ss >> s_date;
                Date date(s_date);
                vector<string> d = db.Find(date);
                for (const string& s : d) {
                    cout << s << endl;
                }
            } catch (invalid_argument& ex) {
                cout << ex.what() << endl;
                return -1;
            } catch (out_of_range&) {}
        } else if (request == "Del") {
            try {
                string s_date;
                ss >> s_date;
                Date date(s_date);
                if (ss.eof()) {
                    cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
                } else {
                    string event;
                    ss >> event;
                    if (db.DeleteEvent(date, event)) {
                        cout << "Deleted successfully" << endl;
                    } else {
                        cout << "Event not found" << endl;
                    }
                }
            } catch (invalid_argument& ex) {
                cout << ex.what() << endl;
                return -1;
            }
        } else if (request == "Add") {
            try {
                string s_date;
                ss >> s_date;
                Date date(s_date);
                string event;
                ss >> event;
                db.AddEvent(date, event);
            } catch (invalid_argument& ex) {
                cout << ex.what() << endl;
                return -1;
            }
        } else {
            if (request != "") {
                cout << "Unknown command: " << request << endl;
                return -1;
            }
        }
    }
    return 0;
}
