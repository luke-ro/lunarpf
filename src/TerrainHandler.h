#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "eigen3/Eigen/Core"

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

    

    TerrainHandler(){}

    // gets the height agl for a point at latitude lat and longitude lon
    double getAGL(double lat, double lon, double z);
    double getSurfaceHeight(double lat, double lon);
    std::vector<Eigen::Vector3d> getNearestPoints(double x, double y);

    // converts lat.lon to stereographic coords x,y wit origin at south pole
    std::pair<double,double> latlon2stereo(double lat, double lon);
    // converts from stereographic x,y to lat,lon
    std::pair<double,double> stereo2latlon(double x_stereo, double y_stereo);

    // converts stereographic x,y to an x,y pair that properly correlates with topo map coords
    std::pair<double,double> stereo2topo(double x_stereo, double y_stereo);
    //converts from the topo map x,y pair to stereographic x,y
    std::pair<double,double> topo2stereo(double x_topo, double y_topo);

    // uses converts from topographific map coords to lat,lon
    std::pair<double,double> topo2latlon(double x_topo, double y_topo);
    // converts from lat,lon to coords that correlate with the topo maps
    std::pair<double,double> latlon2topo(double lat, double lon);

    float* loadTile(int i, int j);
    void printTile(Tile tile);

    private: 
    
    // removes tiles that are no longer needed after a few iterations of disuse
    void manageTiles();

    // given four points, interoplate the surface at the coordinate x,y
    double interpolateSurface(std::vector<Eigen::Vector3d> points, double x, double y);

    // shared_ptr<uint32_t> getNewTile()

    
    // a unordered_map to store currently loaded topographic map tiles
    std::unordered_map<int, Tile > _tiles;
     
};

