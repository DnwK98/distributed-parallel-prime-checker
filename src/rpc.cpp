#include "digit_array.cpp"
#include "rpc_engine.cpp"

bool isDivider(int dividend, int divider) {
    return *(DigitArray(dividend) % DigitArray(divider)) == DigitArray(0);
}

bool isPrime(DigitArray number, RPC rpc) {
    DigitArray checker = DigitArray(2);
    DigitArray zero = DigitArray(0);

    while(true) {
        // Check if not exceeded
        DigitArray* b = new DigitArray(checker);
        if(*(++(*b)) > number) {
            delete b;
            return true;
        }
        delete b;

        // Check if is divider
        if(rpc.call(number.toInt(), checker.toInt())) {
            return false;
        }
        ++checker;
    }
}

int main(int argc, char *argv[]) {
    RPC rpc = RPC();

    rpc.runWorker([](int a, int b) -> int {
        return isDivider(a, b);
    });

    if(rpc.isWorker()) {
        return 0;
    }

    if(argc != 2) {
        std::cout << "Provide exactly one argument" << std::endl;
        return 1;
    }

    DigitArray number = DigitArray(argv[1]);
    bool numberIsPrime = isPrime(number, rpc);
    std::cout << "Number " << number.toString() << " is " << (numberIsPrime  ? "" : "not ") << "prime." << std::endl;

    rpc.stop();
    return 0;
}