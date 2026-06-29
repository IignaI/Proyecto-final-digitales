/*
 * teclado.c
 *
 *  Created on: Jun 20, 2026
 *      Author: marcos
 */

#include "stm32f4xx_hal.h"
#include "teclado.h"
#include "stdio.h"

#define tcl_puerto GPIOE
#define tcl_pin_x4 GPIO_PIN_15
#define tcl_pin_x3 GPIO_PIN_13
#define tcl_pin_x2 GPIO_PIN_14//
#define tcl_pin_x1 GPIO_PIN_12
#define tcl_pin_y4 GPIO_PIN_11
#define tcl_pin_y3 GPIO_PIN_10
#define tcl_pin_y2 GPIO_PIN_9
#define tcl_pin_y1 GPIO_PIN_8

typedef enum{
    evento_presionar,
	evento_soltar
}evento;

extern volatile int X;
extern volatile int Y;
extern volatile char jugador;
extern volatile int bloqueo;
extern volatile uint32_t T;

extern volatile evento evento_actual;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // Verificas qué pin fue el que generó la interrupción
	if (HAL_GetTick() > T+500 && bloqueo == 0)
		{
    int Xf;
	switch (GPIO_Pin)
	case tcl_pin_x1:
	{

		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x1);	//Se lee nuevamente el valor de entrada
		    delay_bruto(100000);

		    	//Si despues del delay_bruto sigue siendo cero (ACTIVO BAJO) entonces era intencionada
		    	//La presion del switcht
		        if (Xf == 0 && Xf == HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x1))
		        {
		            X = 1;
		            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
		            rutina_leer_filas(tcl_pin_x1);
		            while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x1)==0)
					{
						//Mientras el boton no se suelte se queda en la interrupcion
					}
		            T=HAL_GetTick();
		            //Esta variable sirve para evitar que la rutina se vuelva a ejecutar
		            //producto de los rebotes al soltar. Esta variable sirve
		            //para pausar la interrupcion por unos milisegundos, porque es condicion
		            //de la interrupcion para que se ejecute que lo haga por lo menos
		            //a HAL_GetTick() >= T+500 despues...
		        }

		        //Los demas casos son analogos pera para las demas columnas
		break;
	case tcl_pin_x2:

		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x2);
		    delay_bruto(100000);
		    if (Xf == 0 && Xf == HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x2))
		        {
		            X = 2;
		            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
		            rutina_leer_filas(tcl_pin_x2);
		            while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x2)==0)
					{
						//printf("nada\n");
					}
		            T=HAL_GetTick();
		        }

		break;
	case tcl_pin_x3:

		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x3);
		    delay_bruto(100000);
		    if (Xf == 0 && Xf == HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x3))
		        {
		            X = 3;
				    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
				    rutina_leer_filas(tcl_pin_x3);
				    while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x3)==0)
					{
						//printf("nada\n");
					}
				    T=HAL_GetTick();
		        }
		        else

		break;
	case tcl_pin_x4:

		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x4);
		    delay_bruto(100000);
		    if (Xf == 0 && Xf == HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x4))
		        {
		            X = 4;
				    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
				    rutina_leer_filas(tcl_pin_x4);
				    while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x4)==0)
				    {
				    	//printf("nada\n");
				    }
				    T=HAL_GetTick();
		        }


		break;
	default:
		break;

		printf("Rebote o Procesamiento en curso \n");
	}
	if (Y != 0 && X != 0)
	    {
	    	printf("Evento: evento_presionar, coordenada ");
	    	printf("(X,Y)=(");
	    	printf("%d", X);
	    	printf(",");
	    	printf("%d", Y);
	    	printf(")\n");
	    	evento_actual = evento_presionar;
	    	printf("Evento: evento_presionar \n");
	    }
		}
}

void rutina_leer_filas(uint16_t pin_x)
{
	//Este algoritmo es el barrido que se genera una vez detectada la columna (X)
	//que interrumpió...
	//Simplemente barre haciendo una fila en cero y las demas en alta impedancia
	//hasta que se lea un cero en la columna y asi determinar (X,Y) que se presionó
	Y = 0;
	HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y1,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y3,GPIO_PIN_SET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y4,GPIO_PIN_SET);
    delay_bruto(1000);
    if (HAL_GPIO_ReadPin(tcl_puerto,pin_x) == 0)
    {
        Y = 1;
    }

    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y2,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y3,GPIO_PIN_SET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y4,GPIO_PIN_SET);
    delay_bruto(1000);
    if (HAL_GPIO_ReadPin(tcl_puerto,pin_x) == 0)
    {
    	Y = 2;
    }

    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y3,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y4,GPIO_PIN_SET);
    delay_bruto(1000);
    if (HAL_GPIO_ReadPin(tcl_puerto,pin_x) == 0)
    {
    	Y = 3;
    }

    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y4,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y3,GPIO_PIN_SET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y1,GPIO_PIN_SET);
    delay_bruto(1000);
    if (HAL_GPIO_ReadPin(tcl_puerto,pin_x) == 0)
    {
    	Y = 4;
    }
    reiniciar_filas_tcl();
}

void reiniciar_filas_tcl()
{
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y1,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y2,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y3,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(tcl_puerto,tcl_pin_y4,GPIO_PIN_RESET);
}
void delay_bruto(volatile uint32_t ciclos)
{
    while (ciclos > 0)
    {
        ciclos--;
    }
}
void PruebaConLed()
{
	if (X == 1)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		X = 0;
	}
	else if (X == 2)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		X = 0;
	}
	else if (X==3)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		X = 0;
	}
	else if (X == 4)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		X = 0;
	}
	else
	{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
	}
	if (Y == 1)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		Y = 0;
	}
	else if (Y == 2)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		Y = 0;
	}
	else if (Y==3)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(400);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		Y = 0;
	}
	else if (Y == 4)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		Y = 0;
		}
	else
	{
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
	}
}
