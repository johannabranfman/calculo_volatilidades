






## Resumen

Este programa analiza la opción call GFGC1033OC mediante el modelo Black-Scholes. Se obtienen datos de la cotización de este activo en sus precios bid y ask de la prima y el activo subyacente a partir del archivo Exp_Octubre.csv. Se calculan la volatilidad implícita y realizada del activo y guarda lo producido en el archivo  volatilidades.csv.

## Supuestos del modelo

 - La opción es de tipo europea. Sólo puede ejecutarse al vencimiento.
 - No hay costos de transacción o impuestos.
 - Tasa de Interés constante de 90%.
  - La acción no paga a dividendos.
  - Vencimiento de la opción call 20/10/2023.
  - Fecha presente se debe tomar 08/09/2023.
  - Precio Strike: $1033.

## Consideraciones

La fórmula Black-Scholes permite el cálculo del valor de la prima a partir de los datos de volatilidad constante y conocida, el precio del activo subyacente, precio strike, la tasa de interés libre de riesgo, la fecha de valuación y la fecha de vencimiento.

Como no hay una forma directa de despejar la volatilidad de esta fórmula, para el cálculo de la volatilidad implícita, este programa utiliza la metodología bisección, que es un algoritmo de búsqueda de raíces. En este caso, el programa utilizará esta metodología para iterar recursivamente la fórmula Black-Scholes a través de una función hasta alcanzar el valor de la prima buscada.

##  Salidas

 - Serie temporal de la volatilidad implícita correspondiente a cada precio de opción Call de la entrada.
 - Serie temporal de la volatilidad realizada correspondiente al activo subyacente a partir de la información de 5 rondas pasadas por registro.

 - A partir de el output, se construye un gráfico en Power Bi para visualizar las series mediante un gráfico de la evolución de las volatilidades en días.

##  Detalles:

Los datos están en minutos y el formato de la fecha es “mm/dd/aaaa hh:mm”.

La volatilidad realizada se calcula a partir de los retornos normalizados de 5 valores anteriores.

## **Procedimiento del programa**

**1)** Se realiza una limpieza del archivo cvs, omitiendo los registros que contengan valores incorrectos y se calcula el precio_call y el precio_subyacente a través de promedios ask y bid.

Este paso se ejecuta mediante la función **limpiarCsvYCalcularPrecioProm**().

Devuelve un archivo auxiliar con esta limpieza para no pisar el archivo original.


**2)** Se realiza el cálculo de volatilidad implícita recorriendo el archivo auxiliar línea por línea. Utilizando la metodología Black Scholes que se ejecutará en forma recursiva (mediante el método interval_bisection) hasta hallar el precio_call correspondiente y obteniendo así la volatilidad por línea.

Se procede al cálculo de volatilidad implícita mediante la función **calcularVolImplicitayRetornos**(). La misma utiliza funciones de los archivos:

-black_scholes.h Declaración de las funciones.
-black_scholes.cpp: Implementación de la fórmula Black Scholes.
-interval_bisection.h: Metodología Inverval Bisection para utilizar recursivamente una función hasta hallar un valor objetivo, en este caso el precio_call.
-calculo_fechas.h: Funciones de manejo de fechas para el cálculo de la maturity.

**3)** Se procede al cálculo de la volatilidad histórica mediante la función calcularVolHistorica(int).
El parámetro de esta función refiere al número de rondas a considerar en la volatilidad realizada.
Se utilizarán 5 operaciones previas que representan 5 minutos anteriores.

Para anualizar la volatilidad, en base a 5 minutos, se calculó el desvío estándar de los retornos normalizados de los 5 registros previos y se multiplicó por la raíz de 74.880 (anualizando con 260 rondas. 260*24*60/5=74880) para anualizar la volatilidad.

Esta función utiliza el siguiente archivo:
-calculos_estadisticos.h: Función que calcula el desvío estándar a partir de una lista.
Luego de realizar los cálculos se elimina el archivo auxiliar.

Resultado de la ejecución: **volatilidades.csv**

## Volatilidad Implícita vs. Volatilidad Realizada**

******Análisis de Resultados******

A continuación, se muestran los gráficos de las volatilidades con las variaciones de precio en el activo subyacente, Gráfico 1, y con el precio call y valor temporal en el Gráfico 2.

**Gráfico** **1**

![enter image description here](https://github.com/johannabranfman/calculo_volatilidades/blob/main/Grafico%201.png)

**Gráfico** **2**
![enter image description here]((https://github.com/johannabranfman/calculo_volatilidades/blob/main/Grafico%202.png)



## **Discrepancia significativas entre Volatilidad Implícita y Realizada.**

**En 8y 11 de septiembre**

En estos días, la volatilidad realizada estaba muy por encima de la implícita. Se podría esperar que en el futuro se produjera un incremento de volatilidad. Podría ser un indicador de señal de compra de la opción o o realizar spreads que se beneficien de incrementos de volatilidad. También coincide con el valor más alto del valor temporal donde la opción llega a encontrarse at the money.

**12 de septiembre al 9 de octubre**

La volatilidad implícita presenta una forma monótona decreciente. La volatilidad realizada se encuentra por debajo de la implícita y suele converger a ella. En este rango, la expectativa está en que la volatilidad futura cayera. Podría ser indicador para vender opciones o realizar estrategias que se aprovechen de los descensos de volatilidad.

**10 al 20 de octubre**

A partir del 10 de octubre coincide la importante caída de volatilidad implícita con la caída del valor temporal de la opción.
