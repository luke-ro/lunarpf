# lunarpf
This is my (Luke Roberson) project for ASEN 5519 Science of Autonomous Systems in the Smead Department of Aerospace Engineeering Sciences at CU Boulder. 

This was performed on Ubuntu 22.04 LTS using C++ 11 and Python3.  


## installing dependencies (Ubuntu 22.04)

### Eigen
Simply run `sudo apt install libeigen3-dev` in the console. Cmake will then be able to find Eigen using `find-package()`.

### TinyTIFF
Read the instructions from the TinyTiff gihub pages [here](https://jkriege2.github.io/TinyTIFF/page_buildinstructions.html#page_buildinstructions_CMAKE). Perform the steps listed under "Building with MinGW/GNU/... Makefiles". I had issues running the -G which specifies the generator. I ommited this option with no issue. Once you have built using cmake with the `--target install` option, the CMakeLists.txt will be able to find the library and link to it. 
