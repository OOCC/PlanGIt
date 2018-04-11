#include <libcommon.h>
#include <time.h>
#include <sys/time.h>


char* clocaltime()
{
    struct tm   *tm;
    time_t      t;
    static char timestr[128] = {0};


    time(&t);
    tm = localtime(&t);

    snprintf(timestr, sizeof(timestr),
             "%d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec);

    return timestr;
}

