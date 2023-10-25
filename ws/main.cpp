#include <iostream>



#include "tinytiff_tools.hxx"
#include "tinytiffreader.h"
#include "tinytiffreader.hxx"



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
    const char* filename = "/media/sf_repos/lunarpf/topo_maps/x_090_y_090_km.tiff";
    TinyTIFFReaderFile* tiffr=NULL;
    tiffr=TinyTIFFReader_open(filename); 
    if (!tiffr) { 
        std::cout<<"    ERROR reading (not existent, not accessible or no TIFF file)\n"; 
    } else { 
        const uint32_t width=TinyTIFFReader_getWidth(tiffr); 
        const uint32_t height=TinyTIFFReader_getHeight(tiffr);
        const uint16_t bitspersample=TinyTIFFReader_getBitsPerSample(tiffr, 0);      
        std::cout<<bitspersample<<"\n";
        uint32_t* image=(uint32_t*)calloc(width*height, bitspersample/8);  
        TinyTIFFReader_getSampleData(tiffr, image, 0); 
                
        std::cout<<"here\n";
        // for(int i=height-1;i>=0; i--){
        float data;
        for(int i=1; i<2; i++){
            for (int j=0; j<50;j++){
                data = ((float*)image)[i*j];
                printf("%f\n",data);
                // float temp = ((float*)image)[i*j];
            }
        }
        std::cout<<"there\n";
                            // HERE WE CAN DO SOMETHING WITH THE SAMPLE FROM THE IMAGE 
                            // IN image (ROW-MAJOR!)
                            // Note: That you may have to typecast the array image to the
                            // datatype used in the TIFF-file. You can get the size of each
                            // sample in bits by calling TinyTIFFReader_getBitsPerSample() and
                            // the datatype by calling TinyTIFFReader_getSampleFormat().
                
        free(image); 
    } 
    TinyTIFFReader_close(tiffr); 
    return 0;
}