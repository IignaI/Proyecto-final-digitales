/*
 * luces.c
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */
#include "stm32f4xx_hal.h"
#include "luces.h"

uint32_t pwm_buffer[START_BITS + TOTAL_BITS + RESET_BITS]; //tamaño de mi buffer de datos que depende del tamaño del tablero
uint32_t tablero_interno[NUM_LEDS];
extern volatile int ocupado ;

#define PWM_BIT_0  5   // 400ns
#define PWM_BIT_1  10  // 800ns

void actualizar_matriz(TIM_HandleTypeDef *htim, uint32_t *lista_colores) { // <-- NUEVO: Volvemos a pedir uint32_t*

    for (int i = 0; i < (START_BITS + TOTAL_BITS + RESET_BITS); i++) { // limpio la matriz entera
        pwm_buffer[i] = 0;
    }

    int index = START_BITS;

    for (int led = 0; led < NUM_LEDS; led++) {

        // 1. Agarramos el color completo de 32 bits directo de la lista
        uint32_t color_original = lista_colores[led];

        // 2. Extraemos los canales limpios de 8 bits cada uno
        uint8_t r = (color_original >> 16) & 0xFF;
        uint8_t g = (color_original >> 8)  & 0xFF;
        uint8_t b =  color_original        & 0xFF;

        // 3. Los armamos en formato GRB de 24 bits nativos que exige el LED físico
        uint32_t color_nativo_24bits = (g << 16) | (r << 8) | b;

        // 4. Mandamos los 24 bits al buffer PWM como hacías antes
        for (int bit = 23; bit >= 0; bit--) {
            if ((color_nativo_24bits >> bit) & 1) {
                pwm_buffer[index] = PWM_BIT_1;
            } else {
                pwm_buffer[index] = PWM_BIT_0;
            }
            index++;
        }
    }

    HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 0);

    HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_2, (uint32_t*)pwm_buffer, START_BITS + TOTAL_BITS + RESET_BITS);
}

void mapeo(uint32_t matriz[FILAS][COLUMNAS], uint32_t tablero[NUM_LEDS]){
	 int indice_vector1 = 0;   // Bloque de abajo: ocupa las posiciones 0 a 15
		  int indice_vector2 = 16;  // Bloque de arriba: ocupa las posiciones 16 a 31

		  // 1. Recorremos las columnas de derecha a izquierda (de la 3 a la 0)
		  for (int col = COLUMNAS - 1; col >= 0; col--) {

		      // 2. Módulo de abajo: Subimos desde la fila 7 a la 4
		      for (int fila = FILAS - 1; fila >= 4; fila--) {
		          tablero[indice_vector1] = matriz[fila][col];
		          indice_vector1++; // Avanza acá adentro por cada led de abajo
		      }

		      // 3. Módulo de arriba: Subimos desde la fila 3 a la 0
		      for (int fila = 3; fila >= 0; fila--) {
		          tablero[indice_vector2] = matriz[fila][col];
		          indice_vector2++;
		      }
		  }
}

void escribir(uint32_t matriz[FILAS][COLUMNAS], TIM_HandleTypeDef *htim) {
	static uint32_t matrizi[FILAS][COLUMNAS] = {0};
	int c=0;
	int f=0;

	for (int i = 7; i >= 0; i--) {
	            for (int j = 0; j < 4; j++) {
	            	switch (j){
	            	case 0:
	            	        f=2;
	            	        break;
	            	    case 1:
	            	        f=4;
	            	        break;
	            	    case 2:
	            	        f=0;
	            	        break;
	            	    case 3:
	            	        f=1;
	            	        break;
	            	}
	            	if (matriz[i][j] == 1 ) {
	            		matrizi[c][j] = 0x00000002;
	            }
	            	if (matriz[i][j] == 2 ) {
	            		matrizi[c][j] = 0x00000200;
	            }
	        }
	c++;
	}
    mapeo(matrizi, tablero_interno);
    actualizar_matriz(htim, tablero_interno);
}

