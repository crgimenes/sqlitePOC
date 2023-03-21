#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *random_string() {
    static char str[11];
    const char charset[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charset_len = sizeof(charset) - 1;
    srand(time(NULL));
    for (int i = 0; i < 10; ++i) {
        str[i] = charset[rand() % charset_len];
    }
    str[10] = '\0';
    return str;
}

int main(int argc, char **argv) {
    FILE *fp;
    fp = fopen("file.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    clock_t start_time = clock();
    for (int i = 0; i < 1000; i++) {
        char sender[100];
        char body[100];

        sprintf(sender, "sender %d %s", i, random_string());
        sprintf(body, "body %d %s", i, random_string());

        fprintf(fp, "%s %s\n", sender, body);

        // fflush(fp);
    }
    fclose(fp);

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time spent for insert: %f seconds\n", time_spent);

    return 0;
}
