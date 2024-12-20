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
        int getByteIdx();
        int getBitIdx();
        bool getBitAtX(int byteIndex, int bitIndex);
        void setBitAtX(int byteIndex, int bitIndex);
    public:
        BitStream(const string& str);
        BitStream();
        BitStream(const BitStream& bs);
        bool readBit();
        char readChar();
        int readInt();
        int readInt(int width);
        void writeBit(bool bit);
        void writeInt(int value, int numBits);
        void writeChar(char value, int numBits = 8);
        int size();
        BitStream& start();
        int offset();
        bool done();
        void flush();
        void print();
        BitStream operator=(const BitStream& bs);
};

BitStream::BitStream() : bitpos(0), bitcount(0) {

}

BitStream::BitStream(const BitStream& bs) {
    bitpos = bs.bitpos;
    bitcount = bs.bitcount;
    buffer = bs.buffer;
}

BitStream::BitStream(const string& str) {
    bitpos = 0;
    bitcount = 0;
    for (char c : str) {
        writeChar(c);
    }
}

bool BitStream::readBit() {
    int byteIndex = bitpos / 8;
    int bitIndex = bitpos % 8;
    if (byteIndex > buffer.size()) {
        cout<<"End of stream."<<endl;
        return false;
    }
    bool bit = getBitAtX(byteIndex, bitIndex);
    bitpos++;
    return bit;
}

char BitStream::readChar() {
    char value = 0;
    for (int i = 7; i >= 0; --i) {
        value |= (readBit() << i);
    }
    return value;
}

int BitStream::readInt() {
    int value = 0;
    for (int i = 15; i >= 0; --i) {
        value |= (readBit() << i);
    }
    return value;
}

int BitStream::readInt(int width) {
    int value = 0;
    for (int i = width - 1; i >= 0; --i) {
        value |= (readBit() << i);
    }
    return value;
}

void BitStream::writeBit(bool bit) {
    int byteIndex = bitpos / 8;
    int bitIndex = bitpos % 8;
    if (byteIndex >= buffer.size())
        buffer.push_back(0);
    if (bit)
        setBitAtX(byteIndex, bitIndex);
    bitpos++;
    bitcount++;
}

void BitStream::writeInt(int value, int numBits) {
    for (int i = numBits - 1; i >= 0; --i) {
        writeBit((value >> i) & 1);
    }
}

void BitStream::writeChar(char value, int numBits) {
    for (int i = numBits - 1; i >= 0; --i) {
        writeBit((value >> i) & 1);
    }
}

bool BitStream::getBitAtX(int byteIndex, int bitIndex) {
    return (buffer[byteIndex] & (1 << bitIndex));
}

void BitStream::setBitAtX(int byteIndex, int bitIndex) {
    buffer[byteIndex] |= (1 << bitIndex);
}

int BitStream::size() {
    return bitcount;
}

BitStream& BitStream::start() {
    bitpos = 0;
    return *this;
}

int BitStream::offset() {
    return bitpos;
}

bool BitStream::done() {
    return bitpos >= bitcount;
}

void BitStream::flush() {
    buffer.clear();
}

void BitStream::print() {
    start();
    int i = 0;
    while (i++ < size())
        cout<<(readBit() ? 1:0);
    cout<<endl;
}

BitStream BitStream::operator=(const BitStream& bs) {
    bitpos = bs.bitpos;
    bitcount = bs.bitcount;
    buffer = bs.buffer;
    return *this;
}

#endif