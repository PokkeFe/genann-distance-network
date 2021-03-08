#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include "genann.h"
#include "dist.hpp"

const int TESTING_POINTS = 10000;
const bool ENABLE_LOG_EACH_TEST = false;

int main(int argc, char** argv) {

    genann *ann;
    double training_points;

    // Import ann model from file
    FILE * annFile = fopen("ann_model", "r");
    if(annFile == NULL) {
        printf("Error, cannot read model file.\n");
        exit(-1);
    } else {
        ann = genann_read(annFile);
        fclose(annFile);
    }
    FILE * annInfoFile = fopen("ann_info", "r");
    if(annInfoFile == NULL) {
        printf("Error, cannot read info file.\n");
        exit(-2);
    } else {
        fscanf(annInfoFile, "%d", &training_points);
        fclose(annInfoFile);
    }

    // Seed randomness
    int time_seed = (int) std::time(NULL);
    printf("Seeding randomness with %d\n", time_seed);
    std::srand(time_seed);

    // Starting timing
    clock_t pclk = clock();

    // Run predictions
    double error_total = 0.0;
    double error_max = 0.0;
    int clock_diff_total = 0;
    {
        // allocate testing inputs
        for(int i = 0; i < TESTING_POINTS; i++) {
            // initialize array
            double *testing_inputs = new double[2];

            // Generate a random point
            double x = ((double)std::rand()/(double)RAND_MAX);
            double y = ((double)std::rand()/(double)RAND_MAX);
            clock_t cz = clock();
            double z = get_dist_from_center(x, y);
            cz = clock() - cz;
            testing_inputs[0] = x;
            testing_inputs[1] = y;

            // Get ann prediction & error
            
            clock_t cp = clock();
            const double *prediction = genann_run(ann, testing_inputs);
            cp = clock() - cp;
            const double error = std::abs(prediction[0] - z);
            error_total += error;
            if(error > error_max) error_max = error;
            clock_diff_total += (cz - cp);

            // Print prediction stats
            if(ENABLE_LOG_EACH_TEST){
                printf("- %2d -------------\n", i);
                printf(" PREDICTION: %.3f\n", prediction[0]);
                printf(" ACTUAL    : %.3f\n", z);
                printf(" ERROR     : %.3f\n", error);
            }
        }
    }

    pclk = clock() - pclk;
    printf("Total cycles to complete: %d\n", pclk);

    // Print out stats
    
    const double error_avg = error_total / TESTING_POINTS;
    const double clock_diff_avg = (double)clock_diff_total / TESTING_POINTS;
    printf("---------------------------------\n");
    printf(" TRAINING DATA SIZE : %d points\n", training_points);
    printf(" TESTING DATA SIZE  : %d points\n", TESTING_POINTS);
    printf(" ERROR AVERAGE      : %.3f\n", error_avg);
    printf(" ERROR MAX          : %.3f\n", error_max);
    printf(" CLK DIFF TOTAL     : %d cycles\n", clock_diff_total);
    printf(" CLK DIFF AVG       : %.2f cycles\n", clock_diff_avg);
}