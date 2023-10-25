//Need some way to handle the different terrain tiles and to get height AGL 
class TerrainHandler{
    public:

    TerrainHandler(){};

    double getAlt(double x, double y);


    private: 
    
    // removes tiles that are no longer needed
    void manageTiles();
};