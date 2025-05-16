#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define ADRESA "172.16.0.100"
#define PORT 5001
#define DIM_BUCATA 1000
#define DURATA 10

int main() {
    int conexiune;
    struct sockaddr_in destinatie;
    char date[DIM_BUCATA];
    memset(date, 'a', DIM_BUCATA);

    conexiune = socket(AF_INET, SOCK_STREAM, 0);
    if (conexiune < 0) {
        perror("Socket");
        exit(1);
    }

    destinatie.sin_family = AF_INET;
    destinatie.sin_port = htons(PORT);
    destinatie.sin_addr.s_addr = inet_addr(ADRESA);

    if (connect(conexiune, (struct sockaddr*)&destinatie, sizeof(destinatie)) < 0) {
        perror("Conectare");
        close(conexiune);
        exit(1);
    }

    time_t inceput = time(NULL);
    time_t final = inceput + DURATA;
    long long total = 0;

    while (time(NULL) < final) {
        ssize_t trimis = send(conexiune, date, DIM_BUCATA, 0);
        if (trimis < 0) break;
        total += trimis;
    }

    close(conexiune);

    double mbps = (total * 8.0) / (DURATA * 1000000.0);
    printf("Viteza: %.2f Mbps\n", mbps);

    return 0;
}
