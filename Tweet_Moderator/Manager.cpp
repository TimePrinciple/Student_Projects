#include "Manager.h"
#include <string>

using namespace std;

Manager::Manager(const string &n, int r) : Person::Person(n) {
    name = n;
    regNo = r;
}

int Manager::getRegNo() const {
    return regNo;
}
