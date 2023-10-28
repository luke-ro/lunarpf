#include "TerrainHandler.h"



// converts lat.lon to stereographic coords x,y wit origin at south pole
std::pair<double,double> TerrainHandler::latlon2stereo(double lat, double lon){
    //TODO!
    return std::pair<double,double>(0,0);
}

// converts from stereographic x,y to lat,lon
std::pair<double,double> TerrainHandler::stereo2latlon(double x_stereo, double y_stereo){
    //TODO!
    return std::pair<double,double>(0,0);
}

// converts stereographic x,y to an x,y pair that properly correlates with topo map coords
std::pair<int,int> TerrainHandler::stereo2topo(double x_stereo, double y_stereo){
    //TODO!
    return std::pair<int,int>(0,0);
}

//converts from the topo map x,y pair to stereographic x,y
std::pair<double,double> TerrainHandler::topo2stereo(double x_topo, double y_topo){
    //TODO!
    return std::pair<double,double>(0,0);
}

// uses converts from topographific map coords to lat,lon
std::pair<double,double> TerrainHandler::topo2latlon(double x_topo, double y_topo){
    //TODO!
    return std::pair<double,double>(0,0);
}

// converts from lat,lon to coords that correlate with the topo maps
std::pair<int,int> TerrainHandler::latlon2topo(double lat, double lon){
    //TODO!
    return std::pair<int,int>(0,0);
}

/**
 * get the tile id for the tile that stereo coords lie in.  
*/
int TerrainHandler::getTileID(double x, double y){
    std::pair<int,int> ij = stereo2topo(x,y);
    return getTileID(ij.first,ij.second);
}

/**
 * @brief get the tile id for topo indeces i j
*/
int TerrainHandler::getTileID(int i, int j){
    return i*_tile_width+j;
}



/**
 * @brief Returns the 
*/
double TerrainHandler::getAGL(double lat, double lon, double r){\
    // radius minus surface height
    return r-getSurfaceHeight(lat,lon);
}

/**
 * @brief get the height of the surface of the moon at lat lon
*/
double TerrainHandler::getSurfaceHeight(double lat, double lon){
    std::pair<double,double> xy_topo = latlon2topo(lat,lon);

    // TODO Interpolate between adjacent cells
    // // get nearest points to 
    // std::vector<Eigen::Vector3d> nearest = getNearestPoints(xy_topo.first, xy_topo.second);

    // // get interpolated value at xy
    // double alt = interpolateSurface(nearest, xy_topo.first, xy_topo.second);

    Eigen::Vector3d near = getNearestPoint(xy_topo.first,xy_topo.second);

    return near[2];
}

/**
 * @brief gets the point nearest to topographic x y 
*/
Eigen::Vector3d TerrainHandler::getNearestPoint(double x, double y){
    std::pair<int,int> ij = stereo2topo(x,y);
    int id = getTileID(x,y);
    if(_tiles.find(id)==_tiles.end())
        loadTile(id);

    // set used to true
    _tiles[id].used = true;

    double alt = getAltAtInd(ij.first, ij.second);

    return Eigen::Vector3d(double(ij.first),double(ij.second),alt);
}

/**
 * @brief gets the alt of the ground at indeces i and j rel to top left 
*/
double TerrainHandler::getAltAtInd(int i, int j){
    int id = getTileID(i,j);
    double alt = _tiles[id].ptr.get()[i*_tile_width+j];
    return alt;
}

/**
 * @brief Gets the nearest points to stereographic coords x,y
*/
std::vector<Eigen::Vector3d> TerrainHandler::getNearestPoints(double x, double y){
    // if tile is not loaded:
        // load tile
    int id = getTileID(x,y);
    if(_tiles.find(id)==_tiles.end())
        loadTile(id);

    
    // set used to true
    _tiles[id].used = true;

    // return the four closest points TODO
    
    return std::vector<Eigen::Vector3d>();
}

void TerrainHandler::loadTile(int tile_id){
    int i = floor(tile_id/20);
    int j = tile_id%20;
    std::shared_ptr<float> ptr(getTile(i,j),free);
    Tile temp;
    temp.ptr = ptr;
    temp.used = true;
    _tiles[tile_id] = temp;
    return;
}

