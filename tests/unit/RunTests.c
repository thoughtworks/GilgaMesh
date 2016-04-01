#include "GapTest.h"
#include "GattTest.h"
#include "ConnectionTest.h"

int main(int argc, char **argv) {
    int retVal = 0;

    //call all test suites here
    retVal &= RunGapTest();
    retVal &= RunGattTest();
    retVal &= RunConnectionTest();

    return retVal;
}
