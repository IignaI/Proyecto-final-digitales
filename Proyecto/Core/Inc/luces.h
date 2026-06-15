/*
 * luces.h
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */

#ifndef INC_LUCES_H_
#define INC_LUCES_H_
#define COLUMNAS     4   // Cantidad de columnas
#define FILAS      8   // Cantidad de filas
#define MATRIZ_ANCHO     4   // Cantidad de columnas
#define MATRIZ_ALTO      8   // Cantidad de filas
#define BITS_PER_LED  	 24
#define BITS_RESET_NETO  40	 // 40bits * 1.25us/bit = 50us = tiempo necesario para  que se almacenen los datos
#define START_BITS		 40  // 40bits * 1.25us/bit = 50us  tiempo necesario para que el primer led no se encieda por ruido
#define NUM_LEDS      	 (MATRIZ_ANCHO * MATRIZ_ALTO)
#define TOTAL_BITS    	 (NUM_LEDS * BITS_PER_LED)
#define RESET_BITS 		 (TOTAL_BITS + BITS_RESET_NETO) //mado un 0 a todos los leds y le sumo 50us para el ultimo (si no lo hacia no me funcionaba)

extern uint32_t tablero_interno[NUM_LEDS];

void actualizar_matriz(TIM_HandleTypeDef *htim, uint32_t *lista_colores);
void mapeo(uint32_t matriz[FILAS][COLUMNAS], uint32_t tablero[NUM_LEDS]);
void escribir(uint32_t matriz[FILAS][COLUMNAS], TIM_HandleTypeDef *htim);
void caer_en_columna(uint32_t matriz[FILAS][COLUMNAS], int columna_elegida, uint32_t color, TIM_HandleTypeDef *htim);

#endif /* INC_LUCES_H_ */
