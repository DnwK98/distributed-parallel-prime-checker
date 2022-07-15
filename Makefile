sequential:
	mkdir -p target
	g++ src/sequential.cpp -o target/app
	./target/app $(number)

openmp:
	mkdir -p target
	g++ src/openmp.cpp -o target/app -fopenmp -lpthread
	./target/app $(number)

threads:
	mkdir -p target
	g++ src/threads.cpp -o target/app -pthread
	./target/app $(number)

mpi:
	mkdir -p target
	mpic++ src/mpi.cpp -o target/app 
	mpirun target/app $(number)