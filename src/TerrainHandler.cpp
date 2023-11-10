#include "TerrainHandler.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// converts lat.lon to stereographic coords x,y wit origin at south pole
std::pair<double,double> TerrainHandler::latlon2stereo(double lat, double lon){
    // https://mathworld.wolfram.com/StereographicProjection.html
    const double phi1 {-90.0*3.14/180.0};
    const double lam0 {0};
    double k = (2.0*_r_moon)/(1+sin(phi1)*sin(lat)+ cos(phi1)*cos(lat)*cos(lon-lam0));
    double x = k*cos(lat)*sin(lon-lam0);
    double y = k*(cos(phi1)*sin(lat)-sin(phi1)*cos(lat)*cos(lon-lam0));
    return std::pair<double,double>(x,y);
}

// converts from stereographic x,y to lat,lon
std::pair<double,double> TerrainHandler::stereo2latlon(double x_stereo, double y_stereo){
    // https://mathworld.wolfram.com/StereographicProjection.html

    // I dont really need these as their own variables, but 
    // it makes translation from wolfram easier 
    const double phi1 {-90.0*3.14/180.0};
    const double lam0 {0};

    // some of the following things could def use some optimizations. 
    // Want to make sure it works though 

    double rho = sqrt((x_stereo*x_stereo)+(y_stereo*y_stereo));
    double c = 2.0 * atan2(rho, 2.0*_r_moon);

    // phi = lat
    double phi = asin((cos(c)*sin(phi1))+((y_stereo*sin(c)*cos(phi1))/rho));

    // lam = lon
    double lam = lam0 + atan2( x_stereo*sin(c), (rho*cos(phi1)*cos(c)) - (y_stereo*sin(phi1)*sin(c))); 
    return std::pair<double,double>(phi, lam); 
}

// converts stereographic x,y to an x,y pair that properly correlates with topo map coords
std::pair<double,double> TerrainHandler::stereo2topo(double x_stereo, double y_stereo){
    if(abs(x_stereo)>=99){
        spdlog::warn("Stereographic x coord out of bounds.");
        x_stereo = sgn(x_stereo)*99.0;
    }

    if(abs(y_stereo)>=99){
        spdlog::warn("Stereographic y coord out of bounds.");
        y_stereo = sgn(y_stereo)*99.0;
    }

    double x = (x_stereo+100.0);

    // THIS NEEDS FIXING
    // double i_double = .0-y_stereo;
    double y = (100.0-y_stereo);

    return std::pair<double,double>(x,y);
}

// converts from coords that have origin in top left to stereographic x,y
std::pair<double,double> TerrainHandler::topo2stereo(double x_topo, double y_topo){
    
    double x_stereo = x_topo - 100;

    double y_stereo = -y_topo + 100;
    return std::pair<double,double>(x_stereo,y_stereo);
}

// uses converts from coords that have origin in top left to lat,lon
std::pair<double,double> TerrainHandler::topo2latlon(double x_topo, double y_topo){
    std::pair<double,double> latlon = topo2stereo(x_topo,y_topo);
    return stereo2latlon(latlon.first,latlon.second);
}

// converts from lat,lon to coords that have origin at top left of map
std::pair<double,double> TerrainHandler::latlon2topo(double lat, double lon){
    std::pair<double,double> xy_stereo = latlon2stereo(lat,lon);
    return stereo2topo(xy_stereo.first, xy_stereo.second);
}

// convertes coordinates with origin at top left of map to map indeces
std::pair<int,int> TerrainHandler::topo2idxs(double x_topo, double y_topo){
    return std::pair<int,int> (int(y_topo*_tile_pixel_height/_km_tile_height), int(x_topo*_tile_pixel_width/_km_tile_width));
}

// converts map indeces to continuous points with origin at top left of map
std::pair<double,double> TerrainHandler::idxs2topo(int i_map, double j_map){
    return std::pair<double,double> (double(j_map)/_map_pixels_width, double(i_map)/_map_pixels_height);
}

/**
 * get the tile id for the tile that stereo coords lie in.  
*/
int TerrainHandler::getTileIDFromStereo(double x, double y){
    std::pair<double,double> topo = stereo2topo(x,y);
    std::pair<int,int> ij = topo2idxs(topo.first, topo.second);
    // getTileIDFromIDXs(ij.first,ij.second);
    // std::pair<int,int> tile_ij(ij.first/_tile_pixel_height,ij.second/_tile_pixel_width);
    return getTileIDFromIDXs(ij.first, ij.second);

}

