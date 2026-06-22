/*
 * imp_matriz.c
 *
 *  Created on: Jun 21, 2026
 *      Author: marcos
 */
#include "stm32f4xx_hal.h"
#include "fsm.h"
#include "stdio.h"
#include "luces.h"

extern int matriz[8][4];
extern TIM_HandleTypeDef htim4;
void imprimir_matriz_actual(void)
{
	printf("Matriz actual:\n");
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			// Imprime el valor con espacio. Usa `%4d` para alinear columnas
			printf("%4d", matriz[i][j]);
		}
		// Salto de línea al terminar cada fila
		printf("\n");
	}

	return 0;
	escribir(matriz,&htim4);
}
