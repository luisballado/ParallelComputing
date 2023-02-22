#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

int main() {

    // Measure start time
    struct timeval t;
    gettimeofday(&t, NULL);
    double start = t.tv_sec * 1000 + t.tv_usec / 1000;

    // Some processing to be timed
    usleep(200000); // 200ms

    // Measure end time
    gettimeofday(&t, NULL);
    double end = t.tv_sec * 1000 + t.tv_usec / 1000;

    // Elapsed time
    printf("%lf milliseconds elapsed\n", (end - start));

    return 0;
    
}