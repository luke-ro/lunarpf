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
    std::cout<<"Accessing three tiles.\n";
    th.getSurfaceHeight(-88.0*3.14/180, 1.0*3.14/180);
    th.getSurfaceHeight(-88.5*3.14/180, 1.0*3.14/180);
    th.getSurfaceHeight(-89.0*3.14/180, 1.0*3.14/180);
    th.printCurrTileInfo();

    std::cout<<"Running manageTiles(). 3 tiles should remain:\n";
    th.manageTiles();
    th.printCurrTileInfo();

    std::cout<<"Running manageTiles(). Tiles should be deleted.\n";
    th.manageTiles();
    th.printCurrTileInfo();
}