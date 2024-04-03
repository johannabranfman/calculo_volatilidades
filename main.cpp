using namespace std;

#ifndef __MAIN_CPP
#define __MAIN_CPP

#include <iostream>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <cstring>
#include <stdexcept>

#include "black_scholes.h"
#include "black_scholes.cpp"
#include "interval_bisection.h"
#include "calculo_fechas.h"
#include "calculos_estadisticos.h"


void calcularVolImplicitayRetornos();
void calcularVolHistorica(int);
int contarRegistros(char*);
float obtenerVI(char*, float, float);
float obtenerRetornos(string, string);
float devolver_variable_por_posicion(string,int);
string devolver_variable_por_posicion_str(string,int);
int obtenerPrimerosDosDig(string);
int obtenerSegundosDosDig(string);
void limpiarCsvYCalcularPrecioProm();


int main(int argc, char **argv) {
    try{
            printf("Entra a limpiar archivo y calcular promedios. \n");

            //Limpia el CSV de valores erroneos, le asigna variable numérica y calcula el promedio de precio_call y precio_suby
            limpiarCsvYCalcularPrecioProm();

            //Calculo la Volatilidad Implícita y los retornos
            printf("-Calcula la Volatilidad Implicita.\n");
            calcularVolImplicitayRetornos();

            printf("-Calcula la Volatilidad Realizada.\n");
            calcularVolHistorica(5);

            printf("*Procesamiento sin errores.\n");
            printf("\nSe genero el archivo: volatilidades.csv\n");


      return 0;
      }
      catch (exception& ex){
           printf("Se produjo un error en la ejecución. Error: \n",ex);
      return 99999;
      }
}

