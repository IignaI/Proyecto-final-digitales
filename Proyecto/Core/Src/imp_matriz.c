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

extern TIM_HandleTypeDef htim4;

extern int matriz[8][4];
uint32_t mi_matriz[8][4];

void imprimir_matriz_actual(void)
{
	//printf("Matriz actual:\n");
	//for (int i = 7; i >= 0; i--)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		// Imprime el valor con espacio. Usa `%4d` para alinear columnas
	//		printf("%4d", matriz[i][j]);
	//	}
	//	// Salto de línea al terminar cada fila
	//	printf("\n");
	//}

	//conversion matriz int a uint32_t
	for (int j = 0; j <= 7; j++)
		{
			for (int i = 0; i <= 3; i++)
			{
					//para invertir el display j=7-1
				if (matriz[j][i]==1)
				{
					mi_matriz[7-j][i]=0x000002;
				}
				else if (matriz[j][i]==2)
				{
					mi_matriz[7-j][i]=0x020000;
				}
				else if (matriz[j][i]==5)
				{
					mi_matriz[7-j][i]=0x000200;
				}
				else if (matriz[j][i]==9)
				{
					mi_matriz[7-j][i]=0x020202;
				}
				else
				{
					mi_matriz[7-j][i]=0x000000;
				}
			}
		}

	escribir(mi_matriz,&htim4);

}
