#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <mpi.h>
#include <functional>


/**
 * RPC implementation on top of MPI interface
 */
class RPC {
    private:
        int rank;
        int size;
        int (*callable)(int, int);
    public:

        RPC() {
            int a = 0;
            char **b;
            MPI_Init (&a, &b);
            MPI_Comm_rank (MPI_COMM_WORLD, &rank);
            MPI_Comm_size (MPI_COMM_WORLD, &size);
        }

        bool isWorker() {
            return rank != 0;
        }

        void runWorker(int (*callable)(int, int)) {
            this->callable = callable;

            if(isWorker()) {
                while (true) {
                    break;
                    // Listen for MPI calls and send response back
                }
                MPI_Finalize();
            }
        }

        int call(int a, int b) {
            // Call remote procedure (send MPI call and wait for response)
            return callable(a, b);
        }

        void stop() {
            // Stop workers (send stop message)
            MPI_Finalize();
        }
};