/*
 * teclado.h
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */

#ifndef INC_TECLADO_H_
#define INC_TECLADO_H_
#define tcl_puerto GPIOE
#define tcl_pin_x4 GPIO_PIN_15
#define tcl_pin_x3 GPIO_PIN_14
#define tcl_pin_x2 GPIO_PIN_13
#define tcl_pin_x1 GPIO_PIN_12
#define tcl_pin_y4 GPIO_PIN_11
#define tcl_pin_y3 GPIO_PIN_10
#define tcl_pin_y2 GPIO_PIN_9
#define tcl_pin_y1 GPIO_PIN_8

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void PruebaConLed();
void reiniciar_filas_tcl();
#endif /* INC_TECLADO_H_ */