void caer_en_columna(uint32_t matriz[FILAS][COLUMNAS], int columna_elegida, uint32_t color, TIM_HandleTypeDef *htim) {

    // VARIABLES ESTÁTICAS (Mantienen su memoria interna entre llamadas)
    static int i = 0;
    static int fila_destino = -1;
    static int animacion_activa = 0;
    static uint32_t tiempo_anterior = 0;


    if (ocupado == 1 && animacion_activa == 0) {
            return;
        }

    // Busco el 0 más bajo en la columna (Solo la primera vez)
    if (animacion_activa == 0) {
        fila_destino = -1;

        for (int fila = FILAS - 1; fila >= 0; fila--) {
            if (matriz[fila][columna_elegida] == 0x000000) {
                fila_destino = fila;
                break;
            }
        }

        // Si la columna está llena me salgo
        if (fila_destino == -1) {
            ocupado = 0;
            return;
        }

        i = 0;
        animacion_activa = 1;
        ocupado = 1;
        tiempo_anterior = HAL_GetTick();
    }


    if (HAL_GetTick() - tiempo_anterior < 250) {
        return;
    }

    tiempo_anterior = HAL_GetTick();

    // cae una posicion
    if (i <= fila_destino) {
        matriz[i][columna_elegida] = color;

        // Elimino la posición anterior
        if (i > 0) {
            matriz[i - 1][columna_elegida] = 0;
        }

        escribir(matriz, htim);

        i++;
    }
    else {
        // llego al limite
        animacion_activa = 0;
        ocupado = 0;
    }
}

void animacion_victoria(uint32_t matriz[FILAS][COLUMNAS], int f1, int c1, int f2, int c2, int f3, int c3, TIM_HandleTypeDef *htim) {

    // variables para control de tiempo y parpadeo
    static uint32_t tiempo_anterior_vic = 0;
    static int estado_luces = 0; // 0 = Apagar fichas, 1 = Encender fichas
    static int ciclos = 0;

    // variable para color original
    static uint32_t col = 0;

    // si se estaba ocupado se sale
    if (ocupado == 1 && ciclos == 0) {
        return;
    }

    // inicializacion
    if (ciclos == 0 && estado_luces == 0) {
        ocupado = 1;
        col = matriz[f1][c1];
        tiempo_anterior_vic = HAL_GetTick();

        //apago posicion ganadora
        matriz[f1][c1] = 0x00000000;
        matriz[f2][c2] = 0x00000000;
        matriz[f3][c3] = 0x00000000;

        estado_luces = 1;
        ciclos = 1;

        escribir(matriz, htim);
    }

    // condicion final
    if (ciclos >= 21) {
        matriz[f1][c1] = col;
        matriz[f2][c2] = col;
        matriz[f3][c3] = col;

        ciclos = 0;
        estado_luces = 0;
        ocupado = 0;

        // Enviamos al hardware el tablero final restaurado (una sola vez)
        escribir(matriz, htim);
        return;
    }

    // reviso si paso el tiempo minimo
    if (HAL_GetTick() - tiempo_anterior_vic >= 300) {
        tiempo_anterior_vic = HAL_GetTick();
        // apago leds
        if (estado_luces == 0) {
            matriz[f1][c1] = 0x00000000;
            matriz[f2][c2] = 0x00000000;
            matriz[f3][c3] = 0x00000000;

            estado_luces = 1;
            ciclos++;
        }
        else if (estado_luces == 1) {
            // enciendo leds
            matriz[f1][c1] = col;
            matriz[f2][c2] = col;
            matriz[f3][c3] = col;

            estado_luces = 0;
            ciclos++;
        }

        escribir(matriz, htim);
    }
}

void fin(uint32_t matriz[FILAS][COLUMNAS], TIM_HandleTypeDef *htim) {

    static uint32_t tiempo_anterior_caida = 0;
    static int desplazamientos_hechos = 0;
    const uint32_t VELOCIDAD_CAIDA = 250;

    if (ocupado == 1 && desplazamientos_hechos == 0) {
        return;
    }

    // inicializacion
    if (desplazamientos_hechos == 0) {
        ocupado = 1;
        tiempo_anterior_caida = HAL_GetTick();
        desplazamientos_hechos = 1; // Arranca el primer paso
    }

    // final
    if (desplazamientos_hechos > 8) {
        desplazamientos_hechos = 0;
        ocupado = 0;
        return;
    }

    // control de tiempo minimo
    if (HAL_GetTick() - tiempo_anterior_caida >= VELOCIDAD_CAIDA) {
        tiempo_anterior_caida = HAL_GetTick();

        // logica de desplazamnmiento (guarda en la fila lo que hay en la fila anterior )
        for (int i = 7; i > 0; i--) {
            for (int j = 0; j < 4; j++) {
                matriz[i][j] = matriz[i - 1][j];
            }
        }

        // Limpiar la fila 0 (para que el valor no baje repitiendose)
        for (int j = 0; j < 4; j++) {
            matriz[0][j] = 0;
        }

        escribir(matriz, htim);

        desplazamientos_hechos++;
    }
}