//Recorre el csv línea por línea y devuelve un csv con los retornos y la vol implícita
void limpiarCsvYCalcularPrecioProm(){
    try{
           char nombreArchivoEnt[] = "Exp_Octubre.csv";
           char nombreArchivoSal[] = "archivo_auxiliar.csv";

            int n = contarRegistros(nombreArchivoEnt);

            //Apertura de archivos
            FILE *pEnt = fopen(nombreArchivoEnt,"r");
            if(pEnt==NULL){
                printf("Problema al abrir el archivo %s. Termina el programa.\n",nombreArchivoEnt);
                exit(-1);
            }
            FILE *pSal = fopen(nombreArchivoSal,"w");
            if(pSal==NULL){
                printf("Problema al crear el archivo %s. Termina el programa.\n",nombreArchivoSal);
                exit(-1);
            }


            char linea[1000];
            fgets(linea,sizeof(linea),pEnt); //Titulos de campos

            linea[strlen(linea)-1] = '\0'; //Colocamos el terminador de cadena en lugar del salto de linea

            //Agregamos las dos columnas de promedio de precios de call y subyacente.
            fprintf(pSal,"%s;precio_call; precio_suby\n",linea);

            float precio_call,precio_call_bid,precio_call_ask,precio_suby,precio_suby_bid,precio_suby_ask;
            int i;
            for(i=0;i<n+1;i++){
                fgets(linea,1000,pEnt); //Leemos la linea correspondiente a un alumno
                linea[strlen(linea)-1] = '\0'; //Sacamos el salto de linea

                precio_call_bid = devolver_variable_por_posicion(linea,3);
                precio_call_ask = devolver_variable_por_posicion(linea,4);
                precio_suby_bid = devolver_variable_por_posicion(linea,5);
                precio_suby_ask = devolver_variable_por_posicion(linea,6);

                //Imprime la línea con el tipo de dato correcto y si no tiene errores
                if (precio_call_bid!=99999 & precio_call_ask!=99999 & precio_suby_bid!=99999 & precio_suby_ask!=99999){
                    precio_call=(precio_call_ask+precio_call_bid)/2;
                    precio_suby=(precio_suby_bid+precio_suby_ask)/2;

                    //Imprime la línea con las nuevas columnas de precio call y suby
                    fprintf(pSal,"%s;%.3f;%.3f;\n",linea,precio_call, precio_suby);
                }
                }
            //Cierre de archivos
            fclose(pEnt); fclose(pSal);
      }
      catch (exception& ex){
           printf("Se produjo un error en la ejecución.En limpiarCsvYCalcularPrecioProm(). \n Error: \n",ex);
      }
}
//Recorre el csv línea por línea y devuelve un csv con los retornos y la vol implícita
void calcularVolImplicitayRetornos(){
      try{
           char nombreArchivoEnt[] = "archivo_auxiliar.csv";
           char nombreArchivoSal[] = "archivo_auxiliar_2.csv";

            int n = contarRegistros(nombreArchivoEnt);

            //Apertura de archivos

            FILE *pEnt = fopen(nombreArchivoEnt,"r");
            if(pEnt==NULL){
                printf("Problema al abrir el archivo %s. Termina el programa.\n",nombreArchivoEnt);
                exit(-1);
            }
            FILE *pSal = fopen(nombreArchivoSal,"w");
            if(pSal==NULL){
                printf("Problema al crear el archivo %s. Termina el programa.\n",nombreArchivoSal);
                exit(-1);
            }


            char linea[1000];
            fgets(linea,sizeof(linea),pEnt); //Titulos de campos

            linea[strlen(linea)-1] = '\0'; //Colocamos el terminador de cadena en lugar del salto de linea

            //Agregamos las dos columnas
            fprintf(pSal,"%s;retornos_norm;vol_imp\n",linea);

            //Luego, recorremos la lista
            float retornos_norm, vol_imp;
            string linea_anterior;
            int i;
            for(i=0;i<n+1;i++){
                fgets(linea,1000,pEnt); //Leemos la linea correspondiente a un alumno
                linea[strlen(linea)-1] = '\0'; //Sacamos el salto de linea

                //Arma la serie de VI con tasa libre de riesgo 90% y Strike 1033
                vol_imp=obtenerVI(linea, 0.9,1033.0);

                retornos_norm=obtenerRetornos(linea, linea_anterior);
                linea_anterior=linea;

                fprintf(pSal,"%s%.4f;%.4f\n",linea,retornos_norm,vol_imp);
            }
            //Cierre de archivos
            fclose(pEnt); fclose(pSal);
            remove("archivo_auxiliar.csv");
      }
      catch (exception& ex){
           printf("Se produjo un error en la ejecución.En calcularVolImplicitayRetornos(). \n Error: \n",ex);
      }
}
//Calcula la Vol Histórica y devuelve el resultado en el csv
void calcularVolHistorica(int nroRondas){
    try{
           char nombreArchivoEnt[] = "archivo_auxiliar_2.csv";
           char nombreArchivoSal[] = "volatilidades.csv";

            int n = contarRegistros(nombreArchivoEnt);

            FILE *pEnt = fopen(nombreArchivoEnt,"r");
            if(pEnt==NULL){
                printf("Problema al abrir el archivo %s. Termina el programa.\n",nombreArchivoEnt);
                exit(-1);
            }

            FILE *pSal = fopen(nombreArchivoSal,"w");
            if(pSal==NULL){
                printf("Problema al crear el archivo %s. Termina el programa.\n",nombreArchivoSal);
                exit(-1);
            }

            //Primero llegamos a la segunda linea
            char linea[1000];
            fgets(linea,sizeof(linea),pEnt); //Titulos de campos

            //Recordemos que el fgets tambien almacena el salto de linea!!
            linea[strlen(linea)-1] = '\0'; //Colocamos el terminador de cadena en lugar del salto de linea

           //Luego, recorremos la lista
            float rendimiento, vol_hist;
            float rendimientos[n+1];
            int i;
            //Recorro el archivo de salida creado anteriormente para armar una lista de los rendimientos
            for(i=0;i<n+1;i++){
                fgets(linea,1000,pEnt); //Leemos la linea correspondiente a un alumno
                linea[strlen(linea)-1] = '\0'; //Sacamos el salto de linea

                rendimiento=devolver_variable_por_posicion(linea,10);
                rendimientos[i]=rendimiento;
            }
            //Cierro el archivo
            fclose(pEnt);

            //Abro el archivo nuevamente para realidad el cálculo de volatilidades históricas con la lista de rendimientos[i]
             pEnt = fopen(nombreArchivoEnt,"r");

            //Primero llegamos a la segunda linea
            char linea2[1000];
            fgets(linea2,sizeof(linea2),pEnt); //Titulos de campos

            linea2[strlen(linea2)-1] = '\0'; //Colocamos el terminador de cadena en lugar del salto de linea

            //Agregamos el título de la columna: vol_hist
            fprintf(pSal,"%s;vol_hist\n",linea2);

            int j,contador_rondas,indice_rend;

            //el contador "indice_rend" marcará el inicio del seguiente bloque de rendimientos para el calculo de la vol hist
            indice_rend=1;
            contador_rondas=0;

            //Creo los rendimientos según las rondas
            float rend_n_rondas[nroRondas];

            //Recorro línea por línea e imprimo la vol hist
            for(i=0;i<n+1;i++){
                fgets(linea2,1000,pEnt); //Leemos la linea i
                linea2[strlen(linea2)-1] = '\0'; //Sacamos el salto de linea

                vol_hist=0; //Asigno 0 a las primeras n rondas
                contador_rondas++;
               if(contador_rondas>nroRondas){

                    //Construyo una lista de n redimientos para luego calcular su desvío estándar
                    for(j=0;j<nroRondas;j++){
                            rend_n_rondas[j]=rendimientos[indice_rend+j];
                    }
                    //Creo la vol hist en base a n rondas(5min), anualizando con 260 rondas. 260*24*60/5=74880
                    vol_hist=devolver_desvio_estandar(rend_n_rondas)*sqrt(74880);
                    indice_rend++;
                 }
                //Imprimlo línea en archivo
                fprintf(pSal,"%s;%.4f\n",linea2,vol_hist);
            }
            //Cierre de archivos
            fclose(pEnt); fclose(pSal);

            remove("archivo_auxiliar_2.csv");
      }
      catch (exception& ex){
           printf("Se produjo un error en la ejecución.En calcularVolHistorica(). \n Error: \n",ex);
      }
}

