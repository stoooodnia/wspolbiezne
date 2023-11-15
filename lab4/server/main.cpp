#include <iostream>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "db.cpp"



int createFIFO(char* &path) {
    int result = mkfifo(path, 0666);
    if (result == 0) {
        printf("Kolejka FIFO utworzona pomyślnie.\n");
        return EXIT_SUCCESS;
    } else {
        printf("Nie udało się utworzyć kolejki.\n");
        return EXIT_FAILURE;
    }
}

int openFIFO(char* &path) {
    int fd = open(path, O_RDWR);
    if (fd == -1) {
        printf("Nie udało się otworzyć kolejki.\n");
        return EXIT_FAILURE;
    } else {
        printf("Kolejka FIFO otwarta pomyślnie.\n");
    }
    return fd;
}

int closeFIFO(int &fd) {
    int result = close(fd);
    if (result == -1) {
        printf("Nie udało się zamknąć kolejki.\n");
        return EXIT_FAILURE;
    } else {
        printf("Kolejka FIFO zamknięta pomyślnie.\n");
    }
    return EXIT_SUCCESS;
}

void serverWait(int ile) {
    unsigned int microsecond = 1000000;
    usleep(ile * microsecond);
}

char* readFromFIFO(int &fd) {
    char* message = new char[1000];
    read(fd, message, 1000);
    return message;
}

void writeToFIFO(int &fd, char* message) {
    write(fd, message, 1000);
}

char* getClientFIFOPath(char* message) {
    // wiadomosc typu "path/to/fifo:tresc wiadomosci"

    char* path = strtok(message, ":");
    return path;
}

// to musi być użyte po getClientFIFOPath bo strtok pamięta ostatniego stringa jakiego parsował
char* getMessage(char* message) {
    char* tresc = strtok(nullptr, "");
    return tresc;
}

void handleSIGHUP(const int signal) {
    printf("SIGHUP przechwycony: %d\n", signal);
}

void handleSIGTERM(const int signal) {
    printf("SIGTERM przechwycony: %d\n", signal);
}
void handleSIGUSR1(int) {
    printf("SIGUSR1 przechwycony, zamykanie serwera...\n");
    exit(0);
}


int main() {

    signal(SIGHUP, handleSIGHUP);
    signal(SIGTERM, handleSIGUSR1);
    signal(SIGUSR1, handleSIGUSR1);



    char* message;
    char* client_fifo_path;
    char* tresc;
    char* processed_message;
    int client_fifo;


    char* server_fifo_path = (char*) "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab4/server/server_fifo";
    // inicjalizacja bazy danych,
    prepareDatabase();
    displayCharacters();

    printf("Server starting...\n");
    // wydzielam obsługe samego pliku do oddzielnych plików, tworzenie, otwieranie
    createFIFO(server_fifo_path);

    int server_fifo;
    server_fifo = openFIFO(server_fifo_path);

    while (true) {
        serverWait(4);

        printf("Oczekuję na wiadomość...\n");
        message = readFromFIFO(server_fifo);

        serverWait(2);

        client_fifo_path = getClientFIFOPath(message);
        tresc = getMessage(message);

        printf("Otrzymałem wiadomość: %s\n", tresc);
//        closeFIFO(server_fifo);

        serverWait(2);

        processed_message = getCharacterByID(std::stoi(tresc, nullptr, 10));

        printf("Wysyłam wiadomość do klienta...\n");
        client_fifo = openFIFO(client_fifo_path);
        writeToFIFO(client_fifo, processed_message);
        closeFIFO(client_fifo);


    }

}
