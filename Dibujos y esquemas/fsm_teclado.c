void limpiarX()
{
    X = leerPinesSW();
    delay(20);
    if (X != leerPinesSW())
    {
        X = leerPinesSW();  //aca se obtiene el componente x...
        printf("Se esta presionando el boton \n");
        SW_presionado = 1;
    }
    else
    {
        X = 0;
        printf("¡Falso 1s detectado! \n");
        printf("Saliendo de la rutina de atención...\n");
        SW_presionado = 0,
    }
    while (leerSW() != 0)
    {
        //mientras alguno de los SW's siga presionado no continua el programa...
    }
}

//inicialización
int Y = 1;
y2_high_z();
y3_high_z();
y4_high_z();
y1_high();
Y = 1;
//

void int_barrido_20ms();
{
    limpiarX();
    if (SW_presionado = 0)
    {
        Siguiente_Y();
    }
    if (SW_presionado = 1)
    {
        int_barrido_off();  //apagar esta interrupcion hasta que empiece nuevamente
                            //el turno de algun jugador o el juego termine...
    }
}

void Siguiente_Y()
{
        if (Y = 1)
        {
            y1_high_z();
            y3_high_z();
            y4_high_z();
            y2_high();
            Y = 2;
        }
        else if (Y = 2)
        {
            y1_high_z();
            y2_high_z();
            y4_high_z();
            y3_high();
            Y = 3;
        }
        else if (Y = 3)
        {
            y1_high_z();
            y2_high_z();
            y3_high_z();
            y4_high();
            Y = 4;
        }
        else if (Y = 4)
        {
            y2_high_z();
            y3_high_z();
            y4_high_z();
            y1_high();
            Y = 1;
        }
    }
}

int leerSW()
{
    if (leerSWX1() = 1)
    {
        X = 1;
    }
    else if (leerSWX2() = 1)
    {
        X = 2;
    }
    else if (leerSWX3() = 1)
    {
        X = 3;
    }
    else if (leerSWX4() = 1)
    {
        X = 4;
    }
    else
    {
        X = 0;
    }
}

int leerPinesSW()
{
    //devolver en leerPinesSW() el x=1 o 2 o 3 o 4 presionado. Sino un x=0
}


