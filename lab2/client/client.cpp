#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>


using namespace std;


// metody pomocnicze
void wait(int ile) {
    unsigned int microsecond = 1000000;
    usleep(ile * microsecond);
}
string read() {
    string line;
    vector<string> v;
    cout << "reading till empty line: \n";
    while(getline(cin, line)){
        if (line.empty()){
            break;
        }
        v.push_back(line);
    }

    std::string s;
    for (const string &piece : v) s += (piece + "\n");
    return s;


}

// ścieżki do bufora i lockfile
string bufferPath = "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab2/server/buffer.txt";
const char* lockFilePath = "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab2/server/lockFile.txt";


// główna funkcja klienta
int client(const string& responsePath){
    cout << "client is now running \n";

    string lines = read();

//    int fd;
    while(open(lockFilePath, O_CREAT|O_EXCL, 0) == -1) {
        cout << ("Server is busy, please wait\n");
        wait(4);
    }
    cout << "server is ready for input, proccesing\n" << endl;

    ofstream plik(bufferPath);
    if (plik.is_open()) {
        plik << responsePath << endl << lines << endl;
        cout << "message sent!" << endl;
    }

    return 0;
}

int main(int argc, char *argv[]) {

    string responsePath = argv[1];
    client(responsePath);
    return 0;
}