/**
 * @brief loads an image representing the height of the lunar surface
 * 
 * @param i the vertical identifier of the tile in 10's of km from the top
 * @param j the horizontal identifier of the tile in 10s of km from the left
*/
float* TerrainHandler::getTile(int i, int j){
    std::string filename =  "/media/sf_repos/lunarpf/topo_maps/x_000_y_000_km.tiff";
    std::string si = std::to_string(i);
    std::string sj = std::to_string(j);

    for(int k=si.length()-1; k>=0; k--)
        filename[k+37] = si[k];

    for(int m=sj.length()-1; m>=0; m--)
        filename[m+43] = si[m];

    const char* c_filename = filename.c_str();
    TinyTIFFReaderFile* tiffr=NULL;
    tiffr=TinyTIFFReader_open(c_filename); 

    std::cout<<c_filename<<"\n";
    if (!tiffr) { 
        std::cout<<"    ERROR reading (not existent, not accessible or no TIFF file)\n"; 
        std::cout<<"Attempted to read in: "<<filename<<"\n";
    } else { 
        const uint32_t width=TinyTIFFReader_getWidth(tiffr); 
        const uint32_t height=TinyTIFFReader_getHeight(tiffr);
        std::cout<<"width: "<<width<<"\n";
        std::cout<<"width: "<<height<<"\n";
        const uint16_t bitspersample=TinyTIFFReader_getBitsPerSample(tiffr, 0);      
        std::cout<<bitspersample<<"\n";
        
        float* image=(float*)calloc(width*height, bitspersample/8);  
        TinyTIFFReader_getSampleData(tiffr, image, 0); 
                
        std::cout<<"here\n";
        // for(int i=height-1;i>=0; i--){
        float data;
        // for(int i=height-2; i<height; i++){
        int i = height-1;
        for (int j=width-51; j<width;j++){
            data = (image)[i*width+j];
            printf("%f\n",data);
            // float temp = ((float*)image)[i*j];
        }

        TinyTIFFReader_close(tiffr); 
        return image; 
    } 

    TinyTIFFReader_close(tiffr); 
    return nullptr;
}

/**
 * @brief Interpolate a point based on its four closest neighbors. Right now it just choses nearest neighbor
*/
double TerrainHandler::interpolateSurface(std::vector<Eigen::Vector3d> points, double x, double y){
    return 0.0;
}


void TerrainHandler::manageTiles(){
    //if a tile has not been used in the last iteration, free that memory
    for (auto elem=_tiles.begin(); elem!=_tiles.end(); elem++){
        if(!elem->second.used){
            _tiles.erase(elem);
            continue;
        }
        elem->second.used=false;
    }

    //set the "used" bool to false for all remaining tiles
    return;
}

void TerrainHandler::printTile(Tile tile){
    const int num_chars = 69;
    char chars[num_chars] = {'$','@','B','%','8','&','W','M','#','*','o','a','h','k','b','d','p','q','w','m','Z','O','0','Q','L','C','J','U','Y','X','z','c','v','u','n','x','r','j','f','t','1','{','}','[',']','?','/','\\','|','(',')','<','>','i','!','+','~','l','I',';',':','-','_',',','\"','^','`','\'','.'};
    const int box_size = 40; 
    const int box_elems = box_size*box_size;
    const int n_boxes = 2000/box_size;

    double min = -1900;
    double max = 2000;

    char to_print[n_boxes*n_boxes] = {0};
    double intensity;
    for(int i=0; i<n_boxes; i++){
        for(int j=0; j<n_boxes; j++){
            intensity = 0.0;
            for(int k=0; k<box_size; k++){
                int offset = (((i*box_size+k)*2000) + j*box_size);
                for(int m=0; m<box_size; m++){
                    intensity+=(tile.ptr.get()[offset+m])/box_elems;
                }
            }
            // std::cout<<intensity<<", ";
            to_print[i*n_boxes+j] = chars[int(floor(double(num_chars)*(intensity-min)/(max-min)))];
        }
        // std::cout<<"\n";
    }

    for(int i=0;i<n_boxes;i++){
        for(int j=0;j<n_boxes;j++){
            std::cout<<to_print[i*n_boxes+j]<<" ";
        }
        std::cout<<"\n";
    }

    return;

}