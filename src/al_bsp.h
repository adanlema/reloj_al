/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_BSP_H
#define AL_BSP_H

/*==================[inclusions]=============================================*/
#include "al_gpio.h"
#include "al_display7seg.h"
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef struct board_s {
    DigitalInput_pt  f1;
    DigitalInput_pt  f2;
    DigitalInput_pt  f3;
    DigitalInput_pt  f4;
    DigitalOutput_pt aceptar;
    DigitalOutput_pt rechazar;
    DigitalOutput_pt buz;
    display_pt       display;
} const * const board_pt;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief Board Create
 * Se encarga de crear una placa, la cual contendra cuatro entradas digitales y
 * seis salidas digitales.
 * @return board_pt Retorna el puntero de la estructura de la placa.
 */
board_pt BoardptCreate(void);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif