/*
 * fsm.h
 *
 *  Created on: Jun 20, 2026
 *      Author: marcos
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

void actualizar_fsm_juego(void);
void actualizar_fsm_juego_vs_bot(void);
void asignar_jugada(void);
void comprobar_jugada(void);
void siguiente_secuencia(void);
void comprobar(void);
void borrarSW(void);
void turno_anim(int jugador_n);
void gana(int jugador_n);

#endif /* INC_FSM_H_ */
