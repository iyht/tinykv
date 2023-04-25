#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <time.h>

#define DB_FILE "kvdb.dat"
#define MAX_KEY_LEN 256
#define MAX_VALUE_LEN 1024
#define TIMESTAMP_LEN 24

struct kvpair {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    char timestamp[24]; // "YYYY-MM-DD HH:MM:SS.SSS"
};



struct log_record{
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    char timestamp[TIMESTAMP_LEN]; // "YYYY-MM-DD HH:MM:SS.SSS"

};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    int fd = open(DB_FILE, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (flock(fd, LOCK_EX) < 0) {
        perror("flock");
        return 1;
    }

    if (strcmp(argv[1], "set") == 0 && argc == 4) {
        struct kvpair pair;
        strncpy(pair.key, argv[2], MAX_KEY_LEN);
        strncpy(pair.value, argv[3], MAX_VALUE_LEN);
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        strftime(pair.timestamp, 24, "%Y-%m-%d %H:%M:%S.000", tm);
        lseek(fd, 0, SEEK_END);
        write(fd, &pair, sizeof(pair));
    } else if (strcmp(argv[1], "get") == 0 && argc == 3) {
        char key[MAX_KEY_LEN];
        strncpy(key, argv[2], MAX_KEY_LEN);
        struct kvpair pair;
        lseek(fd, 0, SEEK_SET);
        while (read(fd, &pair, sizeof(pair)) == sizeof(pair)) {
            if (strcmp(pair.key, key) == 0) {
                printf("%s\n", pair.value);
                break;
            }
        }
    } else if (strcmp(argv[1], "del") == 0 && argc == 3) {
        char key[MAX_KEY_LEN];
        strncpy(key, argv[2], MAX_KEY_LEN);
        struct kvpair pair;
        off_t pos = 0;
        while (read(fd, &pair, sizeof(pair)) == sizeof(pair)) {
            if (strcmp(pair.key, key) == 0) {
                lseek(fd, pos, SEEK_SET);
                struct kvpair empty_pair = {0};
                write(fd, &empty_pair, sizeof(empty_pair));
                break;
            }
            pos = lseek(fd, 0, SEEK_CUR);
        }
    }else if (strcmp(argv[1], "ts") == 0 && argc == 3) {
        char key[MAX_KEY_LEN];
        strncpy(key, argv[2], MAX_KEY_LEN);
        struct kvpair pair;
        lseek(fd, 0, SEEK_SET);
        time_t first_set_time = 0;
        time_t last_set_time = 0;
        while (read(fd, &pair, sizeof(pair)) == sizeof(pair)) {
            if (strcmp(pair.key, key) == 0) {
                if (first_set_time == 0) {
                    struct tm tm;
                    strptime(pair.timestamp, "%Y-%m-%d %H:%M:%S", &tm);
                    first_set_time = mktime(&tm);
                }
                struct tm tm;
                strptime(pair.timestamp, "%Y-%m-%d %H:%M:%S", &tm);
                last_set_time = mktime(&tm);
            }
        }
        if (first_set_time == 0) {
            printf("Key not found\n");
        } else {
            char first_set_time_str[24];
            char last_set_time_str[24];
            strftime(first_set_time_str, 24, "%Y-%m-%d %H:%M:%S.000", localtime(&first_set_time));
            strftime(last_set_time_str, 24, "%Y-%m-%d %H:%M:%S.000", localtime(&last_set_time));
            printf("%s %s\n", first_set_time_str, last_set_time_str);
        }
    } else {
        printf("Invalid command\n");
        return 1;
    }

    if (flock(fd, LOCK_UN) < 0) {
        perror("flock");
        return 1;
    }

    close(fd);

    return 0;
}