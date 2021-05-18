#include "mpi_tools.h"
#include "ArrayParticlePositionCoupler.h"
#include <cstdlib>
#include <mpi.h>
#include <cmath>
#include <memory>


std::shared_ptr<ArrayParticlePositionCoupler> distribute_data(std::vector<Particle> &allData, int Ntot) {
    // Distribute global data

    int NTask,ThisTask;

    std::shared_ptr<ArrayParticlePositionCoupler> particlePositionCoupler(new ArrayParticlePositionCoupler());
    particlePositionCoupler->Ntot = Ntot;


    MPI_Comm_size(MPI_COMM_WORLD, &NTask);
    MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask);

    {
        int counts[NTask],displ[NTask];

        int n_mean = particlePositionCoupler->Ntot/NTask;

        // Local allocations

    //    particlePositionCoupler->testpositions = new TestData[N];

        for ( int iTask=0 ; iTask<NTask-1 ; iTask++ ) {
            counts[iTask]=n_mean;
        }
        counts[NTask-1]=particlePositionCoupler->Ntot-n_mean*(NTask-1); // last one gets the remainder

        particlePositionCoupler->Nlocal = counts[ThisTask];
        particlePositionCoupler->particles.resize(particlePositionCoupler->Nlocal);


        for ( int iTask=0 ; iTask<NTask ; iTask++ ) {
//            if ( ThisTask==0 ) {
//                std::cout << iTask << " " << counts[iTask] << std::endl;
//            }
            counts[iTask]*=sizeof(Particle);
        }

        displ[0] = 0;
        for ( int iTask=1 ; iTask<NTask ; iTask++ ) {
            displ[iTask]=displ[iTask-1]+counts[iTask-1];
        }

        // Distribute between tasks
        MPI_Scatterv(allData.data(),
               counts,
               displ,
               MPI_BYTE,
               particlePositionCoupler->particles.data(),
               sizeof(Particle)*particlePositionCoupler->localN(),
               MPI_BYTE,
               0,
               MPI_COMM_WORLD);
    }

    for ( int ip=0 ; ip<particlePositionCoupler->localN() ; ip++ ) {
        particlePositionCoupler->particles[ip].proc = ThisTask;
    }


    return particlePositionCoupler;
}

std::shared_ptr<std::vector<Particle>> gather_data(std::shared_ptr<ArrayParticlePositionCoupler> particlePositionCoupler) {
    int NTask,ThisTask;

    MPI_Comm_size(MPI_COMM_WORLD, &NTask);
    MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask);

    std::shared_ptr<std::vector<Particle>> allData(new std::vector<Particle>());


    if (ThisTask==0) {
        allData->resize(particlePositionCoupler->allN());
    }


    {
        int counts[NTask],displ[NTask];

        int n = particlePositionCoupler->localN();

        MPI_Gather(&n,
                1,
                MPI_INT,
                counts,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
                );

        if ( ThisTask==0 ) {
            for ( int iTask=0 ; iTask<NTask ; iTask++ ) {
                counts[iTask]*=sizeof(Particle);
            }

            displ[0] = 0;
            for ( int iTask=1 ; iTask<NTask ; iTask++ ) {
                displ[iTask]=displ[iTask-1]+counts[iTask-1];
            }

        }

        MPI_Gatherv(particlePositionCoupler->particles.data(),
           sizeof(Particle)*particlePositionCoupler->localN(),
           MPI_BYTE,
           allData->data(),
           counts,
           displ,
           MPI_BYTE,
           0,
           MPI_COMM_WORLD);
    }
    return allData;
}

std::vector<double> gather_tau(double *thistau,int localN,int allN) {
    int NTask,ThisTask;

    MPI_Comm_size(MPI_COMM_WORLD, &NTask);
    MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask);

    std::vector<double> alltau;


    if (ThisTask==0) {
        alltau.resize(allN);
    }


    {
        int counts[NTask],displ[NTask];

        int n = localN;

        MPI_Gather(&n,
                1,
                MPI_INT,
                counts,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
                );

        if ( ThisTask==0 ) {
//            for ( int iTask=0 ; iTask<NTask ; iTask++ ) {
//                counts[iTask]*=sizeof(Particle);
//            }

            displ[0] = 0;
            for ( int iTask=1 ; iTask<NTask ; iTask++ ) {
                displ[iTask]=displ[iTask-1]+counts[iTask-1];
            }

        }

        MPI_Gatherv(thistau,
           localN,
           MPI_DOUBLE,
           alltau.data(),
           counts,
           displ,
           MPI_DOUBLE,
           0,
           MPI_COMM_WORLD);
    }
    return alltau;
}
