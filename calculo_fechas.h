using namespace std;

#ifndef __CALCULO_FECHAS_H
#define __CALCULO_FECHAS_H

#include <iostream>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <list>
#include <vector>
#include <cstring>
#include <stdexcept>

typedef struct{
	int dia;
	int mes;
	int anio;
}Fecha;

int cantidad_dias_entre_fechas(Fecha, Fecha);
int regularBisiesto( int tFecha );

int regularBisiesto( int tFecha ){
	if ((tFecha %4 == 0 && tFecha%100 != 0) || (tFecha%400) == 0){
		return 1;
	}
	else{
		return 0;
	}
}

int devolverPrimerosDosDigitos(string fecha){
        int primerosDigitos,dia,i;
        char var_cadena[3];
        string var_cadena_string;
        string chars = "/";

         for(i = 0; i < 2; i++){
            var_cadena[i] = +fecha[i];
        }

        var_cadena_string=var_cadena;
          for (char c: chars) {
         //   var_cadena_string.erase(std::remove(var_cadena_string.begin(), var_cadena_string.end(), c), var_cadena_string.end());
          }
         primerosDigitos=stoi(var_cadena_string);

        return primerosDigitos;
}
//Devuelve la cantidad de días entre dos Fechas
int cantidad_dias_entre_fechas(Fecha fecha1, Fecha fecha2){
	int difAnios, difDias=0, i, regBisAnioF1 = 0, regBisAnioF2, tFecha1, tFecha2;
	tFecha1 = fecha1.anio;
	tFecha2 = fecha2.anio;
	int tot_dias[] = { 00,31,28,31,30,31,30,31,31,30,31,30,31 };

	difAnios = fecha2.anio - fecha1.anio;
	regBisAnioF1 = regularBisiesto(tFecha1);

	if( difAnios == 0){
		if( fecha1.mes == fecha2.mes ){
			difDias += fecha2.dia - fecha1.dia;
		}
		else{
			difDias += (tot_dias[fecha1.mes] - fecha1.dia);
			if( fecha1.mes == 2 ){
				difDias += regBisAnioF1;
			}
			for( i=fecha1.mes+1; i < fecha2.mes; i++){
				if( i == 2 ){
					difDias += regBisAnioF1;
				}
				difDias += tot_dias[i];
			}
			difDias += fecha2.dia;
		}
	}
	else{
		difDias += (tot_dias[fecha1.mes] - fecha1.dia);
		if(fecha1.mes == 2){
			difDias += regBisAnioF1;
		}
		for( i = fecha1.mes +1; i <= 12; i++ ){
			difDias += tot_dias[i];
		}
		for( i=tFecha1 + 1; i < fecha2.anio ; i++){
			tFecha1++;
			difDias += 365;
			regBisAnioF1 = regularBisiesto(tFecha1);
			difDias += regBisAnioF1;
		}
		regBisAnioF2 = regularBisiesto(tFecha2);
		for( i=1; i < fecha2.mes; i++ ){
			if( i == 2 ){
				difDias += regBisAnioF2;
			}
			difDias += tot_dias[i];
		}
		difDias += fecha2.dia;
	}


	//getchar();
	return difDias;
}


#endif
