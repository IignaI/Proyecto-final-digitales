/*
 * luces.c
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */
#include "stm32f4xx_hal.h"
#include "luces.h"
// Agregamos un colchón de 50 bits de silencio al principio para absorber glitches de arranque
#define START_BITS 50
#define RESET_BITS 800

// El tamaño total del buffer ahora incluye el inicio, los datos y el final
uint32_t pwm_buffer[START_BITS + TOTAL_BITS + RESET_BITS];

#define PWM_BIT_0  5   // 400ns
#define PWM_BIT_1  10  // 800ns

void enviar_numero_binario(TIM_HandleTypeDef *htim, uint16_t *lista_colores) {
    // 1. Limpieza absoluta de todo el array (Todo arranca en cero estricto)
    for (int i = 0; i < (START_BITS + TOTAL_BITS + RESET_BITS); i++) {
        pwm_buffer[i] = 0;
    }

    // El índice de datos reales ya no arranca en 0, arranca DESPUÉS del colchón de inicio
    int index = START_BITS;

    // 2. Procesamiento de los 16 LEDs con su conversión
    for (int led = 0; led < NUM_LEDS; led++) {

        uint16_t color_rgb565 = lista_colores[led];

        uint8_t r = ((color_rgb565 >> 11) & 0x1F) << 3;
        uint8_t g = ((color_rgb565 >> 5)  & 0x3F) << 2;
        uint8_t b = (color_rgb565         & 0x1F) << 3;

        uint32_t color_nativo_24bits = (g << 16) | (r << 8) | b;

        for (int bit = 23; bit >= 0; bit--) {
            if ((color_nativo_24bits >> bit) & 1) {
                pwm_buffer[index] = PWM_BIT_1;
            } else {
                pwm_buffer[index] = PWM_BIT_0;
            }
            index++;
        }
    }

    // 3. Disparo seguro de hardware
    HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 0);

    // Enviamos el buffer completo (los 50 ceros iniciales limpian el canal)
    HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_2, (uint32_t*)pwm_buffer, START_BITS + TOTAL_BITS + RESET_BITS);
}
