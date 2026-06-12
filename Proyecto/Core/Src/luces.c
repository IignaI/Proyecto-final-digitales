/*
 * luces.c
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */
#include "stm32f4xx_hal.h"

void tablero(TIM_HandleTypeDef timer){
// Obtenemos el valor máximo de ciclo de trabajo de la configuración del hgardware
uint16_t ciclomaximo = TIM4->ARR; // Valor máximo de ciclo de trabajo
// Configuramos el PWM para que inicie con los LED encendidos
TIM4->CCR2 = ciclomaximo; // LED rojo al 100 %
// Arrancamos el PWM

HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_2); // Inicio de la modulación PWM, LED rojo
// Mantenemos los LED encendidos por un tiempo
HAL_Delay(1000); // Retardo de TENCENDIDO milisegundos
// Apagamos los LED
TIM4->CCR2 = 0; // LED rojo al 0 %
// Mantenemos los LED apagados por un tiempo
HAL_Delay(1000); // Retardo de TAPAGADO milisegundos
//Encendemos el LED rojo
TIM4->CCR2 = ciclomaximo; // LED rojo al 100 %
}

