/*
 * fsm.c
 *
 *  Created on: Jun 20, 2026
 *      Author: marcos
 */
#include "stm32f4xx_hal.h"
#include "fsm.h"
#include "stdio.h"
#include "luces.h"

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
	estado_bomba,
	estado_prepartida
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
extern evento evento_actual;
extern char jugador;
extern int X;
extern int Y;
extern int bloqueo;
extern TIM_HandleTypeDef htim4;

volatile int x_bot;
volatile int dificultad = 2;

volatile int find;
extern volatile int Xf;
extern volatile int Yf;
volatile char jugador_f;
volatile int dx;
volatile int dy;
volatile int win_x1, win_x2, win_x3;
volatile int win_y1, win_y2, win_y3;
volatile int modobomba=1;
volatile int modobot=0;

extern int matriz[8][4];
extern int ocupado;
int matrizj[8][4] = {
     			{0, 1, 1, 0},
     			{0, 0, 1, 0},
     			{0, 0, 1, 0},
     			{0, 1, 1, 1},
     			{0, 2, 2, 0},
     			{0, 0, 2, 0},
     			{0, 0, 2, 0},
     			{0, 2, 2, 2}
     		};
int matrizb[8][4] = {
     			{0, 1, 1, 0},
     			{5, 0, 1, 0},
     			{0, 0, 1, 0},
     			{0, 1, 1, 1},
     			{0, 2, 2, 0},
     			{5, 0, 2, 0},
     			{0, 0, 2, 0},
     			{0, 2, 2, 2}
     		};
int matrizmb[8][4] = {
     			{0, 1, 1, 0},
     			{0, 0, 1, 0},
     			{0, 0, 1, 0},
     			{0, 1, 1, 1},
     			{0, 2, 0, 2},
     			{0, 2, 0, 2},
     			{0, 2, 2, 2},
     			{0, 2, 0, 2}
     		};

