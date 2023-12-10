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

    // Pobieranie słowa do przetłumaczenia
    RequestMessage request;
    request.mtype = MSG_TYPE_REQUEST;
    request.pid = getpid();
    printf("Enter a word to translate: ");
    scanf("%s", request.word);

    // Wysłanie słowa do kolejki wejściowej serwera
    if (msgsnd(inputQueue, &request, sizeof(RequestMessage) - sizeof(long), 0) == -1) {
        perror("Error sending request");
        exit(EXIT_FAILURE);
    }

    // Odbiór odpowiedzi z kolejki wyjściowej serwera
    ResponseMessage response;
    if (msgrcv(outputQueue, &response, sizeof(ResponseMessage) - sizeof(long), request.pid, 0) == -1) {
        perror("Error receiving response");
        exit(EXIT_FAILURE);
    }

    printf("Translation: %s\n", response.translation);

    return 0;
}
