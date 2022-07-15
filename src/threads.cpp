#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <atomic>
#include "digit_array.hpp"


std::mutex checkerLock;
std::mutex responseLock;

void verifyPrime(DigitArray* number, DigitArray* checker, int* response, std::atomic<bool>* active) {
    DigitArray zero = DigitArray(0);
    DigitArray* localChecker;

    checkerLock.lock();
    localChecker = new DigitArray(*checker);
    ++(*checker);
    checkerLock.unlock();

    // Check if not exceeded
    DigitArray* b = new DigitArray(*localChecker);
    if(*(++(*b)) > *number) {
        // Set is prime only when other thread didn't set it isn't
        responseLock.lock();
        if(*response == 0) {
            *response = 1;
        }
        responseLock.unlock();
    } 
    // Check if is divider only when this thread didn't check it
    else if (*(*number % *localChecker) == zero) {
        responseLock.lock();
        *response = 2;
        responseLock.unlock();
    }

    delete b; 
    delete localChecker;

    *active = false;
}

bool isPrime(DigitArray number) {
    int threadCount = 16;
    DigitArray checker = DigitArray(2);
    int response = 0;

    std::thread threads[threadCount];
    std::atomic<bool> threadsActive[threadCount];

    // Initialize threads
    for (int i = 0; i < threadCount; ++i) {
        threadsActive[i] = true; 
        std::atomic<bool>* threadActive = &threadsActive[i];
		threads[i] = std::thread([&number, &checker, &response, &threadActive] {verifyPrime(&number, &checker, &response, threadActive);});
	}

    // Join and run next threads
    while(true) {
        responseLock.lock();
        if(response > 0) {
            responseLock.unlock();
            break;
        }
        responseLock.unlock();
        for (int i = 0; i < threadCount; ++i) {
            if(threadsActive[i] == false) {
                threads[i].join();
                threadsActive[i] = true;
                std::atomic<bool>* threadActive = &threadsActive[i];
		        threads[i] = std::thread([&number, &checker, &response, &threadActive] {verifyPrime(&number, &checker, &response, threadActive);});
            }
            usleep(1000); // Need to slow down this check loop because it uses lock and consumes all resources
        }
    }

    // Finish all threads
    for (int i = 0; i < threadCount; ++i) {
        threads[i].join();
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