int existe(int x,int y)  //funcion auxiliar que determina si la posicion existe...
{
    if ( x<=3 && y<=7 && x>=0 && y>=0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void main_menu(void)
{
	static uint32_t tiempo_anterior_menu = 0;
	  if (HAL_GetTick() - tiempo_anterior_menu >= 300) {
	        tiempo_anterior_menu = HAL_GetTick();
	        if (modobomba == 1 && modobot==0){
	        	for (int j=0; j<=7; j++)
	        		{
	        			for (int i=0; i<=3; i++)
	        			{
	        					matriz[j][i] = matrizb[j][i] ;
	        			}

	        		}
	        }else if(modobot==0 && modobomba==0){
	for (int j=0; j<=7; j++)
	{
		for (int i=0; i<=3; i++)
		{
				matriz[j][i] = matrizj[j][i] ;
		}
	}
	}else{
		for (int j=0; j<=7; j++)
			{
				for (int i=0; i<=3; i++)
				{
						matriz[j][i] = matrizmb[j][i] ;
				}

	}

	  }
	        escribir(matriz,&htim4);
}}

void jugada_de_bot_no_dejar_ganar(void)
{
	for (int y=0; y<=7; y++)
	{
		for (int x=0; x<=3; x++)
		{

			//ACA LAS JUGADAS GANADORAS
						if (matriz[y][x]==2 && matriz[y][x+1]==2 && matriz[y][x+2]==0 && (matriz[y-1][x+2]!=0 || existe(x+2,y-1)==0) && (matriz[y+1][x+2]==0 || existe(x+2,y+1)==0))
						{
							x_bot=x+2;
						}
						else if (matriz[y][x]==2 && matriz[y][x-1]==2 && matriz[y][x-2]==0 && (matriz[y-1][x-2]!=0 || existe(x-2,y-1)==0) && (matriz[y+1][x-2]==0 || existe(x-2,y+1)==0))
						{
							x_bot=x-2;
						}

						else if  (matriz[y][x]==2 && matriz[y+1][x]==2 && matriz[y+2][x]==0 && existe(x,y+2)==1)
						{
							x_bot=x;
						}

						else if  (matriz[y][x]==2 && matriz[y+1][x+1]==2 && matriz[y+2][x+2]==0 && matriz[y+1][x+2]!=0 && (matriz[y+3][x+2]==0 || existe(x+2,y+3)==0))
						{
							x_bot=x+2;
						}
						else if  (matriz[y][x]==2 && matriz[y+1][x-1]==2 && matriz[y+2][x-2]==0 && matriz[y+1][x-2]!=0 && (matriz[y+3][x-2]==0 || existe(x-2,y+3)==0))
						{
							x_bot=x-2;
						}
						else if  (matriz[y][x]==2 && matriz[y-1][x+1]==2 && matriz[y-2][x+2]==0 && matriz[y-1][x+2]==0 && (existe(x+2,y-3)==0 || matriz[y-3][x+2]!=0))
						{
							x_bot=x+2;
						}
						else if  (matriz[y][x]==2 && matriz[y-1][x-1]==2 && matriz[y-2][x-2]==0 && matriz[y-1][x-2]==0 && (existe(x-2,y-3)==0 || matriz[y-3][x-2]!=0))
						{
							x_bot=x-2;
						}

						//Aca las jugadas semi-ganadoras
						else if (matriz[y][x]==2 && matriz[y+1][x]==0 && existe(x,y+1)==1)
						{
							x_bot=x;
						}
						else if (matriz[y][x]==2 && matriz[y][x+1]==0 && matriz[y+1][x+1]==0 && existe(x+1,y)==1 && (matriz[y-1][x+1]!=0 || existe(x+1,y-1)==0))
						{
							x_bot=x+1;
						}
						else if (matriz[y][x]==2 && matriz[y][x-1]==0 && matriz[y+1][x-1]==0 && existe(x-1,y)==1 && (matriz[y-1][x-1]!=0 || existe(x-1,y-1)==0))
						{
							x_bot=x-1;
						}

						else if (matriz[y][x]==2 && matriz[y-1][x-1]==0 && matriz[y][x-1]==0 && (matriz[y-2][x-1]!=0 || existe(x-1,y-2)==0))
						{
							x_bot=x-1;
						}
						else if (matriz[y][x]==2 && matriz[y-1][x+1]==0 && matriz[y][x+1]==0 && (matriz[y-2][x+1]!=0 || existe(x+1,y-2)==0))
						{
							x_bot=x+1;
						}

						else if (matriz[y][x]==2 && matriz[y+1][x+1]==0 && matriz[y][x+1]!=0 && existe(x+1,y+1)==0)
						{
							x_bot=x+1;
						}
						else if (matriz[y][x]==2 && matriz[y+1][x-1]==0 && matriz[y][x-1]!=0 && existe(x-1,y+1)==0)
						{
							x_bot=x-1;
						}
			//Si no puede ganar entonces intenta bloquear...
				else if (matriz[y][x]==1 && matriz[y][x+1]==1 && matriz[y][x+2]==0 && (matriz[y-1][x+2]!=0 || existe(x+2,y-1)==0) && (matriz[y+1][x+2]==0 || existe(x+2,y+1)==0))
				{
					x_bot=x+2;
				}
				else if (matriz[y][x]==1 && matriz[y][x-1]==1 && matriz[y][x-2]==0 && (matriz[y-1][x-2]!=0 || existe(x-2,y-1)==0) && (matriz[y+1][x-2]==0 || existe(x-2,y+1)==0))
				{
					x_bot=x-2;
				}

				else if  (matriz[y][x]==1 && matriz[y+1][x]==1 && matriz[y+2][x]==0 && existe(x,y+2)==1)
				{
					x_bot=x;
				}

				else if  (matriz[y][x]==1 && matriz[y+1][x+1]==1 && matriz[y+2][x+2]==0 && matriz[y+1][x+2]!=0 && (matriz[y+3][x+2]==0 || existe(x+2,y+3)==0))
				{
					x_bot=x+2;
				}
				else if  (matriz[y][x]==1 && matriz[y+1][x-1]==1 && matriz[y+2][x-2]==0 && matriz[y+1][x-2]!=0 && (matriz[y+3][x-2]==0 || existe(x-2,y+3)==0))
				{
					x_bot=x-2;
				}


				else if  (matriz[y][x]==1 && matriz[y-1][x+1]==1 && matriz[y-2][x+2]==0 && matriz[y-1][x+2]==0 && (existe(x+2,y-3)==0 || matriz[y-3][x+2]!=0))
				{
					x_bot=x+2;
				}
				else if  (matriz[y][x]==1 && matriz[y-1][x-1]==1 && matriz[y-2][x-2]==0 && matriz[y-1][x-2]==0 && (existe(x-2,y-3)==0 || matriz[y-3][x-2]!=0))
				{
					x_bot=x-2;
				}
				else
				{
					jugada_de_bot_aleatoria();
				}
		}
	}
	X=x_bot+1;
	evento_actual = evento_presionar;
}

void jugada_de_bot_ganar(void)
{
	for (int y=0; y<=7; y++)
	{
		for (int x=0; x<=3; x++)
		{
			//ACA LAS JUGADAS GANADORAS EN CASO QUE EXISTAN DOS fichas posibles ganadoras
			if (matriz[y][x]==2 && matriz[y][x+1]==2 && matriz[y][x+2]==0 && (matriz[y-1][x+2]!=0 || existe(x+2,y-1)==0) && (matriz[y+1][x+2]==0 || existe(x+2,y+1)==0))
			{
				x_bot=x+2;
			}
			else if (matriz[y][x]==2 && matriz[y][x-1]==1 && matriz[y][x-2]==0 && (matriz[y-1][x-2]!=0 || existe(x-2,y-1)==0) && (matriz[y+1][x-2]==0 || existe(x-2,y+1)==0))
			{
				x_bot=x-2;
			}

			else if  (matriz[y][x]==2 && matriz[y+1][x]==2 && matriz[y+2][x]==0 && existe(x,y+2)==1)
			{
				x_bot=x;
			}

			else if  (matriz[y][x]==2 && matriz[y+1][x+1]==2 && matriz[y+2][x+2]==0 && matriz[y+1][x+2]!=0 && (matriz[y+3][x+2]==0 || existe(x+2,y+3)==0))
			{
				x_bot=x+2;
			}
			else if  (matriz[y][x]==2 && matriz[y+1][x-1]==2 && matriz[y+2][x-2]==0 && matriz[y+1][x-2]!=0 && (matriz[y+3][x-2]==0 || existe(x-2,y+3)==0))
			{
				x_bot=x-2;
			}
			else if  (matriz[y][x]==2 && matriz[y-1][x+1]==2 && matriz[y-2][x+2]==0 && matriz[y-1][x+2]==0 && (existe(x+2,y-3)==0 || matriz[y-3][x+2]!=0))
			{
				x_bot=x+2;
			}
			else if  (matriz[y][x]==2 && matriz[y-1][x-1]==2 && matriz[y-2][x-2]==0 && matriz[y-1][x-2]==0 && (existe(x-2,y-3)==0 || matriz[y-3][x-2]!=0))
			{
				x_bot=x-2;
			}

			//DE ACA EN ADELANTE PONE CERCA DE UNA YA EXISTENTE
			else if (matriz[y][x]==2 && matriz[y+1][x]==0 && existe(x,y+1)==1)
			{
				x_bot=x;
			}
			else if (matriz[y][x]==2 && matriz[y][x+1]==0 && matriz[y+1][x+1]==0 && existe(x+1,y)==1 && (matriz[y-1][x+1]!=0 || existe(x+1,y-1)==0))
			{
				x_bot=x+1;
			}
			else if (matriz[y][x]==2 && matriz[y][x-1]==0 && matriz[y+1][x-1]==0 && existe(x-1,y)==1 && (matriz[y-1][x-1]!=0 || existe(x-1,y-1)==0))
			{
				x_bot=x-1;
			}

			else if (matriz[y][x]==2 && matriz[y-1][x-1]==0 && matriz[y][x-1]==0 && (matriz[y-2][x-1]!=0 || existe(x-1,y-2)==0))
			{
				x_bot=x-1;
			}
			else if (matriz[y][x]==2 && matriz[y-1][x+1]==0 && matriz[y][x+1]==0 && (matriz[y-2][x+1]!=0 || existe(x+1,y-2)==0))
			{
				x_bot=x+1;
			}

			else if (matriz[y][x]==2 && matriz[y+1][x+1]==0 && matriz[y][x+1]!=0 && existe(x+1,y+1)==0)
			{
				x_bot=x+1;
			}
			else if (matriz[y][x]==2 && matriz[y+1][x-1]==0 && matriz[y][x-1]!=0 && existe(x-1,y+1)==0)
			{
				x_bot=x-1;
			}

			//SINO PONE UNA EN CUALQUIER LADO
			else
			{
			jugada_de_bot_aleatoria();
			}
		}
	}
	X=x_bot+1;
	evento_actual = evento_presionar;
}

void jugada_de_bot_aleatoria(void)
{
	x_bot = HAL_GetTick() % 3;
	while (matriz[7][x_bot]!=0)	//se fija si alguna de las columnas no esta llena (no puede tirar sino)
	{
		x_bot = HAL_GetTick() % 3;
	}
	X=x_bot+1;
	evento_actual = evento_presionar;
}

void animacion_caida_bomba(void)
{
	static uint32_t tiempo_anterior_cb = 0;
	if (HAL_GetTick() - tiempo_anterior_cb >= 300) {
		        tiempo_anterior_cb = HAL_GetTick();
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
		escribir(matriz,&htim4);	//en este caso se imprime para mostrar la animacion

	}
	}
	}
}

