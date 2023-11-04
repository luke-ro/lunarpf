#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "eigen3/Eigen/Core"

//logger
#include "spdlog/spdlog.h"

// tools to read tiffs
#include "tinytiff_tools.hxx"
#include "tinytiffreader.h"
#include "tinytiffreader.hxx"

struct Tile
{
    std::shared_ptr<float> ptr = nullptr;
    bool used = false;
};

//Need some way to handle the different terrain tiles and to get height AGL 
class TerrainHandler{
    public:

    TerrainHandler():
        _tile_pixel_width(2000),
        _tile_pixel_height(2000),
        _num_tiles_height(20),
        _num_tiles_width(20),
        _max_tiles(10), 
        _r_moon(1736.0) //mean radius of the moon in km at the poles (https://en.wikipedia.org/wiki/Moon)
    {
        _map_pixels_height = _num_tiles_height*_tile_pixel_height;
        _map_pixels_width = _num_tiles_width*_tile_pixel_width;
    }

    // gets the height agl for a point at latitude lat and longitude lon
    double getAGL(double lat, double lon, double z);
    double getSurfaceHeight(double lat, double lon);
    std::vector<Eigen::Vector3d> getNearestPoints(double x, double y);
    Eigen::Vector3d getNearestPoint(double x, double y);

    // converts lat.lon to stereographic coords x,y wit origin at south pole
    std::pair<double,double> latlon2stereo(double lat, double lon);
    // converts from stereographic x,y to lat,lon
    std::pair<double,double> stereo2latlon(double x_stereo, double y_stereo);

    // converts stereographic x,y to an x,y pair that properly correlates with topo map coords
    std::pair<int,int> stereo2topo(double x_stereo, double y_stereo);
    //converts from the topo map x,y pair to stereographic x,y
    std::pair<double,double> topo2stereo(double x_topo, double y_topo);

    // uses converts from topographific map coords to lat,lon
    std::pair<double,double> topo2latlon(double x_topo, double y_topo);
    // converts from lat,lon to coords that correlate with the topo maps
    std::pair<int,int> latlon2topo(double lat, double lon);

    float* getTile(int i, int j);
    void printTile(Tile tile);

    
    // removes tiles that are no longer needed after a few iterations of disuse
    void manageTiles();

    // get the altitude at given topo indeces (relative to top left)

    // given four points, interoplate the surface at the coordinate x,y
    double interpolateSurface(std::vector<Eigen::Vector3d> points, double x, double y);
    
    void printCurrTileInfo();

    double getAltPolarCoords(double lat, double lon);
    
    private: 
    
    double getAltAtInd(int i, int j);

    // shared_ptr<uint32_t> getNewTile()
    void loadTile(int tile_id);

    // gets a tile's ID based on stereographic x y coords
    int getTileIDFromStereo(double x, double y);

    // gets a tile's ID based on topo i j coords
    int getTileIDFromIDXs(int i, int j);

    // gets tiles indexes from ID
    std::pair<int,int> tileID2Indeces(int id);


    // terminates the program but closes necessary things before hand. 
    void my_exit(std::string memo = "Default msg: Something has gone wrong. Exiting.");

    // // // Varaibles

    // a unordered_map to store currently loaded topographic map tiles
    std::unordered_map<int, Tile > _tiles;

    int _tile_pixel_width;
    int _tile_pixel_height;
    int _map_pixels_width;
    int _map_pixels_height;
    int _num_tiles_width;
    int _num_tiles_height;
    int _max_tiles;

    double _r_moon;
     
};

