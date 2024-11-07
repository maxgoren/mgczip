#ifndef bitstream_hpp
#define bitstream_hpp
#include <iostream>
#include <vector>
using namespace std;

class BitStream {
    private:
        std::vector<unsigned char> buffer;
        int bitpos;
        int bitcount;
    public:
        BitStream() : bitpos(0), bitcount(0) {

        }
        BitStream(const BitStream& bs) {
            bitpos = bs.bitpos;
            bitcount = bs.bitcount;
            buffer = bs.buffer;
        }
        int size() {
            return bitcount;
        }
        bool readBit() {
            int byteIndex = bitpos / 8;
            int bitIndex = bitpos % 8;
            if (byteIndex > buffer.size()) {
                cout<<"End of stream."<<endl;
                return false;
            }
            bool bit = (buffer[byteIndex] & (1 << bitIndex)) != 0;
            bitpos++;
            return bit;
        }
        char readChar() {
            char value = 0;
            for (int i = 7; i >= 0; --i) {
                value |= (readBit() << i);
            }
            return value;
        }
        int readInt() {
            int value = 0;
            for (int i = 7; i >= 0; --i) {
                value |= (readBit() << i);
            }
            return value;
        }
        void writeBit(bool bit) {
            int byteIndex = bitpos / 8;
            int bitIndex = bitpos % 8;
            if (byteIndex >= buffer.size())
                buffer.push_back(0);
            if (bit)
                buffer[byteIndex] |= (1 << bitIndex);
            bitpos++;
            bitcount++;
        }
        void writeInt(int value, int numBits) {
            for (int i = numBits - 1; i >= 0; --i) {
                writeBit((value >> i) & 1);
            }
        }
        void writeChar(char value, int numBits) {
            for (int i = numBits - 1; i >= 0; --i) {
                writeBit((value >> i) & 1);
            }
        }
        const std::vector<unsigned char>& getBuffer() const {
            return buffer;
        }
        bool done() {
            return bitpos < bitcount;
        }
        int offset() {
            return bitpos;
        }
        void start() {
            bitpos = 0;
        }
        void flush() {
            buffer.clear();
        }
        void print() {
            start();
            int i = 0;
            while (i++ < size())
                cout<<(readBit() ? 1:0);
            cout<<endl;
        }
        BitStream operator=(const BitStream& bs) {
            bitpos = bs.bitpos;
            bitcount = bs.bitcount;
            buffer = bs.buffer;
            return *this;
        }
};


#endif