void bomba(void)
{
	static uint32_t tiempo_anterior_b = 0;
	if (HAL_GetTick() - tiempo_anterior_b >= 300) {
		        tiempo_anterior_b = HAL_GetTick();
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
		escribir(matriz,&htim4);
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
		escribir(matriz,&htim4);
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
		escribir(matriz,&htim4);
		HAL_Delay(100);
		animacion_caida_bomba();
	}
}
}
void borrar(void)
{
	for (int j=0; j<=7; j++)
	{

		for (int i=0; i<=3; i++)
		{
				matriz[j][i] = 0;
		}

	}
	escribir(matriz,&htim4);

}

void actualizar_fsm_juego(void)
{
    switch (estado_siguiente)
    {
    default:
            // "Todas las demás opciones que no me importan entran aquí"
            break; // No hace nada y sale del switch de forma segura
    case estado_inicio:
    	main_menu();
    	if (evento_actual == evento_presionar && X==4 && Y==4)
    	{

    		modobomba=0;
    		modobot=0;
    		evento_actual = evento_soltar;
    		estado_siguiente = estado_inicio;
    	}
    	else if (evento_actual == evento_presionar && X==3 && Y==4)
		{

    		modobomba=0;
    					modobot=1;
    					dificultad=2;
			evento_actual = evento_soltar;
			estado_siguiente = estado_inicio;
		}
    	else if (evento_actual == evento_presionar && X==2 && Y==4)
		{

    		modobomba=1;
    		modobot=0;
			evento_actual = evento_soltar;
			estado_siguiente = estado_inicio;
		}
    	else if (evento_actual == evento_presionar && X==1 && Y==4)
		{


			evento_actual = evento_soltar;
			estado_siguiente = estado_prepartida;
		}


    	else
    	{
    		evento_actual = evento_soltar;
    	}
    	break;
    case estado_prepartida:
    	fin(matriz, &htim4 );

    	if (ocupado == 1)
    	{

    		estado_siguiente = estado_prepartida;
    	}else{    	estado_siguiente = estado_turno_A;
		turno_anim(1);}

    	break;
	case estado_turno_A:
		if (evento_actual == evento_presionar)
		{
			jugador = 'A';
			asignar_jugada();
			evento_actual = evento_soltar;
			estado_siguiente = estado_comprobar_jugada;




		}
		break;
	case estado_turno_B:
		if(modobot==0){
		if (evento_actual == evento_presionar)
		{
			jugador = 'B';
			asignar_jugada();
			evento_actual = evento_soltar;
			estado_siguiente = estado_comprobar_jugada;
		}
		}else{
			if (dificultad == 0)
			{
				jugada_de_bot_aleatoria();
			}
			else if (dificultad == 1)
			{
				jugada_de_bot_ganar();
			}
			else if (dificultad ==2)
			{
				jugada_de_bot_no_dejar_ganar();
			}

			//juega el bot, despues simula el presionar una boton
			if (evento_actual == evento_presionar)
			{
				jugador = 'B';
				asignar_jugada();
				evento_actual = evento_soltar;
				estado_siguiente = estado_comprobar_jugada;
			}
		}
		break;
	case estado_bomba:
		if (evento_actual == evento_presionar)
		{
			jugador = 'K';
			asignar_jugada();
			bomba();


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
			bloqueo=1;
			animacion_victoria(matriz, win_y1, win_x1, win_y2, win_x2, win_y3, win_x3, &htim4);
			bloqueo=0;
			fin(matriz, &htim4 );

			    	if (ocupado == 1)
			    	{

			    		estado_siguiente = estado_comprobar_jugada;
			    	}else{    	estado_siguiente = estado_inicio;}

		}
		else if(evento_actual == evento_gana_B)
		{
			bloqueo=1;
			animacion_victoria(matriz, win_y1, win_x1, win_y2, win_x2, win_y3, win_x3, &htim4);
			bloqueo=0;
			fin(matriz, &htim4 );

						    	if (ocupado == 1)
						    	{

						    		estado_siguiente = estado_comprobar_jugada;
						    	}else{    	estado_siguiente = estado_inicio;}
		}
		else if (last_digit == 1 || last_digit == 3)
		{

			if (modobomba==1){
				bloqueo=1;
				turno_anim(5);
				bloqueo=0;
				estado_siguiente = estado_bomba;
			}
		}
		else
		{
			if (jugador_f == 'A')
			{
				bloqueo=1;
				turno_anim(2);
				bloqueo=0;
				jugador = 'B';
				estado_siguiente = estado_turno_B;
			}
			else if (jugador_f == 'B')
			{
				bloqueo=1;
				turno_anim(1);
				bloqueo=0;
				jugador = 'A';
				estado_siguiente = estado_turno_A;
			}
			break;
		}
    }
}

