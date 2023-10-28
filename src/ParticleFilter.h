#include <vector>

#include "eigen3/Eigen/Core"

#include "TerrainHandler.h"

struct Particle
{
    //states
    Eigen::Matrix<double,6,1> state;
    
    double w {0.0};
};


class ParticleFilter{
    public:

    ParticleFilter():
    _Dt(1.0),
    _mu_moon(4.9048695e12){}

    void runPF();

    // sets particles to start at some initial condition
    void initializeParticles(int n);
    
    // propogates states of the particles using RK for time Dt using dynamics
    void propogate();

    // updates the weights of all the particles 
    void update();


    // runs rk4 on a particle p IN PLACE
    void RK4(Particle& p, double Dt);

    // state space equation for orbital motion
    Eigen::Matrix<double,6,1> f_orbital(Eigen::Matrix<double,6,1> x);
    
    // returns the vector of particles
    std::vector<Particle> getParticles(){return _particles;}

    private:

    TerrainHandler altimeter;
    
    std::vector<Particle> _particles;
    double _Dt;
    const double _mu_moon;

};