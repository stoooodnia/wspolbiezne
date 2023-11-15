#include <iostream>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

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

char* readFromFIFO(int &fd) {
    char* message = new char[1000];
    read(fd, message, 1000);
    return message;
}

void writeToFIFO(int &fd, char* message) {
    write(fd, message, 1000);
}

char* concatenateStrings(const char* str1, const char* str2) {

    size_t len1 = strlen(str1);

    char* result = new char[len1 + strlen(str2) + 1];

    strcpy(result, str1);

    strcat(result, str2);

    return result;
}


int main(int argc, char** argv) {

    char* server_fifo_path = (char*) "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab4/server/server_fifo";
    char* client_fifo_path = argv[1]; // "/Users/karolstudniarek/Desktop/REPOZYTORIA/5sem/wspolbiezne/lab4/client/client_fifo";
    char* id = argv[2]; // ":2";


    createFIFO(client_fifo_path);

    printf("Wysyłam...\n");
    int server_fifo = openFIFO(server_fifo_path);
    char* message = concatenateStrings(client_fifo_path, id);
    writeToFIFO(server_fifo, message);
    closeFIFO(server_fifo);

    printf("Odbieram...\n");
    int client_fifo = openFIFO(client_fifo_path);
    printf("przed czytaniem:\n");
    char* response = readFromFIFO(client_fifo);
    printf("po czytaniu:\n");

    printf("Odpowiedź serwera: %s\n", response);

}