void asignar_jugada(void)
{
	//bloqueo = 1;
	//int Y ;
	int X_ = X-1;

	int j = 8-1;
	if (matriz[j][X_] == 0 )
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
		escribir(matriz,&htim4);	//en este caso se imprime para mostrar la animacion


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
			escribir(matriz,&htim4);	//en este caso se imprime para mostrar la animacion
			HAL_Delay(106/2-k*6);	//gravedad
			k=k-1;	//gravedad
		}
		Xf = X_;
		Yf = j;

	}
	else if (jugador == 'B' && matriz[j][X_] != 0)
	{
		evento_actual = evento_jugada_invalida_de_B;
	}
	else if (jugador == 'A' && matriz[j][X_] != 0)
	{
		evento_actual = evento_jugada_invalida_de_A;
	}

	bloqueo = 0;
	X=0;
	Y=0;

}

void comprobar(void)
{
	int jugador_n = 0;
	int dy1[8] = {0,0,1,-1,1,-1,1,-1};
	int dy2[8] = {0,0,2,-2,2,-2,2,-2};
	int dy3[8] = {0,0,-1,1,-1,1,-1,1};
	int dx1[8] = {1,-1,0,0,1,-1,-1,1};
	int dx2[8] = {2,-2,0,0,2,-2,-2,2};
	int dx3[8] = {-1,1,0,0,-1,1,1,-1};

	int ganar = 0;
	int i = 0;

	// Convierte jugador (char) a jugador_n (int)
	if (jugador == 'A')
	{
		jugador_n = 1;
	}
	else if (jugador == 'B')
	{
		jugador_n = 2;
	}else{return;}
	while (ganar == 0 && i <= 7)
	{
		// 1. Calculamos las posiciones deseadas para este vector
		int y1 = Yf + dy1[i]; int x1 = Xf + dx1[i];
		int y2 = Yf + dy2[i]; int x2 = Xf + dx2[i];
		int y3 = Yf + dy3[i]; int x3 = Xf + dx3[i];

		// 2. Evaluamos la primera dirección (Ficha adyacente)
		// PRIMERO validamos que las coordenadas estén dentro de la matriz de 8x4
		if (y1 >= 0 && y1 <= 7 && x1 >= 0 && x1 <= 3)
		{
			if (matriz[y1][x1] == jugador_n)
			{
				// Caso A: Revisar si la línea se extiende en la misma dirección (Ficha 1 + Ficha 2)
				if (y2 >= 0 && y2 <= 7 && x2 >= 0 && x2 <= 3)
				{
					if (matriz[y2][x2] == jugador_n)
					{
						win_x1 = Xf; win_y1 = Yf;
						win_x2 = x1; win_y2 = y1;
						win_x3 = x2; win_y3 = y2;
						ganar = 1; // Detiene el bucle while

						if (jugador_n == 1) evento_actual = evento_gana_A;
						else                evento_actual = evento_gana_B;
					}
				}

				// Caso B: Revisar si la ficha actual quedó en el MEDIO (Ficha 1 + Ficha 3 trasera)
				if (ganar == 0 && y3 >= 0 && y3 <= 7 && x3 >= 0 && x3 <= 3)
				{
					if (matriz[y3][x3] == jugador_n)
					{
						win_x1 = Xf; win_y1 = Yf;
						win_x2 = x1; win_y2 = y1;
						win_x3 = x3; win_y3 = y3;
						ganar = 1; // Detiene el bucle while

						if (jugador_n == 1) evento_actual = evento_gana_A;
						else                evento_actual = evento_gana_B;
					}
				}
			}
		}
		i++;
	}


	// Aquí abajo puedes continuar con tu sección de la jugada bomba
}

