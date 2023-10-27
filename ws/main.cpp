#include <iostream>
#include <memory>

#include "tinytiff_tools.hxx"
#include "tinytiffreader.h"
#include "tinytiffreader.hxx"

//my libraries
#include "../src/TerrainHandler.h"

// in case I need characters from dark to light for pretty printing 
// $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.

/**
 * @brief uses a particle filtering algorithm to run one step of 
*/
void runPF(/*a particle filtering algo?, initial states?*/){
    
    //initialize states:

    // run pf

    // get particles and propogate states based on that 
}

/**
 * @brief takes in particles and uses runge kutta to propogate states for each particle on it for timestep Dt
 * 
 * @param particles particles to propogate for time Dt
 * @param Dt
*/
void propogateStates(){
    //runs RK on each particle for 
}

/**
 * @brief takes in all the current particles and current measurements to calculate current probablility for each particle
 * 
 * @param particles some sort of array or list of n particles
 * @param meas current altitude measurement 
*/
void updateStates(){
    //loop through all the particles and update weights
}

int main(){
    TerrainHandler th;
    Tile test_tile;
    std::shared_ptr<float> ptr(th.loadTile(90,90),free);
    test_tile.ptr = ptr;

    th.printTile(test_tile);
    return 0;
}