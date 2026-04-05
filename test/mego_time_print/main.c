
#include <mego/util/std/time.h>

#include <stdio.h>

int main(int _argc, char *_argv[])
{
    printf("Current timezone in minutes: %d\n", mgu_minute_timezone());
    printf("Current timezone in hours  : %d\n", mgu_hour_timezone());

    do {
        mgu_time_t t_curr = time(NULL);
        if (t_curr == (mgu_time_t)-1) {
            fprintf(stderr, "Error getting current time: %s\n", strerror(errno));
            return 1;
        }
        printf("Current time in seconds since epoch: %lld\n", (long long)t_curr);

        struct tm gtm;
        if (mgu_gmtime_s(&t_curr, &gtm) == NULL) {
            fprintf(stderr, "Error converting to GMT: %s\n", strerror(errno));
            return 1;
        }
        printf("Current GMT time: %04d-%02d-%02d %02d:%02d:%02d\n",
               gtm.tm_year + 1900, gtm.tm_mon + 1, gtm.tm_mday,
               gtm.tm_hour, gtm.tm_min, gtm.tm_sec);
        
        t_curr = mgu_mkgmtime(&gtm);
        if (t_curr == (mgu_time_t)-1) {
            fprintf(stderr, "Error converting to mkgmtime: %s\n", strerror(errno));
            return 1;
        }

        printf("mgu_mkgmtime result: %lld\n", (long long)t_curr);

        t_curr = mgu_mkgmtime_fallback(&gtm);
        if (t_curr == (mgu_time_t)-1) {
            fprintf(stderr, "Error converting to mkgmtime_fallback: %s\n", strerror(errno));
            return 1;
        }

        printf("mgu_mkgmtime_fallback result: %lld\n", (long long)t_curr);
    } while(0);

    return 0;
}