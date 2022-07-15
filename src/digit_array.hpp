#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

class DivisionByZeroException{};

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

        DigitArray* operator- (const DigitArray& other) const {
            if(*this < other) {
                return new DigitArray(-1);
            }
            DigitArray* tmp = new DigitArray(*this);
            int lengthDiff = tmp->length - other.length;
          
            for(int i = other.length - 1; i>=0; --i) {
                if(tmp->number[i + lengthDiff] < other.number[i]) {
                    tmp->number[i + lengthDiff] += 10;
                    tmp->number[i - 1 + lengthDiff] -= 1;
                }
                tmp->number[i + lengthDiff] -= other.number[i];
            }

            return tmp;
        }

        DigitArray* operator% (const DigitArray& other) {
            if(other == DigitArray(0)) {
                throw new DivisionByZeroException();
            }
            DigitArray* tmp = new DigitArray(*this);
            while(*tmp >= other) {
                tmp = *tmp - other;
            }
            
            return tmp;
        }

        bool operator== (const DigitArray& other) const {
            return 0 == compare(other);
        }

        bool operator!= (const DigitArray& other) const {
            return 0 != compare(other);
        }

        bool operator< (const DigitArray& other) const {
            return compare(other) < 0;
        }

        bool operator<= (const DigitArray& other) const {
            return compare(other) <= 0;
        }

        bool operator> (const DigitArray& other) const {
            return compare(other) > 0;
        }

        bool operator>= (const DigitArray& other) const {
            return compare(other) >= 0;
        }

    private:

        int compare(const DigitArray& other) const {
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