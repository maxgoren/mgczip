#ifndef stringbuffer_hpp
#define stringbuffer_hpp
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
using namespace std;


class StringBuffer {
    private:
        vector<string> lines;
        char eosChar;
        string buff;
        int spos;
        int lpos;
    public:
        StringBuffer() {
            eosChar = 0x70;
        }
        StringBuffer(vector<string> lns) {
            eosChar = 0x70;
            init(lns);
        }
        StringBuffer(string line) {
            eosChar = 0x70;
            init(line);
        }
        bool done() {
            return spos >= buff.size();
        }
        void init(vector<string> lns) {
            lines.clear();
            lines = lns;
            spos = 0; 
            lpos = 0;
            buff = lines[lpos];
        }
        void init(string line) {
            lines.clear();
            lines.push_back(line);
            spos = 0;
            lpos = 0;
            buff = lines[lpos];
        }
        int lineNo() {
            return lpos;
        }
        unsigned char get() {
            if (spos >= buff.length() && lpos >= lines.size()) {
                    return eosChar; 
            }
            return buff[spos];
        }
        unsigned char advance() {
            spos++;
            if (spos >= buff.length()) {
                lpos++;
                if (lpos >= lines.size()) {
                    return eosChar;
                }
                spos = 0;
                buff = lines[lpos];
            }
            return buff[spos];
        }
        void readBinaryFile(string filename) {
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
};

#endif