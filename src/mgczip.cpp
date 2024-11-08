#include <iostream>
#include "encoder.hpp"
#include "decoder.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout<<"Not enough arguments"<<endl;
        cout<<"mgczip -c <filename>"<<endl;
        cout<<"mgczip -d <filename>"<<endl;
        return 0;
    }
    string doto = argv[2];
    if (argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'c': {
                    Encoder huff;
                    huff.compress(doto);
                }
                break;
            case 'd': {
                    Decoder huff;
                    huff.uncompress(doto);
                }
                break;
            default:
                cout<<"Invalid argument provided"<<endl;
            break;
        }
    }
    return 0;
}