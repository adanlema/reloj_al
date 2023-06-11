#ifndef RELOJ_H
#define RELOJ_H

/*==================[inclusions]=============================================*/
#include <stdbool.h>
#include <stdint.h>
#include "al_bsp.h"
/*==================[macros]=================================================*/
#define TIME_SIZE 6
#define TIME_POST 5

#define CONTROLAR_REBALSE_MIN(DECENA, UNIDAD, SIGUIENTE)                                           \
    if (UNIDAD > 9) {                                                                              \
        DECENA = DECENA + UNIDAD / 9;                                                              \
        UNIDAD = 0 + ((UNIDAD / 9) * UNIDAD - 10);                                                 \
    }                                                                                              \
    if (DECENA > 5) {                                                                              \
        DECENA = DECENA - 6;                                                                       \
        SIGUIENTE += 1;                                                                            \
    }

/*==================[typedef]================================================*/
typedef struct clock_s *  clock_t;
typedef struct alarma_s * alarma_pt;
typedef void (*evento_pt)(board_t);
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
clock_t ClockCreate(int tics_por_seg, evento_pt funcion);
void    ClockTick(clock_t reloj);

bool    ClockGetTime(clock_t reloj, uint8_t * hora, int size);
bool    ClockSetTime(clock_t reloj, const uint8_t * hora, int size);
bool    ClockGetAlarma(clock_t reloj, uint8_t * hora, int size);
bool    ClockGetAlarmaHabilitada(clock_t reloj);

void    ClockSetAlarma(clock_t reloj, const uint8_t * hora, int size);
void    ClockToggleAlarma(clock_t reloj);
void    ClockPosponerAlarma(clock_t reloj, uint8_t time_post);
void    ClockCancelarAlarma(clock_t reloj);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif