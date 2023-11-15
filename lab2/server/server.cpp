#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;


// ścieżki do bufora i lockfile
string bufferPath = "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab2/server/buffer.txt";
const char* lockFilePath = "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab2/server/lockFile.txt";

// zmienne pracujące
string responsePath;
string text;

// metody pomocnicze
void wait(int ile) {
    unsigned int microsecond = 1000000;
    usleep(ile * microsecond);
}

void readBuffer() {
    ifstream file(bufferPath);
    responsePath = "";
    text = "";

    if (file.is_open()) {
        getline(file, responsePath);
    }

    string line;
    while (std::getline(file, line)) {
        text += line + " [approved by server]" + "\n";
    }

    file.close();
}

void clearBuffer() {
    ofstream file;
    file.open(bufferPath, ofstream::out | ofstream::trunc);
    cout << "buffer cleared, ready for next client!" << endl;

}

bool lockFileExists() {
    ifstream lockFile(lockFilePath);
    bool res = lockFile.good();
    lockFile.close();
    return res;
}


// główna metoda serwera
void server() {
    cout << "server is now running \n";
    while(true){
        wait(1);
        if(lockFileExists()){
            cout << "lock file exists, reading buffer" << endl;
            readBuffer();
            ofstream plik(responsePath);
            if (plik.is_open()) {
                plik << text << endl;
                cout << "response sent!" << endl;
                clearBuffer();
            }
            if (remove(lockFilePath) == 0) {
                cout << "lock file deleted!" << endl;
            }

        } else {
            cout << "lockfile doesnt exist, no input in buffer!" << endl;
        }
        wait(1);
    }
}



int main() {
    server();
    return 0;
}
