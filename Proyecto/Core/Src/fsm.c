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
	estado_menu,
	estado_bomba
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
extern int bloqueo;

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
    	borrar();
    	if (evento_actual == evento_presionar)
    	{

    		turno_anim(1);
    		evento_actual = evento_soltar;
    		estado_siguiente = estado_turno_A;
    	}
    	break;
	case estado_turno_A:
		if (evento_actual == evento_presionar)
		{
			printf("A\n");
			jugador = 'A';
			printf("jugador A\n");
			asignar_jugada();
			evento_actual = evento_soltar;
			estado_siguiente = estado_comprobar_jugada;

		}
		break;
	case estado_turno_B:
		if (evento_actual == evento_presionar)
		{
			printf("B\n");
			jugador = 'B';
			asignar_jugada();
			evento_actual = evento_soltar;
			estado_siguiente = estado_comprobar_jugada;
		}
		break;
	case estado_bomba:
		if (evento_actual == evento_presionar)
		{
			jugador = 'K';
			asignar_jugada();
			bomba();

			//sin este if-else ocurre que el turno del que tiro la bomba es el mismo que despues de tirarla

			evento_actual = evento_soltar;
			estado_siguiente = estado_comprobar_jugada;
		}
		break;
	case estado_comprobar_jugada:
		comprobar();

		//decision de turnos
		uint8_t last_digit = (uint8_t)(HAL_GetTick() % 10);	//obtiene el ultimo digito de HAL_GetTick()...

		if (evento_actual == evento_gana_A)
		{
			gana(1);
			estado_siguiente = estado_inicio;
		}
		else if(evento_actual == evento_gana_B)
		{
			gana(2);
			estado_siguiente = estado_inicio;
		}
		else if (last_digit == 1 | last_digit == 3)
		{
			turno_anim(5);
			if (jugador_f == 'A')
			{
				printf("bomba para B \n");
				estado_siguiente = estado_bomba;
			}
			else if (jugador_f == 'B')
			{
				printf("bomba para A \n");
				estado_siguiente = estado_bomba;
			}
		}
		else
		{
			if (jugador_f == 'A')
			{
				turno_anim(2);
				jugador = 'B';
				estado_siguiente = estado_turno_B;
			}
			else if (jugador_f == 'B')
			{
				turno_anim(1);
				jugador = 'A';
				estado_siguiente = estado_turno_A;
			}
			break;
		}
    }
}



