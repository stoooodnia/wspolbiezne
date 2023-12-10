#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h>

#define klucz_PW1 10
#define klucz_PW2 20
#define klucz_SEM 30
int pamiec_PW1;
int pamiec_PW2;
char *adres_PW1;
char *adres_PW2;
char *nazwa_gracza;
int semafor_sync;
int wynik;



char pobierzLitereABC() {
    char litera;

    while (1) {
        printf("Wprowadź literę A, B lub C: ");
        scanf(" %c", &litera);
        if (litera == 'A' || litera == 'B' || litera == 'C') {
            break;
        } else {
            printf("Błędna litera! Wprowadź ponownie.\n");
        }
    }

    return litera;
}

void czyszczeniePamieci() {
    int a1 = shmdt(adres_PW1);
    int b1 = shmdt(adres_PW2);
    int a2 = shmctl(pamiec_PW1, IPC_RMID, 0);
    int b2 = shmctl(pamiec_PW2, IPC_RMID, 0);
    printf("pw1d: %d\n", a1);
    printf("pw2d: %d\n", b1);
    printf("pw1ctl: %d\n", a2);
    printf("pw2ctl: %d\n", b2);
    int c = semctl(semafor_sync, 0, IPC_RMID);
    printf("c: %d\n", c);

}


int main(){
    semafor_sync = semget(klucz_SEM, 2, IPC_CREAT | IPC_EXCL | 0700);
    if(semafor_sync == -1) {
        nazwa_gracza = "Gracz 2";
        semafor_sync = semget(klucz_SEM, 2, 0);
    } else {
        semctl(semafor_sync, 0, SETVAL, 1);
        semctl(semafor_sync, 1, SETVAL, 0);
        nazwa_gracza = "Gracz 1";
    }
    pamiec_PW1 = shmget(klucz_PW1, 256, IPC_CREAT | 0700);
    pamiec_PW2 = shmget(klucz_PW2, 256, IPC_CREAT | 0700);

    printf("Witaj w grze! Twoja nazwa gracza to: %s\n zaraz rozpoczynamy!\n", nazwa_gracza);
    if (strcmp(nazwa_gracza, "Gracz 1") != 0) {
        printf("Oczekiwanie na gracza 1...\n");
    }



    for(int runda=0; runda < 3; runda++) {
        struct sembuf sem_op_wait;
        sem_op_wait.sem_flg = 0;

        if(strcmp(nazwa_gracza, "Gracz 1") == 0) {
            sem_op_wait.sem_num = 0;
            sem_op_wait.sem_op = -1;
        } else {
            sem_op_wait.sem_num = 1;
            sem_op_wait.sem_op = -1;
        }

        semop(semafor_sync, &sem_op_wait, 1);

        printf("Twoja kolej (%s)\n", nazwa_gracza);
        char wybor;
        wybor = pobierzLitereABC();

        if(strcmp(nazwa_gracza, "Gracz 1") == 0) {
            adres_PW1 = shmat(pamiec_PW1, 0, 0);
            strcpy(adres_PW1, &wybor);
            shmdt(adres_PW1);
        } else {
            adres_PW2 = shmat(pamiec_PW2, 0, 0);
            strcpy(adres_PW2, &wybor);
            shmdt(adres_PW2);
        }

        struct sembuf sem_op_post;
        sem_op_post.sem_flg = 0;

        if(strcmp(nazwa_gracza, "Gracz 1") == 0) {
            sem_op_post.sem_num = 1;
            sem_op_post.sem_op = 1;
        } else {
            sem_op_post.sem_num = 0;
            sem_op_post.sem_op = 1;
        }

        semop(semafor_sync, &sem_op_post, 1);

        struct sembuf sem_op_wait2;
        sem_op_wait2.sem_flg = 0;

        if(strcmp(nazwa_gracza, "Gracz 1") == 0) {
            sem_op_wait2.sem_num = 0;
            sem_op_wait2.sem_op = -1;
        } else {
            sem_op_wait2.sem_num = 1;
            sem_op_wait2.sem_op = -1;
        }

        semop(semafor_sync, &sem_op_wait2, 1);

        char wybor_drugiego_gracza;
        if(strcmp(nazwa_gracza, "Gracz 1") == 0) {
            adres_PW2 = shmat(pamiec_PW2, 0, 0);
            wybor_drugiego_gracza = *adres_PW2;
            shmdt(adres_PW2);
        } else {
            adres_PW1 = shmat(pamiec_PW1, 0, 0);
            wybor_drugiego_gracza = *adres_PW1;
            shmdt(adres_PW1);

        }

        struct sembuf sem_op_post2;
        sem_op_post2.sem_flg = 0;

        if(strcmp(nazwa_gracza, "Gracz 1") == 0) {
            sem_op_post2.sem_num = 0;
            sem_op_post2.sem_op = 1;
        } else {
            sem_op_post2.sem_num = 1;
            sem_op_post2.sem_op = 1;
        }

        semop(semafor_sync, &sem_op_post2, 1);


        if(strcmp(nazwa_gracza, "Gracz 1") == 0) {
            if(wybor_drugiego_gracza != wybor) {
                wynik += 1;
                printf("Wygrana! (+1)\n");
            } else {
                printf("Przegrana!\n");
            }
        } else {
            if(wybor_drugiego_gracza == wybor) {
                wynik += 1;
                printf("Wygrana! (+1)\n");
            } else {
                printf("Przegrana!\n");
            }
        }

        printf("Runda nr %d zakończona. Wynik: %d\n", runda+1, wynik);
    }
    printf("Koniec gry. Wynik: %d\n", wynik);
    if(strcmp(nazwa_gracza, "Gracz 2") == 0) {
        czyszczeniePamieci();
    }


    return 0;
}