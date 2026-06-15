//definición de tipo de estados

char jugador;
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
    estado_fin_del_juego
}estado;

typedef enum{
    evento_presionar,   //evento_1 ~ boton presionado
    evento_jugada_invalida_de_A,
    evento_jugada_invalida_de_B,
    evento_jugada_valida,
    evento_sin_presionar,
    evento_no_gana_A,
    evento_no_gana_B,
    evento_soltar
}evento;

void actualizar_fsm_juego(estado *estado_siguiente, evento evento_actual)
{
    switch (*estado_siguiente)
    {
        case estado_inicio:
            {
                printf("Estado: Inicio de partida \n");  
                printf("Modo común \n");
                estado_siguiente = estado_2;
            }
        break;
        case estado_inicializar:
            {
                printf("Estado: Inicializando matríz \n");
                printf("Vaciando Matriz...");
                inicializar_matriz();
                estado_siguiente = estado_3;
            }
        break;
        case estado_turno_A:
            {
                printf("Estado: Turno jugador A \n");
                printf("Esperando que se presione un botón... \n");
                jugador = 'A';
                if evento_presionar()
                {
                    estado_siguiente = estado_esperar_soltar;   
                }
        case estado_esperar_soltar:
            {
                printf("Estado: Esperando soltar \n");
                if evento_soltar()
                {
                    estado_siguiente = estado_asignar_jugada;
                }
            }
                
            }
        break;
        case estado_turno_B:
            {
                printf("Estado: Turno jugador B \n");
                printf("Esperando que se presione un botón... \n");
                jugador = 'B';
                if evento_presionar()
                {
                    estado_siguiente = estado_esperar_soltar;   
                }
        case estado_esperar_soltar:
            {
                printf("Estado: Esperando soltar \n");
                if evento_soltar()
                {
                    estado_siguiente = estado_asignar_jugada;
                }
            }
                
            }
        break;
        case estado_asignar_jugada:
            {
                printf("Estado: Asignar jugada \n");
                asignar_jugada();
                if (evento_actual = evento_jugada_invalida_de_A)
                {
                    estado_siguiente = estado_turno_B;
                    Reactivar_SW();
                    
                } 
                else if (evento_actual = evento_jugada_invalida_de_B)
                {
                    estado_siguiente = estado_turno_A
                    Reactivar_SW();
                }
                else (evento_actual = evento_jugada_valida)
                {
                    estado_siguiente = estado_comprobar_jugada;
                }
            }
        break;
        case estado_comprobar_jugada:
            {
                printf("Estado: Comprobar Victoria/Empate \n");
                comprobar_victoria();
                if (evento_actual = evento_no_gana_B)
                {
                    printf("El juego continua... \n");
                    estado_siguiente = estado_turno_A;
                }
                else if (evento_actual = evento_no_gana_A)
                {
                    printf("El juego continua... \n");
                    estado_siguiente = estado_turno_B;
                }
                else if (evento_actual = evento_gana_A)
                {
                    printf("¡Ganó el jugador A! \n");
                    estado_siguiente = estado_gana_A;
                }
                else if (evento_actual = evento_gana_B)
                {
                    printf("¡Ganó el jugador B! \n");
                    estado_siguiente = estado_gana_B;
                }
                else
                {
                    printf("¡Increible! pero empataron... \n");
                    estado_siguiente = estado_menu; //o fin_del_juego() mejor...
                }
            }
        break;
        case estado_gana_A:
            {
                prinft("Animación de victoria jugador A \n");
                animación_gana_A();
            }
        break;
        case estado_gana_B:
            {
                prinft("Animación de victoria jugador B \n");
                animación_gana_A();
            }
        break;
        case estado_fin_del_juego:
            {
                prinft("¡Fin del juego! \n");
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

void loop_main()
{
    if (evento_actual != evento_fin_del_juego)
    {
        actualizar_fsm_juego(param); 
    }
    else
    {
        actualizar_menu_principal(param);
    }
    
}

