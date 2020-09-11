#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define HILOS 4
#define N 8
#define VECES 100
double dursec[N], durpar[N], sumMedSec[N], sumMedPar[N], aceleracion[N];
int n[N] = {
    1000, 10000, 50000,
    100000, 200000, 500000,
    750000, 1000000};
int main(int argc, char **argv)
{
    int dim;
    printf("%s, %s,       %s,   %s,       %s,       %s,       %s\n", "Nro", "N", "Tiempo Secuancial", "Valor", "Tiempo Paralelo", "Valor", "Aceleracion");

    for (dim = 0; dim < N; dim++)
    {
        int veces, i;
        double sumaSec, sumaParFinal, sumaPar;
        double inicio, duracion;
        /* Proceso secuencial */
        duracion = 0.0;
        for (veces = 0; veces < VECES; veces++)
        {
            inicio = omp_get_wtime();
            sumaSec = 0;
            for (i = 0; i < n[dim]; i++)
            {
                sumaSec += 4 * (pow(-1, i) / (2 * i + 1));
            }
            duracion += (omp_get_wtime() - inicio);
        }
        sumMedSec[dim] = sumaSec;
        dursec[dim] = duracion / VECES;
        /* Fin Proceso secuencial */

        /* Proceso paralelo */
        duracion = 0.0;
        for (veces = 0; veces < VECES; veces++)
        {

            double suma = 0;
            inicio = omp_get_wtime();
#pragma omp parallel for reduction(+ \
                                   : suma)
            for (i = 0; i < n[dim]; i++)
            {
                suma += 4.0 * pow(-1, i) / (2 * i + 1);
            }
            duracion += (omp_get_wtime() - inicio);
            sumMedPar[dim] = suma;
        }
        durpar[dim] = duracion / VECES;

        aceleracion[dim] = dursec[dim] / durpar[dim];
        printf("%i, %i,\t   %f,\t   %f,\t     %f,       %f,       %f\n", dim, n[dim], dursec[dim], sumMedSec[dim], durpar[dim], sumMedPar[dim], aceleracion[dim]);
    }
}