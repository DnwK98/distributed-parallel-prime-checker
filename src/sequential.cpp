#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "digit_array.hpp"


bool isPrime(DigitArray number) {
    DigitArray checker = DigitArray(2);
    DigitArray zero = DigitArray(0);

    while(true) {
        DigitArray* localChecker;

        // Critical
        {
            localChecker = new DigitArray(checker);
            ++checker;
        }

        // Check if not exceeded
        DigitArray* b = new DigitArray(*localChecker);
        if(*(++(*b)) > number) {
            delete b;
            delete localChecker;
            return true;
        }
        delete b;

        // Check if is divider
        if(*(number % *localChecker) == zero) {
            delete localChecker;
            return false;
        }

        delete localChecker;
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Provide exactly one argument" << std::endl;
        return 1;
    }

    DigitArray number = DigitArray(argv[1]);
    bool numberIsPrime = isPrime(number);
    std::cout << "Number " << number.toString() << " is " << (numberIsPrime  ? "" : "not ") << "prime." << std::endl;

    return 0;
}