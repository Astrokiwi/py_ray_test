#include "ArrayParticlePositionCoupler.h"
#include "raytracing.h"
#include "mpi_tools.h"
#include <cstdlib>
#include <mpi.h>
#include <cmath>
#include <random>
#include <memory>



std::vector<double> raytrace_from_arrays(std::vector<std::vector<double>> pos,std::vector<double> smoothing,std::vector<double> opacity) {
    // throw in asserts to check n is fixed

    std::vector<Particle> allData;
    int n = smoothing.size();
    int ThisTask;

    MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask);

    // Copy global data
    if ( ThisTask==0 ) {
        allData.resize(n);

        for ( int ip=0 ; ip<n ; ip++ ) {
            allData[ip].smoothing = smoothing[ip];
            allData[ip].opacity = opacity[ip];
            for ( int jj=0 ; jj<3 ; jj++ ) {
                allData[ip].Pos[jj] = pos[ip][jj];
            }
        }
    }

    std::shared_ptr<ArrayParticlePositionCoupler> p = distribute_data(allData,n);

    Raytracer raytracer(p);
    raytracer.build_tree();

    double *AGN_localtau = new double[p->localN()];
    std::vector<double> AGN_alltau;
    double r_agn[3] = {0,0,0};

    raytracer.agn_optical_depths(r_agn,AGN_localtau,true);

    AGN_alltau = gather_tau(AGN_localtau,p->localN(),p->allN());
    delete[] AGN_localtau;



    return AGN_alltau;
}