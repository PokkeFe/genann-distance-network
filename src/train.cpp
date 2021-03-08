#include <stdlib.h>
#include <ctime>
#include "genann.h"
#include "dist.hpp"

const int TRAINING_POINTS = 100000000;
const int TRAINING_LOOPS = 3; // How many times to run the above training points through the ANN. If low memory, use fewer training points and more training loops

int main(int argc, char** argv) {
    
    genann *ann;
    double existing_training_points;

    // Import ann model and info from file
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
        fscanf(annInfoFile, "%d", &existing_training_points);
        fclose(annInfoFile);
    }

    // Generate Training data
    printf("  Allocating Memory...\n");
    double** training_inputs = new double*[TRAINING_POINTS];
    double** training_outputs = new double*[TRAINING_POINTS];
    // allocate training inputs
    for(int i = 0; i < TRAINING_POINTS; i++) {
        training_inputs[i] = new double[2];
        training_outputs[i] = new double[1];
    }
    for(int iter = 0; iter < TRAINING_LOOPS; iter++){
        printf("Running Training Iteration %d\n", iter);

        int time_seed = (int) std::time(NULL);
        printf("  Seeding randomness with %d\n", time_seed);
        std::srand(time_seed);
        
        printf("  Generating Points...\n");
        for(int i = 0; i < TRAINING_POINTS; i++) {
            // Generate a random point
            double x = ((double)std::rand()/(double)RAND_MAX);
            double y = ((double)std::rand()/(double)RAND_MAX);
            double z = get_dist_from_center(x, y);

            //printf("%.2f, %.2f, %.2f\n", x, y, z);
            
            training_inputs[i][0] = x;
            training_inputs[i][1] = y;
            training_outputs[i][0] = z;
        }
        printf("Training data generated!\n");

        // Train network
        printf("Training Network...\n");
        
        for(int i = 0; i < TRAINING_POINTS; i++) {
            genann_train(ann, training_inputs[i], training_outputs[i], 0.1);
        }
    }

    // Empty training data from memory (Clear up RAM)
    for(int i = 0; i < TRAINING_POINTS; i++) {
        delete[] training_inputs[i];
        delete[] training_outputs[i];
    }
    delete[] training_inputs;
    delete[] training_outputs;

    // Save ANN model to file
    printf("Saving network model and info to file...\n");

    annFile = fopen("ann_model", "w");
    if(annFile == NULL) perror("Error opening file.\n");
    else {
        genann_write(ann, annFile);
        fclose(annFile);
    }
    // Save training info to file
    annInfoFile = fopen("ann_info", "w");
    if(annInfoFile == NULL) perror("Error opening ann_info.\n");
    else {
        fprintf(annInfoFile, "%d\n", TRAINING_POINTS + existing_training_points);
        fclose(annInfoFile);
    }

    // Free Memory
    genann_free(ann);
    
    printf("Network saved!\n");

}