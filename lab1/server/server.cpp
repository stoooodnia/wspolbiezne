#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unistd.h>

using json = nlohmann::json;
using namespace std;

json jsonData, jsonResult;
int firstValue, secondValue, result;
bool running = 1;

int server() {
    cout << "server is now running \n";
    while (running) {
        fstream dataFile("/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab1/messages/data.json");
        dataFile >> jsonData;

        if (jsonData.at("firstValue").is_number() && jsonData.at("secondValue").is_number()) {
            // Otwieranie pliku wynikowego w trybie nadpisywania
            fstream resultFile("/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab1/messages/result.json", ios::out);

            cout << "Proceed to engage data \n";
            cout << "received data, proccesing \n";
            firstValue = jsonData.at("firstValue");
            secondValue = jsonData.at("secondValue");
            result = firstValue + secondValue;
            jsonResult = {
                    {"result", result}
            };

            // Zapisz wynik jako liczba, a nie jako ciąg znaków
            resultFile << jsonResult.dump(4);
            resultFile.close();

            jsonData = {
                    {"firstValue", "noValue"},
                    {"secondValue", "noValue"}
            };
            dataFile.seekp(0);
            dataFile << jsonData.dump(4);
            dataFile.close();

        } else {
            cout << "no data provided \n";
        }
        unsigned int microsecond = 1000000;
        usleep(4 * microsecond);
    }
    return 0;
}



int main() {
    server();
    return 0;
}
