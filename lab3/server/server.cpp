#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>


using namespace std;

// Funkcja rekurencyjnie przetwarza plik, uwzględniając dyrektywy \input
int processFile(const string& filename, const string& target_word) {
    ifstream file(filename);
    string line;
    int word_count = 0;

    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku: " << filename << endl;
        return 0;
    }

    vector<pid_t> child_pids; // Przechowujemy PIDy procesów potomnych

    while (getline(file, line)) {
        // Szukamy dyrektyw \input
        size_t input_pos = line.find("\\input{");
        while (input_pos != string::npos) {
            size_t closing_brace_pos = line.find("}", input_pos);
            if (closing_brace_pos != string::npos) {
                string subfile = line.substr(input_pos + 7, closing_brace_pos - input_pos - 7);
                subfile = "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab3/server/Files/" + subfile;

                pid_t child_pid = fork();

                if (child_pid == 0) { // Proces potomny
                    int child_word_count = processFile(subfile, target_word);
                    exit(child_word_count);
                } else if (child_pid > 0) { // Proces rodzicielski
                    child_pids.push_back(child_pid);
                }

                input_pos = line.find("\\input{", closing_brace_pos);
            } else {
                cerr << "Nie znaleziono zamykającej klamry '}' po \\input w linii: " << line << endl;
                break;
            }
        }

        // Sprawdzamy, czy linia zawiera zadane słowo
        regex r("\\b" + target_word + "\\b");
        smatch m;
        while (regex_search(line, m, r)) {
            word_count++;
            line = m.suffix().str();
        }


    }

    file.close();

    // Oczekiwanie na zakończenie procesów potomnych
    for (pid_t child_pid : child_pids) {
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) {
            word_count += WEXITSTATUS(status);
        }
    }

    return word_count;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Sposób użycia: " << argv[0] << " <nazwa_pliku> <szukane_słowo>" << endl;
        return 1;
    }

    string filename = argv[1];
    filename = "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab3/server/Files/" + filename;
    string target_word = argv[2];
    int total_word_count = processFile(filename, target_word);

    cout << "Ilość wystąpień słowa '" << target_word << "': " << total_word_count << endl;

    return 0;
}
