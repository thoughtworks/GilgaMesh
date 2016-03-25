#include "GapTest.h"
#include "GattTest.h"

int main(void) {
    int retVal = 0;

    //call all test suites here
    retVal &= RunGapTest();
    retVal &= RunGattTest();

    return retVal;
}
