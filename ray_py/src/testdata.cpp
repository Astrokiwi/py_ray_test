/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testdata.cpp
 * Author: davidjwilliamson
 * 
 * Created on 16 April 2021, 10:09
 */

#include "ArrayParticlePositionCoupler.h"
#include "testdata.h"
//#include "rt_prototypes.h"
#include "raytracing.h"
#include "mpi_tools.h"
#include <cstdlib>
#include <mpi.h>
#include <cmath>
#include <iostream>
#include <random>
#include <memory>

// globals, from testdata.h
//struct TestData *testpositions;
//int Nlocal,Ntot;
//ArrayParticlePositionCoupler *particlePositionCoupler = new ArrayParticlePositionCoupler();


std::shared_ptr<ArrayParticlePositionCoupler> generate_test_data() {
    
    int nblob = 27;
    int np_blob = 200;
//    int nblob = 2;
//    int np_blob = 32;
    int ThisTask;

    MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask); 


    // Create global data

    // creates a cube, NTask**(1/3) per side
    // if NTask isn't a cube, there will be some spillover
    // whatever, this is just a quick test
    std::vector<Particle> allData;

    if ( ThisTask==0 ) {
        int nside = std::lround(std::cbrt(nblob));
        double cent = -((float)nside)/2.;

        allData.resize(np_blob * nblob);
//        allData = new Particle[particlePositionCoupler->Ntot];

    //    std::random_device rd;  //Will be used to obtain a seed for the random number engine

        for ( int iblob=0 ; iblob<nblob ; iblob++ ) {
            int offset[3] = {iblob%nside,(iblob/nside)%nside,iblob/(nside*nside)};

            std::mt19937 gen(iblob); //Standard mersenne_twister_engine seeded with proc number for consistency
            std::normal_distribution<> dis(0.5, 0.2);

            for ( int ip=np_blob*iblob ; ip<np_blob*(iblob+1) ; ip++ ) {
                allData[ip].smoothing = 0.1;
                allData[ip].opacity = 1.e-6;
                for ( int jj=0 ; jj<3 ; jj++ ) {
                    allData[ip].Pos[jj] = dis(gen)+offset[jj]+cent;
                }
                // flatten for easier viz, harder tree
                allData[ip].Pos[2]*=0.01;
            }
        }
    }

    
    
//    if ( ThisTask==0 ) {
//        delete[] allData;
//    }

    
    return distribute_data(allData,np_blob * nblob);
    
//    std::cout << ThisTask << " " << ix << " " << iy << " " << iz << " " << nside << std::endl;
}


void dump_positions(std::shared_ptr<ArrayParticlePositionCoupler> particlePositionCoupler) {
    int ThisTask;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask);

    std::shared_ptr<std::vector<Particle>> allData = gather_data(particlePositionCoupler);

    if (ThisTask==0) {
        for ( int ip=0 ; ip<particlePositionCoupler->allN() ; ip++ ) {
            for ( int jj=0 ; jj<3 ; jj++ ) {
                std::cout << (*allData)[ip].Pos[jj] <<" ";
            }
            std::cout << (*allData)[ip].OpticalDepth << std::endl << std::flush;
        }
    }
}



void run_test() {

    std::shared_ptr<ArrayParticlePositionCoupler> p = generate_test_data();

    Raytracer raytracer(p);
    raytracer.build_tree();

    double *AGN_localtau = new double[p->localN()];
    double r_agn[3] = {0,0,0};

    raytracer.agn_optical_depths(r_agn,AGN_localtau,true);

    for ( int ii=0 ; ii<p->localN() ; ii++ ) {
        p->particles[ii].OpticalDepth = AGN_localtau[ii];
    }

    delete[] AGN_localtau;
    dump_positions(p);


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();		/* clean up & finalize MPI */

}


std::vector<double> test_depths() {
    std::shared_ptr<ArrayParticlePositionCoupler> p = generate_test_data();

    Raytracer raytracer(p);
    raytracer.build_tree();

    double *AGN_localtau = new double[p->localN()];
    std::vector<double> AGN_alltau;
    double r_agn[3] = {0,0,0};


    raytracer.agn_optical_depths(r_agn,AGN_localtau,true);

    AGN_alltau = gather_tau(AGN_localtau,p->localN(),p->allN());
    delete[] AGN_localtau;



//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Finalize();		/* clean up & finalize MPI */
    return AGN_alltau;
}