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
volatile int animacion_en_progreso = 0;
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
		          indice_vector2++; // Avanza acá adentro por cada led de arriba
		      }
		  }
}

void escribir(uint32_t matriz[FILAS][COLUMNAS], TIM_HandleTypeDef *htim) {

    mapeo(matriz, tablero_interno);
    actualizar_matriz(htim, tablero_interno);
}

void caer_en_columna(uint32_t matriz[FILAS][COLUMNAS], int columna_elegida, uint32_t color, TIM_HandleTypeDef *htim) {

    // VARIABLES ESTÁTICAS (Mantienen su memoria interna entre llamadas)
    static int i = 0;
    static int fila_destino = -1;
    static int animacion_activa = 0;

    // ¡NUEVA VARIABLE!: Guarda el tiempo del último paso de la animación
    static uint32_t tiempo_anterior = 0;

    // 1. PRIMER PASO: Busco el 0 más bajo en la columna (Solo la primera vez)
    if (animacion_activa == 0) {
        fila_destino = -1;

        for (int fila = FILAS - 1; fila >= 0; fila--) {
            if (matriz[fila][columna_elegida] == 0x000000) {
                fila_destino = fila;
                break;
            }
        }

        // Si la columna está llena, apagamos la bandera general y salimos
        if (fila_destino == -1) {
            animacion_en_progreso = 0;
            return;
        }

        i = 0;
        animacion_activa = 1;
        animacion_en_progreso = 1;
        tiempo_anterior = HAL_GetTick();
    }

//defino tiempo de caida
    if (HAL_GetTick() - tiempo_anterior < 250) {
        return; // saltamos la funcion
    }

    tiempo_anterior = HAL_GetTick(); // Actualizamos el cronómetro para el próximo paso

    // cae una posicion
    if (i <= fila_destino) {
        matriz[i][columna_elegida] = color;

        // Elimino la posición anterior
        if (i > 0) {
            matriz[i - 1][columna_elegida] = 0;
        }

        mapeo(matriz, tablero_interno);
        actualizar_matriz(htim, tablero_interno);

        i++; // Dejamos listo el índice para la próxima fila
    }
    else {
        // llego al limite
        animacion_activa = 0;       // Reseteamos el motor interno de tiempo
        animacion_en_progreso = 0;  // Le avisamos al main que el teclado ya puede revivir
    }
}

