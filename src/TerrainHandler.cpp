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
std::pair<double,double> TerrainHandler::stereo2topo(double x_stereo, double y_stereo){
    //TODO!
    return std::pair<double,double>(0,0);
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
std::pair<double,double> TerrainHandler::latlon2topo(double lat, double lon){
    //TODO!
    return std::pair<double,double>(0,0);
}



/**
 * @brief Returns the 
*/
double TerrainHandler::getAGL(double lat, double lon, double z){


    return z-getSurfaceHeight(lat,lon);
}

/**
 * @brief get the height of the surface of the moon at lat lon
*/
double TerrainHandler::getSurfaceHeight(double lat, double lon){
    std::pair<double,double> xy_topo = latlon2topo(lat,lon);

    // get nearest points to 
    std::vector<Eigen::Vector3d> nearest = getNearestPoints(xy_topo.first, xy_topo.second);

    // get interpolated value at xy
    double alt = interpolateSurface(nearest, xy_topo.first, xy_topo.second);

    return 0.0;
}

/**
 * @brief Gets the nearest points to stereographic coords x,y
*/
std::vector<Eigen::Vector3d> TerrainHandler::getNearestPoints(double x, double y){
    // if tile is not loaded:
        // load tile

    
    // set used to true

    // return the four closest points 
    
    return std::vector<Eigen::Vector3d>();
}



uint32_t* TerrainHandler::loadTile(int i, int j){
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
        // for(int i=height-2; i<height; i++){
        int i = height-1;
            for (int j=width-51; j<width;j++){
                data = ((float*)image)[i*width+j];
                printf("%f\n",data);
                // float temp = ((float*)image)[i*j];
            }
        // }
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

    //set the "used" bool to false for all remaining tiles
    return;
}

void TerrainHandler::printTile(Tile tile){
    char chars[69] = {'$','@','B','%','8','&','W','M','#','*','o','a','h','k','b','d','p','q','w','m','Z','O','0','Q','L','C','J','U','Y','X','z','c','v','u','n','x','r','j','f','t','/','\\','|','(',')','1','{','}','[',']','?','-','_','+','~','<','>','i','!','l','I',';',':',',','\"','^','`','\'','.'};
    const int box_size = 40; 
    const int n_boxes = 2000/box_size;

    double min = -2000;
    double max = 2000;

    char to_print = {0};
    double intensity;
    // for(int i=0; i<n_boxes; i++){
    //     for(int j=0; j<n_boxes; j++){
    //         intensity = 0.0;
    //         for(int k=0; k<box_size; k++){
    //             for(int m=0; m<box_size; m++){
    //                 intensity+=((float*)tile.ptr)[k*box_size+];
    //             }
    //         }
    //         to_print[i*40+j] = intensity;
    //     }
    // }

    return;

}