//Obtengo Volatilidad Implícita con formula Black-Scholes y el método de bisección de intervalos.
/*Método de bisección, también llamado dicotomía, es un algoritmo de búsqueda de raíces que trabaja dividiendo el intervalo a la mitad
y seleccionando el subintervalo que tiene la raíz.
*/
float obtenerVI(char *linea, float tlb,float strike){
        try{
            int mes,dia;
            float precio_call,precio_suby;
            string fecha;
            string linea_actual2=linea;

            fecha=devolver_variable_por_posicion_str(linea_actual2,7);
            mes=obtenerPrimerosDosDig(fecha);
            dia=obtenerSegundosDosDig(fecha);

            precio_call = devolver_variable_por_posicion(linea_actual2,8);
            precio_suby = devolver_variable_por_posicion(linea_actual2,9);

            Fecha fecha1 = { dia,mes, 2023 };

            //Fecha de cierre de la opción
            Fecha fecha2 = { 20,10, 2023 };
            double maturity=float(cantidad_dias_entre_fechas(fecha1,fecha2))/float(360);

            // Función BlackScholes
            BlackScholesCall bsc(precio_suby, strike, tlb, maturity);

            double low_vol = 0.1;
            double high_vol = 100;
            double episilon = 0.01;

            // Calcula la volatilidad implícita utilizando recursivamente la función BlackScholesCall
            // hasta que la prima sea la del precio del archivo de entrada.
            double sigma = interval_bisection(precio_call, low_vol, high_vol, episilon, bsc);
            //double sigma =0;

        return sigma;
      }
      catch (exception& ex){
           printf("Se produjo un error en la ejecución.En obtenerVI(). \n Error: \n",ex);
      }
}

