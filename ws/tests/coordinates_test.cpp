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

    // // Test 1
    std::pair<double,double> xy_st =  th.latlon2stereo(-89.0*3.14/180, 1.0*3.14/180);

    std::cout<<"latlon -> stereo\n";
    std::cout<<xy_st.first<<", "<<xy_st.second<<"\n";
    // std::cout<<latlon.first*180.0/3.14<<", "<<latlon.second*180.0/3.14<<"\n";
    
    // test 2
    // std::pair<double,double> xy_st =  th.stereo2topo(xy.first, xy.second);
    std::pair<double,double> xy_to =  th.stereo2topo(xy_st.first, xy_st.second);
    std::cout<<"stereo -> topo \n";
    // std::cout<<xy_st.first<<", "<<xy_st.second<<"\n";
    std::cout<<xy_to.first<<", "<<xy_to.second<<"\n";

    // test 3
    std::pair<int,int>  idxs =  th.topo2idxs(xy_to.first, xy_to.second);
    // xy_to =  th.idxs2topo(idxs.first, idxs.second);
    std::cout<<"topo -> idxs\n";
    std::cout<<idxs.first<<", "<<idxs.second<<"\n";
    // std::cout<<xy_to.first<<", "<<xy_to.second<<"\n";

    // test 4
    xy_to =  th.idxs2topo(idxs.first, idxs.second);
    std::cout<<"idxs -> topo\n";
    std::cout<<xy_to.first<<", "<<xy_to.second<<"\n";

    // test 5
    xy_st =  th.topo2stereo(xy_to.first, xy_to.second);
    std::cout<<"topo -> stereo\n";
    std::cout<<xy_st.first<<", "<<xy_st.second<<"\n";

    // test 6
    std::pair<double,double> latlon =  th.stereo2latlon(xy_st.first, xy_st.second);
    std::cout<<"stereo -> latlon\n";
    std::cout<<latlon.first*180.0/3.14<<", "<<latlon.second*180.0/3.14<<"\n";



    // std::pair<double,double> xy =  th.latlon2stereo(-89.0*3.14/180, 0.1*3.14/180);
    return 0;
}