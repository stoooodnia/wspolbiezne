#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include<unistd.h>
using json = nlohmann::json;
using namespace std;



bool running = true;

void read() {
    fstream dataFile("/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab1/messages/data.json");

    json jsonData;
    dataFile >> jsonData;

    cout << "Enter first integer: ";
    cin >> jsonData.at("firstValue");
    cout << "Enter second integer: ";
    cin >> jsonData.at("secondValue");

    dataFile.seekp(0);
    dataFile << jsonData.dump(4); //

    dataFile.close();
}



int client(){
    cout << "client is now running \n";
    read();



    unsigned int microsecond = 1000000;
    usleep(4 * microsecond);

    while (running){
        fstream resultFile("/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab1/messages/result.json");
        json jsonResult;
        resultFile >> jsonResult;
        cout << "Proceed to engage result \n";
//        cout << jsonResult.at("result").is_number();

        if(jsonResult.at("result").is_number()){
            cout << "Received result, processing \n";
            cout << "Processed value is equal to " << jsonResult.at("result") << "\n";

            jsonResult = {
                    {
                        "result", "noValue"
                    }
            };
            resultFile.seekp(0);
            resultFile << jsonResult.dump(3);
            resultFile.close();
        } else {
            cout << "no result provided \n";
        }
        resultFile.close();
        unsigned int microsecond = 1000000;
        usleep(4 * microsecond);


    }
    return 0;
}

int main() {
    client();
    return 0;
}