//Devuelve la columna de la línea según numero de coma se encuentre.
string devolver_variable_por_posicion_str(string linea,int comas){
      try{
            //Contaremos el nro de comas hasta llegar a la coma establecida como parámetro de la función
            int nroComas = 0,i=0;
            while(nroComas<comas){ //No verificamos que lleguemos al final de la linea (suponemos el formato correcto)
                if(linea[i]==';') nroComas++;
                i++;
            }
            char nro[100]; //Cargaremos el nro que está en esa parte de la línea
            int j=0;
            while(linea[i]!=';') nro[j++] = linea[i++]; //Copiamos el número (como cadena)
            nro[j] = '\0'; //caracter de terminación de la cadena

            return nro;
        }catch (exception& ex){
               printf("Se produjo un error en la ejecución.En devolver_variable_por_posicion_str(). \n Error: \n",ex);
        }
}

//Devuelve la columna de la línea según numero de coma se encuentre.
float devolver_variable_por_posicion(string linea,int comas){
        try
        {
            float resultado;
            //Contaremos el nro de comas hasta llegar a la coma establecida como parámetro de la función
            int nroComas = 0,i=0;
            while(nroComas<comas){ //No verificamos que lleguemos al final de la linea (suponemos el formato correcto)
                if(linea[i]==';') nroComas++;
                i++;
            }

             //Ahora trabajaremos con el primer numero
            char nro[100]; //Cargaremos el nro que está en esa parte de la línea
            int j=0;
            while(linea[i]!=';') {
                    nro[j++] = linea[i++]; //Copiamos el número (como cadena
            }
            nro[j] = '\0'; //caracter de terminación de la cadena

           //Reemplazo coma por punto para que reconozca el tipo float.
            size_t position;
            string aux_string=nro;
            while ((position = aux_string.find(',')) != std::string::npos) {
                aux_string.replace(position, 1, ".");
            }
            resultado = stof(aux_string);

        return resultado;
        }
        catch (exception& ex){
           // Si no es un número devuelve 99999 para que no corte el ciclo y elimine la linea con error
           // cout << "Elimino Dato Erroneo " << endl;
            return 99999;
        }
    }

//Obtengo Retornos Normalizados
float obtenerRetornos(string linea, string linea_anterior){
        float precio_suby,precio_suby_anterior, retorno;

        precio_suby = devolver_variable_por_posicion(linea,9);
        precio_suby_anterior=devolver_variable_por_posicion(linea_anterior,9);

        retorno=log(precio_suby/precio_suby_anterior);
    return retorno;
}

/*Función que recibe el nombre del archivo y devuelve la cantidad de registros*/
int contarRegistros(char *nombreArch){
        FILE *pa = fopen(nombreArch,"r");
        if(pa==NULL){
            exit(-1);
        }
        int n=0; //cantidad de registros
        char c = fgetc(pa); //Recorremos caracter por caracter
        while(!feof(pa)){
            if(c=='\n') n++;
            c = fgetc(pa);
        }
        fclose(pa);
    return n-1;     //porque en la primera linea están los nombres de los campos
}

int obtenerPrimerosDosDig(string fecha){
       string var_cadena_string;
       char var_cadena[3];
       int mes, i;
         for(i = 0; i < 2; i++){
            var_cadena[i] = +fecha[i];
        }
        var_cadena_string=var_cadena;

        string chars = "/";
        for (char c: chars) {
            var_cadena_string.erase(std::remove(var_cadena_string.begin(), var_cadena_string.end(), c), var_cadena_string.end());
          }
        mes=stoi(var_cadena_string);
    return mes;
}

int obtenerSegundosDosDig(string fecha){
       string var_cadena_string;
       char var_cadena[3];
       int mes, i;
         for(i = 0; i < 2; i++){
            var_cadena[i] = +fecha[i+2];
        }
        var_cadena_string=var_cadena;
        string chars = "/";
          for (char c: chars) {
            var_cadena_string.erase(std::remove(var_cadena_string.begin(), var_cadena_string.end(), c), var_cadena_string.end());
         }
         mes=stoi(var_cadena_string);
    return mes;
}
#endif


