#include <stdio.h>
#include <omp.h>

#define HILOS 4 //Numero de hilos a usar
#define N 10    //Repeticiones a realizar para la comparación
#define A 0     //Cota Inferior
#define B 10    //Cota Superior

#define VECES 100 //Numero de repeticiones para cada algoritmo

double tiempoSecuencial[N], tiempoParalelo[N];                                                //los tiempos de cada algoritmo
int n[N] = {100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000}; //Para realizar al comparacion se realizara la "Suma de Riemman" 10 veces cada vez con distintas particiones
double totalSecuencial[N], totalParalelo[N], aceleracion[N], eficiencia[N];

double funcion(x);

int main(int argc, char **argv[])
{
    int dim;

    printf("Resultados\n\t%s\t%s\t\t%s\t%s\t%s\t%s\t\t%s\t%s\n", "Nro", " N", "Total Secuencial", "Tiempo secuencial", "Total Paralela", "Tiempo paralela", "Aceleracion", "Eficiencia");

    for (dim = 0; dim < N; dim++)
    {
        int veces, i;
        double sumaSec;
        double inicio, duracion;
        duracion = 0.0;
        double dx = (B - A) / (n[dim] * 1.0); //Calculo de dx para "Suma de Riemman" de izquierda a derecha

        for (veces = 0; veces < VECES; veces++)
        {
            sumaSec = 0.0;
            inicio = omp_get_wtime();
            //Parte Secuencial
            for (i = 0; i < n[dim]; i++) //Se realiza para cada "n" del arreglo
            {
                //Por cada iteracion aumentara hasta llegar a la cota Superior dado que se hara para todas las particiones
                sumaSec += funcion(A + (i + 1) * dx); //Los resultados se iran sumando con cada iteracion
            }
            duracion += (omp_get_wtime() - inicio);
            totalSecuencial[dim] = sumaSec;
        }

        tiempoSecuencial[dim] = duracion / VECES; //Se almacena el tiempo para un posterior analisis

        //------------------------------

        duracion = 0.0;
        for (veces = 0; veces < VECES; veces++)
        {
            double totalP = 0; //Inicializando el resultado de las sumatorias, en este caso del algoritmo paralelo, en 0
            inicio = omp_get_wtime();

            //Parte Paralela
            #pragma omp parallel for num_threads(HILOS) private(i) reduction(+:totalP)
            // Para 4 hilos, con el acumulador "totalP" se realizara mediante un reduction para evitar
            // la condicion de carrera
            for (i = 0; i < n[dim]; i++) //Se realiza para cada "n" del arreglo
            {
                totalP += funcion(A + (i + 1) * dx);
            }
            duracion += (omp_get_wtime() - inicio);
            totalParalelo[dim] = totalP;
        }
        tiempoParalelo[dim] = duracion / VECES; //Se almacena el tiempo para un posterior analisis
        aceleracion[dim] = tiempoSecuencial[dim] / tiempoParalelo[dim];
        eficiencia[dim] = aceleracion[dim] / HILOS;
        printf("\t%i\t%i\t\t%f\t\t%f\t\t%f\t%f\t\t%f\t%f \n", dim, n[dim], totalSecuencial[dim] * dx, tiempoSecuencial[dim], totalParalelo[dim] * dx, tiempoParalelo[dim], aceleracion[dim], eficiencia[dim]);
    }
    return 0;
}

double funcion(double x)
{
    return x * x * x;
}