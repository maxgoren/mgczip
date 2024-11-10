#ifndef stringbuffer_hpp
#define stringbuffer_hpp
#include <iostream>
#include <filesystem>
#include <fstream>
using namespace std;


class StringBuffer {
    private:
        char eosChar;
        string buff;
        int spos;
    public:
        StringBuffer();
        StringBuffer(string line);
        bool done();
        void init(string line);
        unsigned char get();
        unsigned char advance();
        void readBinaryFile(string filename);
};

StringBuffer::StringBuffer() {
    eosChar = 0x70;
}

StringBuffer::StringBuffer(string line) {
    eosChar = 0x70;
    init(line);
}

bool StringBuffer::done() {
    return spos >= buff.size();
}

void StringBuffer::init(string line) {
    spos = 0;
    buff = line;
}

unsigned char StringBuffer::get() {
    if (spos >= buff.length()) {
            return eosChar; 
    }
    return buff[spos];
}

unsigned char StringBuffer::advance() {
    spos++;
    if (spos >= buff.length()) {
        return eosChar;
    }
    return buff[spos];
}

void StringBuffer::readBinaryFile(string filename) {
    filesystem::path p(filename);
    auto length = filesystem::file_size(p);
    fstream ifile(filename, std::ios::in|std::ios::binary);
    if (!ifile.is_open()) {
        cout<<"Error: couldn't open '"<<filename<<"' for reading."<<endl;
        return;
    }
    string input;
    input.resize(length);
    ifile.read(reinterpret_cast<char*>(input.data()), length);
    init(input);
}

#endif