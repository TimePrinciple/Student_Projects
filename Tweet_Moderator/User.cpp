#include <string>
#include "User.h"
#include <map>
#include <utility>
#include <iostream>

using namespace std;

User::User(const string &n, int r) : Person::Person(n) {
    name = n;
    regNo = r;
    multimap<string, float> m;
    reputation_scores = m;
    accumulative_reputation_score = 0;
    average_reputation_score = 0;
}

int User::getRegNo() const {
    return regNo;
}

void User::addScore(float score, string twit) {
    reputation_scores.insert(pair<string, float>(twit, score));
}

bool User::updateAccumulativeScore(float score) {
    if (reputation_scores.empty()) {
        return false;
    }
    accumulative_reputation_score += score;
    average_reputation_score = accumulative_reputation_score / reputation_scores.size();
    return true;
}

ostream& operator<<(ostream &str, const User &s) {
    cout << "               Statistics" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "User: " << s.name << endl;
    cout << "Registration Number: " << s.regNo << endl;
    cout << "Accumulative Reputation Score: " << s.accumulative_reputation_score << endl;
    cout << "Average Reputation Score: " << s.average_reputation_score << endl;
    return str;
}
