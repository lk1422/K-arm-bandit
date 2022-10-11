#include <iostream>
#include "bandit.h"
#include "Slots.h"

using namespace std;

int main() {
    SlotMachine m(10, 11);
    Bandit b(m,.1, OPTIMISTIC);
    b.runBandit(60);
    return 0;
}
