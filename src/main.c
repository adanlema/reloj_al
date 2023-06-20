/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "al_gpio.h"
#include "al_bsp.h"
#include "al_display.h"
#include "reloj.h"
#include "al_modo.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static board_t       board_educia;
static clock_t       reloj;
static modo_t        modo;
static uint8_t       hora_actual[TIME_SIZE];

static const uint8_t LIMITE_MINUTOS[] = {6, 0};
static const uint8_t LIMITE_HORAS[]   = {2, 4};
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
    numero[1]++;
    if (numero[1] > 9) {
        numero[1] = 0;
        numero[0]++;
    }
    if ((numero[0] >= limite[0]) && (numero[1] >= limite[1])) {
        numero[1] = 0;
        numero[0] = 0;
    }
}
void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {

    if ((numero[0] == 0) && (numero[1] == 0)) {
        if (limite[0] == 6) {
            numero[0] = limite[0] - 1;
        } else {
            numero[0] = limite[0];
        }
        if (limite[1] == 0) {
            numero[1] = 9;
        } else {
            numero[1] = limite[1] - 1;
        }
    } else if (numero[1] == 0) {
        numero[1] = 9;
        numero[0]--;
    } else {
        numero[1]--;
    }
}

/*==================[external functions definition]==========================*/
void CambiarModo(modo_t estado) {
    modo = estado;
    switch (modo) {
        case SIN_CONFIGURAR:
            DisplayParpadeoDigitos(board_educia->display, 0, 3, 200);
            break;
        case MOSTRANDO_HORA:
            DisplayParpadeoDigitos(board_educia->display, 0, 0, 0);
            break;
        case AJUSTAR_MINUTOS_ACTUAL:
            DisplayParpadeoDigitos(board_educia->display, 2, 3, 200);
            break;
        case AJUSTAR_HORAS_ACTUAL:
            DisplayParpadeoDigitos(board_educia->display, 0, 1, 200);
            break;
        case AJUSTAR_MINUTOS_ALARMA:
            DisplayParpadeoDigitos(board_educia->display, 2, 3, 200);
            DisplayTogglePunto(board_educia->display, 0);
            DisplayTogglePunto(board_educia->display, 1);
            DisplayTogglePunto(board_educia->display, 2);
            DisplayTogglePunto(board_educia->display, 3);
            break;
        case AJUSTAR_HORAS_ALARMA:
            DisplayParpadeoDigitos(board_educia->display, 0, 1, 200);
            DisplayTogglePunto(board_educia->display, 0);
            DisplayTogglePunto(board_educia->display, 1);
            DisplayTogglePunto(board_educia->display, 2);
            DisplayTogglePunto(board_educia->display, 3);
            break;
        default:
            break;
    }
}
int main(void) {
    board_educia = board_Create();
    reloj        = ClockCreate(10, AlarmaToggle);

    SysTickConfig(1000);
    CambiarModo(SIN_CONFIGURAR);

    while (true) {

        if (DigitalInput_HasActivate(board_educia->aceptar)) {
            if (modo == AJUSTAR_MINUTOS_ACTUAL) {
                CambiarModo(AJUSTAR_HORAS_ACTUAL);
            } else if (modo == AJUSTAR_HORAS_ACTUAL) {
                ClockSetTime(reloj, hora_actual, sizeof(hora_actual));
                CambiarModo(MOSTRANDO_HORA);
            }
        }
        if (DigitalInput_HasActivate(board_educia->rechazar)) {
            if (ClockGetTime(reloj, hora_actual, sizeof(hora_actual))) {
                CambiarModo(MOSTRANDO_HORA);
            } else {
                CambiarModo(SIN_CONFIGURAR);
            }
        }

        if (DigitalInput_HasActivate(board_educia->f1)) {
            CambiarModo(AJUSTAR_MINUTOS_ACTUAL);
            ClockGetTime(reloj, hora_actual, sizeof(hora_actual));
            DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
        }
        if (DigitalInput_HasActivate(board_educia->f2)) {
        }
        if (DigitalInput_HasActivate(board_educia->f3)) {
            if (modo == AJUSTAR_MINUTOS_ACTUAL) {
                DecrementarBCD(&hora_actual[2], LIMITE_MINUTOS);
            } else if (modo == AJUSTAR_HORAS_ACTUAL) {
                DecrementarBCD(&hora_actual[0], LIMITE_HORAS);
            }
            DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
        }
        if (DigitalInput_HasActivate(board_educia->f4)) {
            if (modo == AJUSTAR_MINUTOS_ACTUAL) {
                IncrementarBCD(&hora_actual[2], LIMITE_MINUTOS);
            } else if (modo == AJUSTAR_HORAS_ACTUAL) {
                IncrementarBCD(&hora_actual[0], LIMITE_HORAS);
            }
            DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
        }
    }
}

void SysTick_Handler(void) {
    static bool valor_anterior = false;
    bool        valor_actual;
    uint8_t     hora[TIME_SIZE];

    DisplayRefresh(board_educia->display);
    valor_actual = ClockTick(reloj);

    if (valor_actual != valor_anterior) {
        valor_anterior = valor_actual;
        if (modo <= MOSTRANDO_HORA) {
            ClockGetTime(reloj, hora, 4);
            DisplayWriteBCD(board_educia->display, hora, 4);
            if (valor_actual) {
                DisplayTogglePunto(board_educia->display, 1);
            }
        }
    };
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/