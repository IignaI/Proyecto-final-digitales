/*
 * luces.h
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */

#ifndef INC_LUCES_H_
#define INC_LUCES_H_
#define MATRIZ_ANCHO  8   // Cantidad de columnas (Poné 8 para la matriz final)
#define MATRIZ_ALTO   4   // Cantidad de filas

// ==========================================
// CÁLCULOS AUTOMÁTICOS (No tocar)
// ==========================================
#define NUM_LEDS      (MATRIZ_ANCHO * MATRIZ_ALTO)
#define BITS_PER_LED  24
#define TOTAL_BITS    (NUM_LEDS * BITS_PER_LED)

void enviar_numero_binario(TIM_HandleTypeDef *htim, uint16_t *lista_colores);

#endif /* INC_LUCES_H_ */
