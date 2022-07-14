#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

/**
 * TODO Correct implementation of ++operator, %operator, and comparasion
 *      to use array instead of int. 
 */
class DigitArray {
    private:
        int length;
        int* number;
    
    public:

        DigitArray(const DigitArray& other): DigitArray(other.toString()){}

        DigitArray(int val) {
            char* buffer = new char[512];

            std::sprintf(buffer, "%d", val);
            applyString(buffer);
        }

        DigitArray(const char* numberString){
            applyString(numberString);
        }

        ~DigitArray(){
            delete number;
        }

        char* toString() const {
            char* s = new char[length + 1];
            for(int i = 0; i< length; i++) s[i]= (number[i] + 48);
            s[length] = '\0';
            
            return s;
        }

        int toInt() const {
            int value = 0;
            for(int i = length - 1; i>=0; --i) {
                value += number[i] * std::pow(10, (length -1 - i));
            }

            return value;
        }

        DigitArray* operator++ () {
            int val = toInt();
            val++;

            char* buffer = new char[512];
            std::sprintf(buffer, "%d", val);

            applyString(buffer);

            return this;
        }

        bool operator== (const DigitArray& other) {
            return 0 == compare(other);
        }

        bool operator!= (const DigitArray& other) {
            return 0 != compare(other);
        }

        bool operator< (const DigitArray& other) {
            return compare(other) < 0;
        }

        bool operator<= (const DigitArray& other) {
            return compare(other) <= 0;
        }

        bool operator> (const DigitArray& other) {
            return compare(other) > 0;
        }

        bool operator>= (const DigitArray& other) {
            return compare(other) >= 0;
        }
        DigitArray* operator% (const DigitArray& other) {
            int rest = toInt() % other.toInt();
            
            return new DigitArray(rest);
        }

    private:
        int compare(const DigitArray& other) {
            return this->toInt() - other.toInt();
        }
        void applyString(const char* buffer) {
            length = 0;
            while(buffer[length] != '\0') ++length;
            number = new int[length];
            for(int i=0; i<length; ++i) {
                number[i] = int(buffer[i]) - 48;
            }
        }
};