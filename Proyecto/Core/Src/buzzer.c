/*
 * buzzer.c
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */
#include "stm32f4xx_hal.h"
#include "buzzer.h"

uint16_t escala_arr[] = {47891, 42516, 37992, 35815, 31886, 28408, 25302, 23899};//do re mi fa sol la si do (creo)
void buzzer(TIM_HandleTypeDef *timer){
    // Obtenemos el valor máximo del período desde la configuración (12499)

	timer->Instance->ARR=escala_arr[0];
	uint16_t ciclomaximo = timer->Instance->ARR;
	static uint32_t tiempo_anteriorb = 0;
	static int encendido=0;
    // ciclo de trabajo al 50%
    timer->Instance->CCR1 = (ciclomaximo ) / 2;

    if (HAL_GetTick() - tiempo_anteriorb < 1000) {
            return;
        }

        tiempo_anteriorb = HAL_GetTick();
        if (encendido == 0) {
    // Arrancamos el PWM en el Canal 1 usando el puntero del timer
    HAL_TIM_PWM_Start(timer, TIM_CHANNEL_1);
    encendido =1;
        }
        else{
    // Mantenemos el buzzer sonando por 1 segundo

    // Para apagar el sonido de forma limpia, usamos la función de la HAL
    HAL_TIM_PWM_Stop(timer, TIM_CHANNEL_1);
    encendido =0;
    // Mantenemos el silencio por 1 segundo
        }
}
