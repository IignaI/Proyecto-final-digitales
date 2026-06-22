/*
 * fsm.c
 *
 *  Created on: Jun 20, 2026
 *      Author: marcos
 */
#include "stm32f4xx_hal.h"
#include "fsm.h"
#include "stdio.h"
#include "imp_matriz.h"

typedef enum{
    estado_inicio,
    estado_inicializar,   //leer SW por primera vez
    estado_turno_A,
    estado_turno_B,
    estado_esperar_soltar,
    estado_asignar_jugada,
    estado_comprobar_jugada,
    estado_gana_A,
    estado_gana_B,
    estado_empate,
    estado_fin_del_juego,
	estado_asignar_turno,
	estado_menu
}estado;

typedef enum{
    evento_presionar,   //evento_1 ~ boton presionado
    evento_jugada_invalida_de_A,
    evento_jugada_invalida_de_B,
    evento_jugada_valida,
    evento_sin_presionar,
    evento_no_gana_A,
    evento_no_gana_B,
	evento_gana_A,
	evento_gana_B,
    evento_soltar,
	evento_fin_del_juego,
	evento_nada
}evento;

extern estado estado_siguiente;
extern estado evento_actual;
extern char jugador;
extern int X;
extern int Y;

volatile int find;
extern volatile int Xf;
extern volatile int Yf;
volatile char jugador_f;
volatile int dx;
volatile int dy;

extern volatile int matriz[8][4];

void actualizar_fsm_juego(void)
{
    switch (estado_siguiente)
    {
        case estado_inicio:
            {
                printf("Estado: Inicio de partida \n");
                printf("Comentario: Modo: comun \n");
                if (evento_actual == evento_presionar)
                {
                	estado_siguiente = estado_inicializar;
                	//imprimir_matriz_actual();
                }
                printf("Bienvenido al menu principal de tres en raya lpqtp! \n");
                printf("Apreta cualquier boton para empezar nabaso \n");

            }
        break;
        case estado_inicializar:
            {
                printf("Estado: Inicializando matriz \n");
                printf("Comentario: Vaciando Matriz \n");
                //inicializar_matriz();
                estado_siguiente = estado_turno_A;
                imprimir_matriz_actual();
            }
        break;
        case estado_turno_A:
                printf("Estado: Turno jugador A \n");
                printf("Comentario: Esperando que se presione un boton \n");
                jugador = 'A';
                if (evento_actual == evento_presionar)
                {
                    estado_siguiente = estado_asignar_jugada;
                }
                imprimir_matriz_actual();
        break;
        case estado_esperar_soltar:

                printf("Estado: Esperando soltar \n");
                //if (evento_actual == evento_soltar)
                //{
                    estado_siguiente = estado_asignar_jugada;
                //}
                    imprimir_matriz_actual();

        break;
        case estado_turno_B:

			printf("Estado: Turno jugador B \n");
			printf("Comentario: Esperando que se presione un boton \n");
			jugador = 'B';
			if (evento_actual == evento_presionar)
			{
				estado_siguiente = estado_asignar_jugada;
			}
			imprimir_matriz_actual();
            break;
        case estado_asignar_jugada:
                printf("Estado: Asignar jugada \n");
                asignar_jugada();
                if (evento_actual == evento_jugada_valida && jugador == 'A')
                {
                	estado_siguiente = estado_turno_B;
                }
                else if (evento_actual == evento_jugada_valida && jugador == 'B')
                {
                	estado_siguiente = estado_turno_A;
                }

        break;
        case estado_asignar_turno:
        {
        	if (evento_actual == evento_jugada_invalida_de_A)
			{
				estado_siguiente = estado_turno_A;
				X = 0;  //limpiar/reiniciar X
				Y = 0;  //limpiar/reiniciar Y
			}
			else if (evento_actual == evento_jugada_invalida_de_B)
			{
				estado_siguiente = estado_turno_B;
				X = 0;  //limpiar/reiniciar X
				Y = 0;  //limpiar/reiniciar Y
			}
			else if (evento_actual == evento_jugada_valida)
			{
				estado_siguiente = estado_comprobar_jugada;
				X = 0;  //limpiar/reiniciar X
				Y = 0;  //limpiar/reiniciar Y
				evento_actual = evento_soltar;
				printf("Evento: evento_soltar, coordenada ");
				printf("(X,Y)=(");
				printf("%d", X);
				printf(",");
				printf("%d", Y);
				printf(")\n");
			}
        }
        	break;
        case estado_comprobar_jugada:
            {
                //printf("Estado: Comprobar Victoria/Empate \n");
                //comprobar();

                //if (evento_actual == evento_no_gana_B)
                //{
                //    printf("Comentario: El juego continua \n");
                //    estado_siguiente = estado_turno_A;
                //}
                //else if (evento_actual == evento_no_gana_A)
                //{
                //    printf("Comentario: El juego continua \n");
                //    estado_siguiente = estado_turno_B;
                //}
                //else if (evento_actual == evento_gana_A)
                //{
                //    printf("Comentario: ¡Ganó el jugador A! \n");
                //    estado_siguiente = estado_gana_A;
                //}
                //else if (evento_actual == 'evento_gana_B')
                //{
                //    printf("Comentario: ¡Ganó el jugador B! \n");
                //    estado_siguiente = estado_gana_B;
                //}
                //else
                //{
                //    printf("Comentario: ¡Increible! pero empataron \n");
                //    estado_siguiente = estado_menu; //o fin_del_juego() mejor...
               //}
            }
        break;
        case estado_gana_A:
            {
                printf("Animacion de victoria jugador A \n");
                //animación_gana_A();
            }
        break;
        case estado_gana_B:
            {
            	printf("Animacion de victoria jugador B \n");
                //animación_gana_A();
            }
        break;
        case estado_fin_del_juego:
            {
            	printf("¡Fin del juego! \n");
                evento_actual = evento_fin_del_juego;
            }
        break;
        default:
            {
            	printf("Estado: default \n");
            }
        break;
    }
}


