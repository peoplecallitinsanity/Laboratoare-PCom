#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *cerere;
    char *raspuns;
    int conexiune;

    // Ex 1.1: GET dummy from main server
    conexiune = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);
    cerere = compute_get_request("54.217.160.10", "/api/v1/dummy", NULL, NULL, 0);

    printf("Cerere GET...\n");
    send_to_server(conexiune, cerere);
    raspuns = receive_from_server(conexiune);
    printf("Raspuns:\n%s\n", raspuns);

    printf("%s\n", raspuns);
    close(conexiune);
    free(cerere);
    free(raspuns);

    // Ex 1.2: POST dummy and print response from main server
    conexiune = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);

    char *campuri[] = { "dummy=valoare" };
    cerere = compute_post_request("54.217.160.10", "/api/v1/dummy",
    "application/x-www-form-urlencoded", campuri, 1, NULL, 0);


    send_to_server(conexiune, cerere);
    raspuns = receive_from_server(conexiune);
    printf("%s\n", raspuns);
    close(conexiune);
    free(cerere);
    free(raspuns);

    // Ex 2: Login into main server
    conexiune = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);
    char *date_login[] = { "username=student", "password=student" };
    cerere = compute_post_request("54.217.160.10", "/api/v1/auth/login", "application/x-www-form-urlencoded", date_login, 2, NULL, 0);
    send_to_server(conexiune, cerere);
    raspuns = receive_from_server(conexiune);
    printf("LOGIN:\n%s\n", raspuns);

    // Copiază manual cookie-ul din răspuns
    char cookie[200] = {0};
    char *cookie_start = strstr(raspuns, "connect.sid=");
    if (cookie_start) {
        char *end = strchr(cookie_start, ';');
        if (end) *end = '\0';
        sprintf(cookie, "connect.sid=%s", cookie_start + strlen("connect.sid="));
        printf("Cookie: %s\n", cookie);
    } else {
        printf("Eroare cookie\n");
        return 1;
    }

    close(conexiune);
    free(cerere);
    free(raspuns);

    // Ex 3: GET weather key from main server
    conexiune = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);
    char *cookie_login[] = { cookie };
    cerere = compute_get_request("54.217.160.10", "/api/v1/weather/key", NULL, cookie_login, 1);
    send_to_server(conexiune, cerere);
    raspuns = receive_from_server(conexiune);
    printf("key:\n%s\n", raspuns);

    // Extrage cheia API
    char api_key[100] = {0};
    char *key_start = strstr(raspuns, "{\"key\":\"");
    if (key_start) {
        key_start += strlen("{\"key\":\"");
        char *key_end = strchr(key_start, '"');
        if (key_end) {
            *key_end = '\0';
            strcpy(api_key, key_start);
            printf("Cheie: %s\n", api_key);
        }
    } else {
        printf("Eroare cheie\n");
        return 1;
    }

    close(conexiune);
    free(cerere);
    free(raspuns);
    printf("___________________________________________\n");
    // Ex 4: GET weather data from OpenWeather API
    conexiune = open_connection("172.64.153.120", 80, AF_INET, SOCK_STREAM, 0);
    char parametri_meteo[256];
    sprintf(parametri_meteo, "lat=44.7398&lon=22.2767&appid=%s", api_key);
    cerere = compute_get_request("172.64.153.120", "/data/2.5/weather", parametri_meteo, NULL, 0);
    send_to_server(conexiune, cerere);
    raspuns = receive_from_server(conexiune);
    printf("Raspuns OpenWeather:\n%s\n", raspuns);
    char *json_payload = strchr(raspuns, '{');

    printf("Date meteo:\n%s\n", json_payload);
    close(conexiune);
    free(cerere);

    // Ex 5: POST weather data for verification to main server
    conexiune = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);

    // Latitudine și longitudine trebuie să coincidă cu cele de la Ex 4
    char cale_weather[100];
    sprintf(cale_weather, "/api/v1/weather/44.7398/22.2767");

    char *corp_meteo[] = { json_payload };
    cerere = compute_post_request("54.217.160.10", cale_weather,
                "application/json", corp_meteo, 1, cookie_login, 1);

    send_to_server(conexiune, cerere);
    raspuns = receive_from_server(conexiune);
    printf("Raspuns:\n%s\n", raspuns);

    close(conexiune);
    free(cerere);
    free(raspuns);

    // Ex 6: Logout from main server
    conexiune = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);
    cerere = compute_get_request("54.217.160.10", "/api/v1/auth/logout", NULL, cookie_login, 1);

    send_to_server(conexiune, cerere);
    raspuns = receive_from_server(conexiune);
    printf("Raspuns logout:\n%s\n", raspuns);

    close(conexiune);
    free(cerere);
    free(raspuns);



    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!

    return 0;
}