/**
 * @brief get the tile id for topo indeces i j
*/
int TerrainHandler::getTileIDFromIDXs(int i, int j){
    return _num_tiles_width*(i/_tile_pixel_height)+(j/_tile_pixel_width);
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
    std::pair<double,double> xy_ster = latlon2stereo(lat,lon);

    // TODO Interpolate between adjacent cells
    // // get nearest points to 
    // std::vector<Eigen::Vector3d> nearest = getNearestPoints(xy_topo.first, xy_topo.second);

    // // get interpolated value at xy
    // double alt = interpolateSurface(nearest, xy_topo.first, xy_topo.second);

    Eigen::Vector3d near = getNearestPoint(xy_ster.first,xy_ster.second);

    return near[2];
}

/**
 * @brief gets the point nearest to stereographic x y 
*/
Eigen::Vector3d TerrainHandler::getNearestPoint(double x, double y){
    std::pair<double,double> xy_topo = stereo2topo(x,y);
    int id = getTileIDFromStereo(x,y);

    loadTile(id);

    // set used to true
    _tiles[id].used = true;

    auto ij = topo2idxs(xy_topo.first, xy_topo.second);
    double alt = getAltAtInd(ij.first, ij.second);

    return Eigen::Vector3d(double(ij.first),double(ij.second),alt);
}

/**
 * @brief gets the alt of the ground at indeces i and j rel to top left 
*/
double TerrainHandler::getAltAtInd(int i, int j){
    int id = getTileIDFromIDXs(i,j);
    auto ij_local = getLocalij(i,j);
    double alt = _tiles.at(id).ptr.get()[ij_local.first*_tile_pixel_width+ij_local.second];
    return alt;
}

std::pair<int,int> TerrainHandler::getLocalij(int i,int j){
    return std::pair<int,int> (i-((i/_tile_pixel_height)*_tile_pixel_height), j-((j/_tile_pixel_width)*_tile_pixel_width));
}

/**
 * @brief Gets the nearest points to stereographic coords x,y
*/
std::vector<Eigen::Vector3d> TerrainHandler::getNearestPoints(double x, double y){
    // if tile is not loaded:
        // load tile
    int id = getTileIDFromStereo(x,y);
    if(_tiles.find(id)==_tiles.end())
        loadTile(id);

    
    // set used to true
    _tiles[id].used = true;

    // return the four closest points TODO
    
    return std::vector<Eigen::Vector3d>();
}

