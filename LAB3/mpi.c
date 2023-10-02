#include <stdio.h>
#include <mpi.h>


int main(int argv, char** argv)
{
    int np, id;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    if (id == 0)
    {
        printf("Hola mundo soy MASTER %d de %d procesos\n", id, np);
    }
    else
    {
        prinf("Hola mundo mi ID es %d\n", id);
    }
    MPI_Finalize();
    return 0;
}