#include <iostream>
#include <memory>

#include "tinytiff_tools.hxx"
#include "tinytiffreader.h"
#include "tinytiffreader.hxx"

//my libraries
#include "../src/TerrainHandler.h"

// in case I need characters from dark to light for pretty printing 
// $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.



int main(){
    TerrainHandler th;
    Tile test_tile;
    std::shared_ptr<float> ptr(th.loadTile(90,90),free);
    test_tile.ptr = ptr;

    th.printTile(test_tile);
    return 0;
}