/**
 * @brief loads a tile. Assumes that there was already a check to see if the tile was loaded. 
*/
void TerrainHandler::loadTile(int tile_id){
    // Check if we are here, then there was an issue elsewhere 
    std::cout<<"tile_id: "<< tile_id<<"\n";
    if(_tiles.find(tile_id) != _tiles.end()){
        
        // my_exit("Something has gone wrong. Already loaded tile was about to be loaded again. Exiting.");
        
        //tile already loaded, return
        return;
    }

    std::pair<int,int> idxs = tileID2Indeces(tile_id);
    std::shared_ptr<float> ptr(getTile(idxs.first,idxs.second),free);
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
    std::string si = std::to_string(i*10);
    std::string sj = std::to_string(j*10);
    std::cout<<"si: "<<si<<"\n";
    std::cout<<"sj: "<<sj<<"\n";

    for(int k=0; k<si.length(); k--)
        filename[39-si.length()+k] = si[k];

    for(int m=0; m<sj.length(); m--)
        filename[45-sj.length()+m] = sj[m];

    const char* c_filename = filename.c_str();
    TinyTIFFReaderFile* tiffr=NULL;
    tiffr=TinyTIFFReader_open(c_filename); 

    // std::cout<<c_filename<<"\n";
    if (!tiffr) { 
        std::cout<<"    ERROR reading (not existent, not accessible or no TIFF file)\n"; 
        std::cout<<"Attempted to read in: "<<filename<<"\n";
    } else { 
        const uint32_t width=TinyTIFFReader_getWidth(tiffr); 
        const uint32_t height=TinyTIFFReader_getHeight(tiffr);
        // std::cout<<"width: "<<width<<"\n";
        // std::cout<<"width: "<<height<<"\n";
        const uint16_t bitspersample=TinyTIFFReader_getBitsPerSample(tiffr, 0);      
        std::cout<<bitspersample<<"\n";
        
        float* image=(float*)calloc(width*height, bitspersample/8);  
        TinyTIFFReader_getSampleData(tiffr, image, 0); 
                
        // std::cout<<"here\n";
        // for(int i=height-1;i>=0; i--){
        float data;
        // for(int i=height-2; i<height; i++){
        int i = height-1;
        for (int j=width-51; j<width;j++){
            data = (image)[i*width+j];
            // printf("%f\n",data);
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
    // for (auto elem=_tiles.begin(); elem!=_tiles.end(); elem++){
    auto elem = _tiles.begin();
    while(elem!=_tiles.end()){
        if(!elem->second.used){

            // because shared pointers are being used, data being 
            // stored in them should be freed when they go out of scope 
            elem = _tiles.erase(elem); 
            continue;
        }
        elem->second.used=false;
        elem++;
    }

    // make sure their aren't too many tile loaded
    if(_tiles.size()>_max_tiles){
        spdlog::warn("Max number of tiles has been exceeded. Exiting.");

        my_exit(); 
    }

    //set the "used" bool to false for all remaining tiles
    return;
}

void TerrainHandler::printTileByID(int tileID){
    loadTile(tileID);
    printTile(_tiles.at(tileID));
    return;
}

void TerrainHandler::printTile(const Tile& tile){
    const int num_chars = 69;
    char chars[num_chars] = {'$','@','B','%','8','&','W','M','#','*','o','a','h','k','b','d','p','q','w','m','Z','O','0','Q','L','C','J','U','Y','X','z','c','v','u','n','x','r','j','f','t','1','{','}','[',']','?','/','\\','|','(',')','<','>','i','!','+','~','l','I',';',':','-','_',',','\"','^','`','\'','.'};
    const int box_size = 40; 
    const int box_elems = box_size*box_size;
    const int n_boxes = 2000/box_size;

    double min = -4000;
    double max = 3000;

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


void TerrainHandler::printTileInterp(double x_ster_min, double x_ster_max, int n_x, double y_ster_min, double y_ster_max, int n_y){
    const int num_chars = 69;
    char chars[num_chars] = {'$','@','B','%','8','&','W','M','#','*','o','a','h','k','b','d','p','q','w','m','Z','O','0','Q','L','C','J','U','Y','X','z','c','v','u','n','x','r','j','f','t','1','{','}','[',']','?','/','\\','|','(',')','<','>','i','!','+','~','l','I',';',':','-','_',',','\"','^','`','\'','.'};
    // const int box_size = 40; 
    // const int box_elems = box_size*box_size;
    // const int n_boxes = 2000/box_size;

    double min = -4000;
    double max = 3000;

    char to_print[(n_x+1)*(n_y+1)] = {0};
    double intensity;

    // std::pair<int,int> tile_idxs = tileID2Indeces(tile_ID);
    
    double x_step = (x_ster_max-x_ster_min)/n_x;
    double y_step = (y_ster_max-y_ster_min)/n_y;

    Eigen::Vector3d pt;
    int i = 0;
    for(double yy=y_ster_min; yy<y_ster_max; yy+=y_step){

        int j = 0;
        for(double xx=x_ster_min; xx<=x_ster_max; xx+=x_step){
            std::cout<<"xx: "<<xx; 
            std::cout<<", yy: "<<yy<<"\n"; 
            pt = getNearestPoint(xx,yy);
            intensity = pt[2];

            // std::cout<<intensity<<", ";
            to_print[i*n_x+j] = chars[int(floor(double(num_chars)*(intensity-min)/(max-min)))];

            j++;
        }
        // std::cout<<"\n";
        i++;
        printCurrTileInfo();
        manageTiles();
    }

    for(int i=n_y;i>=0;i--){
        for(int j=0;j<n_x;j++){
            std::cout<<to_print[i*n_x+j]<<" ";
        }
        std::cout<<"\n";
    }

    return;

}



std::pair<int,int> TerrainHandler::tileID2Indeces(int id){  
    int i = floor(id/_num_tiles_width);
    int j = id%_num_tiles_width;
    return std::pair<int,int>(i,j);
}

/**
 * @brief prints out some diagnostic info, returns tile IDs
*/
std::vector<int> TerrainHandler::printCurrTileInfo(){
    spdlog::info("Tile numbers: ");
    std::pair<int,int> idxs;
    std::vector<int> IDs;
    std::string msg;
    for(auto iter=_tiles.begin(); iter!=_tiles.end(); iter++){
        idxs = tileID2Indeces(iter->first);
        msg = "(ID: " + std::to_string(iter->first)+") "+std::to_string(idxs.first)+", "+std::to_string(idxs.second);
        spdlog::info(msg);
        IDs.push_back(iter->first);
    }
    msg = "There are "+ std::to_string(_tiles.size()) + " tiles currently loaded";
    spdlog::info(msg);
    return IDs;
}

void TerrainHandler::my_exit(std::string memo){
    spdlog::warn(memo);

    // Using exit() here should be alright because I am not messing with 
    // shared memory or other files that need to be closes. 
    // Could put a block of code above to manage things before exiting if
    // that becomes a problem.  
    exit(EXIT_FAILURE);  
}