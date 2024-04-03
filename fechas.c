
// Programa para establecer la diferencia en día(s) entre 2 fechas diferentes.

#include <stdio.h>

typedef struct{
	int dia;
	int mes;
	int anio;
}Fecha;

int regularBisiesto( int tFecha );


int main2(void){
	Fecha fecha1 = { 8,9, 2023 }, fecha2 = { 10,10, 2023 };
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

	printf("\n Del %d/%d/%d al %d/%d/%d hay %d dias", fecha1.dia, fecha1.mes, fecha1.anio,
			fecha2.dia, fecha2.mes, fecha2.anio, difDias);

	getchar();
	return 0;
}

int regularBisiesto( int tFecha ){
	if ((tFecha %4 == 0 && tFecha%100 != 0) || (tFecha%400) == 0){
		return 1;
	}
	else{
		return 0;
	}
}
