//definición de tipo de estados

char jugador;
typedef enum{
    estado_1,   //leer SW por primera vez
    estado_2,   
    estado_3,
    estado_4
}estado;

typedef enum{
    evento_1,   //evento_1 ~ boton presionado
    evento_2,
    evento_3,
    evento_4
}evento;

void actualizar_fsm_teclado(estado *estado_siguiente, evento evento_actual)
{
    switch (*estado_siguiente)
    {
        case estado_1:
            {
                printf("Estado: Inicio de partida \n");  
                printf("Modo común \n");
                estado_siguiente = estado_2;
            }
        break;
        case estado_2:
            {
                printf("Estado: Inicializando matríz \n");
                printf("Vaciando Matriz...");
                estado_siguiente = estado_3;
            }
        break;
        case estado_3:
            {
                printf("Estado: Turno jugador A \n");
                printf("Esperando que se presione un botón... \n");
                jugador = 'A';
                if (evento_SW = 1)
                {
                    estado_siguiente = estado_4;   
                }
        case estado_4:
            {
                printf("Estado: Guardando posición \n");
                estado_siguiente = estado_6;
            }
                
            }
        break;
        case estado_6:
            {
                printf("Estado: Asignar jugada \n");
                asignar_jugada();
                borrar_datos_sw();  //Una vez utilizados los datos de la posicion del
                                    //SW es necesario borrarlos
                if (evento_actual = evento_3)
                {
                    estado_siguiente = estado_3;
                    
                } 
                else if (evento_actual = evento_4)
                {
                    estado_siguiente = estado_5;
                }
                else
                {
                    estado_siguiente = estado_7;
                }
            }
        break;
        case estado_6:
            {
                
            }
        break;
        case estado_7:
            {
                printf("Estado: Comprobar Victoria/Empate \n");
                comprobar_victoria();
                if (evento_actual = no_win_B)
                {
                    printf("El juego continua... \n");
                    estado_siguiente = estado_3;
                }
                else if (evento_actual = no_win_A)
                {
                    printf("El juego continua... \n");
                    estado_siguiente = estado_5;
                }
                else if (evento_actual = win_A)
                {
                    printf("¡Ganó el jugador A! \n");
                    estado_siguiente = estado_menu;
                }
                else if (evento_actual = win_B)
                {
                    printf("¡Ganó el jugador B! \n");
                    estado_siguiente = estado_menu;
                }
                else
                {
                    printf("¡Increible! pero empataron... \n");
                    estado_siguiente = estado_menu; //o fin_del_juego() mejor...
                }
            }
        default:
            {
                printf("Estado: default \n");
            }
        break;
    }
}

void loop_main()
{
    
}

