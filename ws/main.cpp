#include <iostream>
#include <memory>

// TinyTIFF (for reading DTED)
#include "tinytiff_tools.hxx"
#include "tinytiffreader.h"
#include "tinytiffreader.hxx"

// spdlogger (for logging diagnostic data)
#include "spdlog/spdlog.h"


//my libraries
#include "../src/TerrainHandler.h"

// in case I need characters from dark to light for pretty printing 
// $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.



int main(){
    TerrainHandler th;
    std::pair<double,double> latlon =  th.stereo2latlon(100,0.1);
    std::pair<double,double> xy =  th.latlon2stereo(latlon.first,latlon.second);
    std::cout<<latlon.first*180.0/3.14<<", "<<latlon.second*180.0/3.14<<"\n";
    std::cout<<xy.first<<", "<<xy.second<<"\n";
    return 0;
}