#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVER_INPUT_QUEUE_KEY 1234
#define SERVER_OUTPUT_QUEUE_KEY 5678
#define MSG_TYPE_REQUEST 1

typedef struct {
    long mtype;
    pid_t pid;
    char word[50];
} RequestMessage;

typedef struct {
    long mtype;
    char translation[50];
} ResponseMessage;

int main() {
    // Tworzenie lub pobieranie kolejki wejściowej
    int inputQueue = msgget(SERVER_INPUT_QUEUE_KEY, IPC_CREAT | 0666);
    if (inputQueue == -1) {
        perror("Error creating/getting input queue");
        exit(EXIT_FAILURE);
    }

    // Tworzenie lub pobieranie kolejki wyjściowej
    int outputQueue = msgget(SERVER_OUTPUT_QUEUE_KEY, IPC_CREAT | 0666);
    if (outputQueue == -1) {
        perror("Error creating/getting output queue");
        exit(EXIT_FAILURE);
    }

    RequestMessage request;
    ResponseMessage response;

    while (1) {
        // Odbieranie wiadomości
        msgrcv(inputQueue, &request, sizeof(RequestMessage) - sizeof(long), MSG_TYPE_REQUEST, 0);

        printf("Server received request from client %d for word: %s\n", request.pid, request.word);

        sleep(3);

        // Tłumaczenie słowa
        if (strcmp(request.word, "ciasto") == 0) {
            strcpy(response.translation, "cake");
        } else if (strcmp(request.word, "auto") == 0) {
            strcpy(response.translation, "car");
        } else {
            strcpy(response.translation, "Nie znam takiego słowa");
        }

        printf("Server translated: %s -> %s\n", request.word, response.translation);

        // Wysyłanie wiadomości zwrotnej
        response.mtype = request.pid;
        if (msgsnd(outputQueue, &response, sizeof(ResponseMessage) - sizeof(long), 0) == -1) {
            perror("Error sending response");
            exit(EXIT_FAILURE);
        }

        printf("Server sent response to client %d\n", request.pid);
    }

    return 0;
}
