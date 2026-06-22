/*
 * teclado.h
 *
 *  Created on: Jun 20, 2026
 *      Author: marcos
 */

#ifndef INC_TECLADO_H_
#define INC_TECLADO_H_
#include "stdio.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void rutina_leer_filas(uint16_t pin_x);
void reiniciar_filas_tcl();
void delay_bruto(volatile uint32_t ciclos);
void PruebaConLed();

#endif /* INC_TECLADO_H_ */
