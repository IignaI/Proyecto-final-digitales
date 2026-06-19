/*
 * teclado.c
 *
 *  Created on: Jun 12, 2026
 *      Author: iniak
 */
#include "stm32f4xx_hal.h"
#include "teclado.h"

extern volatile int X;
extern volatile int Y;
extern volatile char jugador;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // Verificas qué pin fue el que generó la interrupción
    int Xf = 1;
	switch (GPIO_Pin)
	case tcl_pin_x1:
	{
		    delay_bruto(100000);
		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x1);
		        if (Xf == 0)
		        {
		            X = 1;
		            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
		            rutina_leer_filas(tcl_pin_x1);
		        }

		    while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x1)==0)
		    {
		    	//bucle de retencion
		    }
		break;
	case tcl_pin_x2:
		    delay_bruto(100000);
		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x2);
		        if (Xf == 0)
		        {
		            X = 2;
		            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
		            rutina_leer_filas(tcl_pin_x2);
		        }

		    while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x2)==0)
		    {
		    	//bucle de retencion
		    }

		break;
	case tcl_pin_x3:
		    delay_bruto(100000);
		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x3);
		        if (Xf == 0)
		        {
		            X = 3;
				    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
				    rutina_leer_filas(tcl_pin_x3);
		        }

		    while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x3)==0)
		    {
		    	//bucle de retencion
		    }
		break;
	case tcl_pin_x4:
		    delay_bruto(100000);
		    Xf = HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x4);
		        if (Xf == 0)
		        {
		            X = 4;
				    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //prueba
				    rutina_leer_filas(tcl_pin_x4);
		        }

		    while (HAL_GPIO_ReadPin(tcl_puerto,tcl_pin_x4)==0)
		    {
		    	//bucle de retencion
		    }

		break;
	default:
		break;
	}
}
void rutina_leer_filas(uint16_t pin_x)
{
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
    if (Y != 0)
    {
    	//Si se llego a este bucle entonces se detecto una pulsacion X != 0 y una Y =! 0
    	//evento_actual = evento_presionar;
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

