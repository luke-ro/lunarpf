#include <iostream>
#include <memory>

// TinyTIFF (for reading DTED)
#include "tinytiff_tools.hxx"
#include "tinytiffreader.h"
#include "tinytiffreader.hxx"

// spdlogger (for logging diagnostic data)
#include "spdlog/spdlog.h"


//my libraries
#include "../../src/TerrainHandler.h"

int main(){
    TerrainHandler th;
    // th.latlon2stereo(-89.0*3.14/180, 1.0*3.14/180);
    double temp = th.getSurfaceHeight(-89.0*3.14/180, 0.0001);
    th.printCurrTileInfo();
    std::cout<<temp<<"\n";
}