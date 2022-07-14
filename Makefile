sequential:
	mkdir -p target
	g++ src/sequential.cpp -o target/app
	./target/app $(number)