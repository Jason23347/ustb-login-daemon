#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <time.h>

FILE *log_stream;

#define __log_init(__file_name) { log_stream = fopen(__file_name, "a+"); }
#define __log_close() fclose(log_stream)

char file_name[0x0040];

time_t t;

__fortify_function void log_init()
{
    time(&t);
    strftime(file_name, 0x0040, "/var/log/ustb/login-%Y-%m-%d.log", localtime(&t));
    __log_init(file_name);
}
#define log_close() __log_close()

char buf[0x0040];

__fortify_function int
__write_log__(FILE *__restrict__ log, const char *__fmt, ...)
{
    time(&t);
    strftime(buf, 0x0040, "%Y-%m-%d %H:%M:%S", localtime(&t));
    return fprintf(log, "[%s]\t", buf) +
            fprintf(log, __fmt, __va_arg_pack ()) +
            fprintf(log, "\n");
}

#define __write_log(__fmt, ...) __write_log__(log_stream, __fmt, ##__VA_ARGS__)

#define write_log(fmt, ...)             \
{                                       \
    log_init();                         \
    if (!log_stream)                    \
        perror("open log file");        \
    __write_log(fmt, ##__VA_ARGS__);    \
    log_close();                        \
}

#define write_log_r(__file_name, fmt, ...)              \
{                                                       \
    __log_init(__file_name);                            \
    if (!log_stream)                                    \
        perror("open log file");                        \
    __write_log__(__file_name, fmt, ##__VA_ARGS__);     \
    __log_close();                                      \
}

#endif