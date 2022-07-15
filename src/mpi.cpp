#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "digit_array.hpp"
#include <mpi.h>

#define QUEUE_INIT 1
#define QUEUE_DATA 2
#define QUEUE_RESULT 3

#define MSG_KILL -9

bool isPrime(DigitArray number, int workerCount) {
    int result = 3;
    DigitArray checker = DigitArray(2);
    
    // Init processes
    int val = number.toInt();
    for(int i = 1; i < workerCount; ++i) {
        MPI_Send (&val, 1, MPI_INT, i, QUEUE_INIT, MPI_COMM_WORLD);
    }

    // Send first messages
    for (int i = 1; i < workerCount; ++i) {
        int val = checker.toInt();
        MPI_Send (&val, 1, MPI_INT, i, QUEUE_DATA, MPI_COMM_WORLD);
        ++checker;
	}

    // Receive and send until not prime or exceeded
    while(true) {
        int value;
        MPI_Status status;
        MPI_Recv (&value, 1, MPI_INT, MPI_ANY_SOURCE, QUEUE_RESULT, MPI_COMM_WORLD, &status);

        int val = checker.toInt();
        MPI_Send (&val, 1, MPI_INT, status.MPI_SOURCE, QUEUE_DATA, MPI_COMM_WORLD);
        ++checker;

        if(value < result) {
            result = value;
            break;
        }
    }

    // Receive last messages
    for (int i = 1; i < workerCount; ++i) {
        int value;
        MPI_Recv (&value, 1, MPI_INT, MPI_ANY_SOURCE, QUEUE_RESULT, MPI_COMM_WORLD, nullptr);
        if(value < result) {
            result = value;
        }
	}

    // Kill workers
    for (int i = 1; i < workerCount; ++i) {
        int val = MSG_KILL;
        MPI_Send (&val, 1, MPI_INT, i, QUEUE_DATA, MPI_COMM_WORLD);
	}

    return result == 2;
}

void worker(int id) {
    int value;
    MPI_Recv (&value, 1, MPI_INT, MPI_ANY_SOURCE, QUEUE_INIT, MPI_COMM_WORLD, nullptr);
    
    DigitArray zero = DigitArray(0);
    DigitArray number = DigitArray(value);

    while (true)
    {
        int response = 3;
        int value;
        MPI_Recv (&value, 1, MPI_INT, MPI_ANY_SOURCE, QUEUE_DATA, MPI_COMM_WORLD, nullptr);

        if(value == MSG_KILL) {
            return;
        }

        DigitArray checker = DigitArray(value);

        // Check if not exceeded
        DigitArray* b = new DigitArray(checker);
        if(*(++(*b)) > number) {
            response = 2;
        } 
        // Check if is divider only when this thread didn't check it
        else if (*(number % checker) == zero) {
            response = 1;
        }

        delete b; 

        MPI_Send(&response, 1, MPI_INT, 0, QUEUE_RESULT, MPI_COMM_WORLD);
    }
    
}


int main (int argc, char *argv[]) {
    int rank, size;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    
    if(argc != 2) {
        std::cout << "Provide exactly one argument" << std::endl;
        return 1;
    }

    DigitArray number = DigitArray(argv[1]);
    
    if (rank == 0) {
        bool numberIsPrime = isPrime(number, size);
        std::cout << "Number " << number.toString() << " is " << (numberIsPrime  ? "" : "not ") << "prime." << std::endl;
    }
    else {
        worker(rank);
    }
    MPI_Finalize();

    return 0;
}