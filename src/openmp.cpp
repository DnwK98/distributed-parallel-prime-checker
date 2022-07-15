#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "digit_array.hpp"


bool isPrime(DigitArray number) {
    DigitArray checker = DigitArray(2);
    DigitArray zero = DigitArray(0);
    int response = 0;

    #pragma omp parallel
    while(response == 0) {
        DigitArray* localChecker;

        #pragma omp critical
        {
            localChecker = new DigitArray(checker);
            ++checker;
        }

        // Check if not exceeded
        DigitArray* b = new DigitArray(*localChecker);
        if(*(++(*b)) > number) {
            #pragma omp critical
            {
                // Set is prime only when other thread didn't set it isn't
                if(response == 0) {
                    response = 1;
                }
            }
        } 
        // Check if is divider only when this thread didn't check it
        else if (*(number % *localChecker) == zero) {
            #pragma omp critical
            {
                response = 2;
            }
        }

        delete b;
        delete localChecker;
    }

    return response == 1;
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