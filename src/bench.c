/**
 * Main method for benchmarking
 */
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "search.h"

/**
 * A more precise verion of time()
 */
double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

/**
 * Print usage informations
 */
void print_usage(int argc, char **argv) {
    printf("USAGE\n");
    printf("%s [options]\n", argv[0]);
    printf("\n");
    printf("OPTIONS\n");
    printf("-h         Print this info\n");
    printf("-n num     Number of timed runs (Default: 100)\n");
    printf("-o file    Write benchmarks results to (Default: stderr)\n");
    printf("-q         Be quiet (Default: no)\n");
    printf("-w num     Number of warmup runs (Default: 10)\n");
    printf("\n");
}

/**
 * Main method
 */
int main(int argc, char **argv) {
    int runs = 100;
    FILE *out = stderr;
    int quiet = 0;
    int warmup_runs = 10;
    int opt_code;
    int outliers = 0;

    double *run_times = malloc(runs * sizeof(double));
    double sum = 0.0;
    double mean = 0.0;
    double standard_deviation = 0.0;
    int i;

    while((opt_code = getopt(argc, argv, "hn:o:qw:")) != -1) {
        switch(opt_code) {
            case 'h':
                print_usage(argc, argv);
                return 0;
            case 'n':
                runs = atoi(optarg);
                break;
            case 'o':
                out = fopen(optarg, "w");
                break;
            case 'q':
                quiet = 1;
                break;
            case 'w':
                warmup_runs = atoi(optarg);
                break;
            default:
                return 1;
        }
    }

    /* Warmup runs */
    for(i = 0; i < warmup_runs; i++) {
        if(!quiet) fprintf(out, "Warmup: %2d/%d...\n", i + 1, warmup_runs);
        search(argv[optind], argv + optind + 1, argc - optind - 1);
    }

    /* Timed runs */
    for(i = 0; i < runs; i++) {
        double start, stop;

        if(!quiet) fprintf(out, "Run: %2d/%d...\n", i + 1, runs);

        start = get_time();
        search(argv[optind], argv + optind + 1, argc - optind - 1);
        stop = get_time();
        run_times[i] = stop - start;

        if(!quiet) fprintf(out, "Time: %f\n", run_times[i]);
        mean += run_times[i] / (double) runs;
    }
    
    /* Calculate mean */
    fprintf(out, "Mean: %f\n", mean);

    /* Calculate standard deviation */
    for(i = 0; i < runs; i++) {
        standard_deviation += SQUARE(run_times[i] - mean);
    }
    standard_deviation = sqrt(standard_deviation / runs);
    fprintf(out, "Standard deviation: %f\n", standard_deviation);

    /* Calculate outliers */
    for(i = 0; i < runs; i++) {
        if(ABS(mean - run_times[i]) > standard_deviation * 3) outliers++;
    }
    fprintf(out, "Outliers: %d\n", outliers);

    free(run_times);

    return 0;
}
