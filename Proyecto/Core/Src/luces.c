/*
 * luces.c
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */
#include "stm32f4xx_hal.h"
#include "luces.h"

uint32_t pwm_buffer[START_BITS + TOTAL_BITS + RESET_BITS]; //tamaño de mi buffer de datos que depende del tamaño del tablero

#define PWM_BIT_0  5   // 400ns
#define PWM_BIT_1  10  // 800ns

void actualizar_matriz(TIM_HandleTypeDef *htim, uint16_t *lista_colores) {

    for (int i = 0; i < (START_BITS + TOTAL_BITS + RESET_BITS); i++) { //limpio la matriz entera
        pwm_buffer[i] = 0;
    }


    int index = START_BITS;  // establezco a partir de quepunto delbufferse mandan los datos (inicia en0 para evitar errores por ruidos)


    for (int led = 0; led < NUM_LEDS; led++) {  // convierto los valores del vector de entrada en un numero binario que representa los colores

        uint16_t color_rgb565 = lista_colores[led];

        uint8_t r = ((color_rgb565 >> 11) & 0x1F) << 3;
        uint8_t g = ((color_rgb565 >> 5)  & 0x3F) << 2;
        uint8_t b = (color_rgb565         & 0x1F) << 3;

        uint32_t color_nativo_24bits = (g << 16) | (r << 8) | b;

        for (int bit = 23; bit >= 0; bit--) { // le asigno al buffer los ciclos de trabajo de cada bit del numero generado
            if ((color_nativo_24bits >> bit) & 1) {
                pwm_buffer[index] = PWM_BIT_1;
            } else {
                pwm_buffer[index] = PWM_BIT_0;
            }
            index++;
        }
    }

    HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2);    // presetea el dma frenando cualquier transmicion o cosa anterior y poniendolo en 0
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 0);



    HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_2, (uint32_t*)pwm_buffer, START_BITS + TOTAL_BITS + RESET_BITS); // mando el buffer que arme con todos los ciclos de trabajo de los leds

}
