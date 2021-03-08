# genann-distance-network
Artificial Neural Network to estimate arctan distance using GENANN

## How to use

1. run ```make``` to build the program.
2. run ```./bin/generate.exe``` to generate a starting model.
3. run ```./bin/train.exe``` to train the model with additional data.
4. run ```./bin/predict.exe``` to run the model against the true formula, and track error.

There are no runtime parameters in the program, so any value changes must be made to the ```const``` variables at the top of each respective program's source code.