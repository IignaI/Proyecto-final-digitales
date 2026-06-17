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

    // ciclo de trabajo al 50%
    timer->Instance->CCR1 = (ciclomaximo ) / 2;

    // Arrancamos el PWM en el Canal 1 usando el puntero del timer
    HAL_TIM_PWM_Start(timer, TIM_CHANNEL_1);

    // Mantenemos el buzzer sonando por 1 segundo
    HAL_Delay(1000);

    // Para apagar el sonido de forma limpia, usamos la función de la HAL
    HAL_TIM_PWM_Stop(timer, TIM_CHANNEL_1);

    // Mantenemos el silencio por 1 segundo
    HAL_Delay(1000);
}