void gana(int jugador_n)
{

	fin(matriz, &htim4 );

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
	escribir(matriz,&htim4);
	HAL_Delay(200);
	borrar();

	for (int j=0; j<=7; j++)
		{
			for (int i=0; i<=3; i++)
			{
				matriz[j][i]=matriz_aux[j][i];
			}
		}
	escribir(matriz,&htim4);


}

void actualizar_fsm_juego_vs_bot(void)
{
    switch (estado_siguiente)
    {
    default:
            // "Todas las demás opciones que no me importan entran aquí"
            break; // No hace nada y sale del switch de forma segura
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
			jugador = 'A';
			asignar_jugada();
			evento_actual = evento_soltar;
			estado_siguiente = estado_comprobar_jugada;

		}
		break;
	case estado_turno_B:
		if (dificultad == 0)
		{
			jugada_de_bot_aleatoria();
		}
		else if (dificultad == 1)
		{
			jugada_de_bot_ganar();
		}
		else if (dificultad ==2)
		{
			jugada_de_bot_no_dejar_ganar();
		}

		//juega el bot, despues simula el presionar una boton
		if (evento_actual == evento_presionar)
		{
			jugador = 'B';
			asignar_jugada();
			evento_actual = evento_soltar;
			estado_siguiente = estado_comprobar_jugada;
		}
		break;
	case estado_comprobar_jugada:
		comprobar();

		//decision de turnos
		if (evento_actual == evento_gana_A)
		{
			bloqueo=1;
			gana(1);
			bloqueo=0;
			estado_siguiente = estado_inicio;
		}
		else if(evento_actual == evento_gana_B)
		{

			bloqueo=1;
			gana(2);
			bloqueo=0;
			estado_siguiente = estado_inicio;
		}
		else
		{
			if (jugador_f == 'A')
			{
				bloqueo=1;
				turno_anim(2);
				bloqueo=0;
				jugador = 'B';
				estado_siguiente = estado_turno_B;
			}
			else if (jugador_f == 'B')
			{
				bloqueo=1;
				turno_anim(1);
				bloqueo=0;
				jugador = 'A';
				estado_siguiente = estado_turno_A;
			}
			break;
		}
    }
}