void asignar_jugada(void)
{
	int Y_ = Y-1;
	int X_ = X-1;

	int j = 8-1;
	if (jugador == 'A' && matriz[j][X_] == 0)
	{
		evento_actual = evento_jugada_valida;

		matriz[j][X_] = 1;
		imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion
		HAL_Delay(500);

		while (matriz[j-1][X_] == 0)
		{
			matriz[j][X_] = 0;
			j = j-1;
			matriz[j][X_] = 1;
			imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion
			HAL_Delay(500);
		}
		Xf = X_;
		Yf = j;
		printf("(Xf,Yf)=(");
		printf("%d",Xf+1);
		printf(",");
		printf("%d",Yf+1);
		printf(")\n");
		jugador_f = 'A';
		HAL_Delay(1000);	//para ver el mensaje

	}
	else if (jugador == 'B' && matriz[j][X_] == 0)
	{
		evento_actual = evento_jugada_valida;

		matriz[j][X_] = 2;
		imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion
		HAL_Delay(500);

		while (matriz[j-1][X_] == 0)
		{
			matriz[j][X_] = 0;
			j = j-1;
			matriz[j][X_] = 2;
			imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion
			HAL_Delay(500);
		}
		Xf = X_;
		Yf = j;
		printf("(Xf,Yf)=(");
		printf("%d",Xf+1);
		printf(",");
		printf("%d",Yf+1);
		printf(")\n");
		jugador_f = 'B';
		HAL_Delay(1000);	//para ver el mensaje

	}
	else if (jugador == 'B' && matriz[j][X_] != 0)
	{
		printf("Error: Jugada invalida de B \n");
		evento_actual = evento_jugada_invalida_de_B;
	}
	else if (jugador == 'A' && matriz[j][X_] != 0)
	{
		printf("Error: Jugada invalida de A \n");
		evento_actual = evento_jugada_invalida_de_A;
	}
	else
	{
		printf("Error: No hay jugador asignado \n");
		estado_siguiente = estado_inicio;
	}

}
void comprobar(void)
{
		int dy[] = {1,1,1,0,-1,-1,-1,0};
		int dx[] = {-1,0,1,1,1,0,-1,-1};
		int index;
		int jugador_n;
		int jugada_ganadora;
		int jugada_no_ganadora;
		Xf = Xf-1;
		Yf = Yf-1;
		int X;
		int Y;
		index = 0;

		if (jugador == 'A')		//esta seccion covierte jugador(char) a jugador_n(int)
		{
			jugador_n = 1;
		}
		else if (jugador == 'B')
		{
			jugador_n = 2;
		}

		while (jugada_ganadora == 1 || jugada_no_ganadora == 1)
		{
			if (matriz[Yf+dy[index]][Yf+dx[index]]==jugador_n && Xf<=3 && Xf>=0 && Yf<=7 && Yf>=0)
			{

				if (matriz[Yf+2*dy[index]][Yf+2*dx[index]]==jugador_n && Xf<=3 && Xf>=0 && Yf<=7 && Yf>=0)
				{
					jugada_ganadora = 1;
					if (jugador == 'A')
					{
						evento_actual = evento_gana_A;
					}
					else if (jugador == 'B')
					{
						evento_actual = evento_gana_B;
					}
					else
					{
						//no gana nadie, esto es un error...
					}
				}
				else
				{
					//jugada no ganadora
				}
			}
			else
			{
				//jugada no ganadora...
			}
			index = index+1;
			if (index == 8)
			{
				if (jugador == 'A')
				{
					evento_actual = evento_no_gana_A;
				}
				else if (jugador == 'B')
				{
					evento_actual = evento_no_gana_B;
				}
				else
				{
					//error...
				}
			}
		}
}

void animacion_caida_bomba(void)
{
	for (int j = 0; j <= 7; j++)
			{
			int fila_inferior[] = {matriz[j][0],matriz[j][1],matriz[j][2],matriz[j][3]};
			int fila_superior[] = {matriz[j+1][0],matriz[j+1][1],matriz[j+1][2],matriz[j+1][3]};

			for (int i = 0; i<=3; i++)
					{
						if (fila_superior[i]!=0 && fila_inferior[i]==0)
						{
							matriz[j+1][i]=0;
							matriz[j][i]=fila_superior[i];
						}
					}
			imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion
			HAL_Delay(500);
			}

}
void ficha_bomba(void)
{

}

void comentar(char Estado[],char Comentario[])
{
		printf(Estado);
		printf(Comentario);
}