void asignar_jugada(void)
{
	bloqueo = 1;
	int Y_ = Y-1;
	int X_ = X-1;

	int j = 8-1;
	if (matriz[j][X_] == 0)
	{
		evento_actual = evento_jugada_valida;
		if (jugador == 'A')
		{
			matriz[j][X_] = 1;
		}
		else if (jugador == 'B')
		{
			matriz[j][X_] = 2;
		}
		else if (jugador == 'K')
		{
			matriz[j][X_] = 5;
		}
		imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion


		int k;	//gravedad
		k=0;	//gravedad
		while (matriz[j-1][X_] == 0 && j-1>=0)
		{
			matriz[j][X_] = 0;
			j = j-1;
			if (jugador == 'A')
			{
				matriz[j][X_] = 1;
				jugador_f = 'A';
			}
			else if (jugador == 'B')
			{
				matriz[j][X_] = 2;
				jugador_f = 'B';
			}
			else if (jugador == 'K')
			{
				matriz[j][X_] = 5;
				// si esto fuese asi:jugador_f = 'K', no funcionaria la jugada bomba con turnos correctamente;
			}
			imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion
			HAL_Delay(106/2-k*6);	//gravedad
			k=k-1;	//gravedad
		}
		Xf = X_;
		Yf = j;
		printf("(Xf,Yf)=(");
		printf("%d",Xf+1);
		printf(",");
		printf("%d",Yf+1);
		printf(")\n");
		HAL_Delay(500);	//para ver el mensaje

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

	bloqueo = 0;
	X=0;
	Y=0;

}
void comprobar(void)
{
		int jugador_n;
		int dy1[8] = {0,0,1,-1,1,-1,1,-1};
		int dy2[8] = {0,0,2,-2,2,-2,2,-2};
		int dy3[8] = {0,0,-1,1,-1,1,-1,1};
		int dx1[8] = {1,-1,0,0,1,-1,-1,1};
		int dx2[8] = {2,-2,0,0,2,-2,-2,2};
		int dx3[8] = {-1,1,0,0,-1,1,1,-1};

		int ganar;
		int i;

		ganar = 0;

		i = 0;

		if (jugador == 'A')		//esta seccion covierte jugador(char) a jugador_n(int)
		{
			jugador_n = 1;
		}
		else if (jugador == 'B')
		{
			jugador_n = 2;
		}

		while (ganar == 0 && i<=7)
		{
			if (matriz[Yf+dy1[i]][Xf+dx1[i]]==jugador_n && Yf+dy1[i]<=7 && Yf+dy1[i]>=0 && Xf+dx1[i]<=3 && Yf+dx1[i]>=0)
			{
				if (matriz[Yf+dy2[i]][Xf+dx2[i]]==jugador_n && Yf+dy2[i]<=7 && Yf+dy2[i]>=0 && Xf+dx2[i]<=3 && Yf+dx2[i]>=0)
				{
					if (jugador_n==1)
					{
						printf("Win A\n");
						evento_actual = evento_gana_A;
					}
					else
					{
						printf("Win B\n");
						evento_actual = evento_gana_B;
					}
				}
				else if (matriz[Yf+dy3[i]][Xf+dx3[i]]==jugador_n && Yf+dy3[i]<=7 && Yf+dy3[i]>=0 && Xf+dx3[i]<=3 && Yf+dx3[i]>=0)
				{

					if (jugador_n==1)
					{
						printf("Win A\n");
						evento_actual = evento_gana_A;
					}
					else
					{
						printf("Win B\n");
						evento_actual = evento_gana_B;
					}
				}
			}
			i=i+1;
		}

		//seccion jugada bomba

}


void animacion_caida_bomba(void)
{
	for (int i=0; i<=7; i++)
	{
		for (int j=0; j<=7; j++)
		{
			if (matriz[j][i]==9)
			{
				matriz[j][i]=0;
			}
		}
	}
	for (int k=0; k<=7; k++)
	{
	//este for doble borrar los nueves dejados por la bomba que servian para iluminar el destrozo...


	for (int j=0; j<=6; j++)
	{

		for (int i=0; i<=3; i++)
		{
			if (matriz[j][i]==0 && matriz[j+1][i]!=0 )
			{
				matriz[j][i] = matriz[j+1][i];
				matriz[j+1][i] = 0;
			}
		}
		imprimir_matriz_actual();	//en este caso se imprime para mostrar la animacion

	}
	HAL_Delay(106-k*15);	//simula el efecto de gravedad si la dv/dt=-cte
	}
	printf("fin de animacion\n");

}

void bomba(void)
{
	if (jugador == 'K')
	{
		matriz[Yf][Xf]=9;
		if (Yf<=7 && Yf>=0 && Xf+1<=3 && Xf+1>=0)
		{
			matriz[Yf][Xf+1]=9;
		}
		if (Yf-1<=7 && Yf-1>=0 && Xf<=3 && Xf>=0)
		{
			matriz[Yf-1][Xf]=9;
		}
		if (Yf<=7 && Yf>=0 && Xf-1<=3 && Xf-1>=0)
		{
			matriz[Yf][Xf-1]=9;
		}
		if (Yf+1<=7 && Yf+1>=0 && Xf<=3 && Xf>=0)
		{
			matriz[Yf+1][Xf]=9;
		}
		printf("PLOFWM!!!!\n");
		imprimir_matriz_actual();
		HAL_Delay(100);
		matriz[Yf][Xf]=9;
		if (Yf<=7 && Yf>=0 && Xf+1<=3 && Xf+1>=0)
		{
			matriz[Yf][Xf+1]=0;
		}
		if (Yf-1<=7 && Yf-1>=0 && Xf<=3 && Xf>=0)
		{
			matriz[Yf-1][Xf]=0;
		}
		if (Yf<=7 && Yf>=0 && Xf-1<=3 && Xf-1>=0)
		{
			matriz[Yf][Xf-1]=0;
		}
		if (Yf+1<=7 && Yf+1>=0 && Xf<=3 && Xf>=0)
		{
			matriz[Yf+1][Xf]=0;
		}
		printf("PLOFWM!!!!\n");
		imprimir_matriz_actual();
		HAL_Delay(100);
		matriz[Yf][Xf]=9;
		if (Yf<=7 && Yf>=0 && Xf+1<=3 && Xf+1>=0)
		{
			matriz[Yf][Xf+1]=9;
		}
		if (Yf-1<=7 && Yf-1>=0 && Xf<=3 && Xf>=0)
		{
			matriz[Yf-1][Xf]=9;
		}
		if (Yf<=7 && Yf>=0 && Xf-1<=3 && Xf-1>=0)
		{
			matriz[Yf][Xf-1]=9;
		}
		if (Yf+1<=7 && Yf+1>=0 && Xf<=3 && Xf>=0)
		{
			matriz[Yf+1][Xf]=9;
		}
		printf("PLOFWM!!!!\n");
		imprimir_matriz_actual();
		HAL_Delay(100);
		animacion_caida_bomba();
	}
}

void gana(int jugador_n)
{
	for (int j=0; j<=7; j++)
	{

		for (int i=0; i<=3; i++)
		{
				matriz[j][i] = jugador_n;
		}

	}
	imprimir_matriz_actual();

}

void borrar()
{
	for (int j=0; j<=7; j++)
	{

		for (int i=0; i<=3; i++)
		{
				matriz[j][i] = 0;
		}

	}
	imprimir_matriz_actual();

}

void turno_anim(int jugador_n)
{
	int matriz_aux[8][4];
	for (int j=0; j<=7; j++)
	{
		for (int i=0; i<=3; i++)
		{
			matriz_aux[j][i]=matriz[j][i];	//guardo la matriz original
		}
	}
	HAL_Delay(200);
	for (int j=0; j<=7; j++)
		{
			for (int i=0; i<=3; i++)
			{
				if (jugador_n == 5)
				{
					matriz[j][i]=5;	//pinto la matriz de verde
				}
				else if (jugador_n == 1)
				{
					matriz[j][i]=1;	//pinto la matriz de verde
				}
				else if (jugador_n == 2)
				{
					matriz[j][i]=2;	//pinto la matriz de verde
				}
			}
		}
	imprimir_matriz_actual();
	HAL_Delay(200);
	borrar();

	for (int j=0; j<=7; j++)
		{
			for (int i=0; i<=3; i++)
			{
				matriz[j][i]=matriz_aux[j][i];
			}
		}
	imprimir_matriz_actual();


}



