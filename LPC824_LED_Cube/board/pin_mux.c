/*
 * Copyright 2017-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********/

/**
 * @file    pin_mux.c
 * @brief   Board pins file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_spi.h"
#include "fsl_swm.h"
#include "pin_mux.h"

/**
 * @brief Set up and initialize all required blocks and functions related to the board hardware.
 */
void BOARD_InitBootPins(void) {
    /* Enables clock for IOCON block.: Enable. */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    //CLOCK_EnableClock(kCLOCK_I2c0);
    /* Enable clock of spi0. */
    CLOCK_EnableClock(kCLOCK_Spi0);
    /* Enables clock for switch matrix.: 0x01u */
     CLOCK_EnableClock(kCLOCK_Swm);

    GPIO->DIR[0] = ((GPIO->DIR[0] &
                     /* Mask bits to zero which are setting */
                     (~(GPIO_DIR_DIRP_MASK)))

                    /* Selects pin direction for pin PIOm_n (bit 0 = PIOn_0, bit 1 = PIOn_1, etc.). Supported pins
                     * depends on the specific device and package. 0 = input. 1 = output.: 0x1000u */
                    | GPIO_DIR_DIRP(0x1E030000u));

    /*
     * 8-bit Decoder drivers
     */
    const uint32_t IOCON_INDEX_PIO0_25_config = (/* Selects pull-up function */
                                                 IOCON_PIO_MODE_PULLUP |
                                                 /* Enable hysteresis */
                                                 IOCON_PIO_HYS_EN |
                                                 /* Input not invert */
                                                 IOCON_PIO_INV_DI |
                                                 /* Disables Open-drain function */
                                                 IOCON_PIO_OD_DI |
                                                 /* Bypass input filter */
                                                 IOCON_PIO_SMODE_BYPASS |
                                                 /* IOCONCLKDIV0 */
                                                 IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_25, IOCON_INDEX_PIO0_25_config);

    const uint32_t IOCON_INDEX_PIO0_26_config = (/* Selects pull-up function */
                                                 IOCON_PIO_MODE_PULLUP |
                                                 /* Enable hysteresis */
                                                 IOCON_PIO_HYS_EN |
                                                 /* Input not invert */
                                                 IOCON_PIO_INV_DI |
                                                 /* Disables Open-drain function */
                                                 IOCON_PIO_OD_DI |
                                                 /* Bypass input filter */
                                                 IOCON_PIO_SMODE_BYPASS |
                                                 /* IOCONCLKDIV0 */
                                                 IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_26, IOCON_INDEX_PIO0_26_config);

    const uint32_t IOCON_INDEX_PIO0_27_config = (/* Selects pull-up function */
                                                 IOCON_PIO_MODE_PULLUP |
                                                 /* Enable hysteresis */
                                                 IOCON_PIO_HYS_EN |
                                                 /* Input not invert */
                                                 IOCON_PIO_INV_DI |
                                                 /* Disables Open-drain function */
                                                 IOCON_PIO_OD_DI |
                                                 /* Bypass input filter */
                                                 IOCON_PIO_SMODE_BYPASS |
                                                 /* IOCONCLKDIV0 */
                                                 IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_27, IOCON_INDEX_PIO0_27_config);

    const uint32_t IOCON_INDEX_PIO0_28_config = (/* Selects pull-up function */
                                                 IOCON_PIO_MODE_PULLUP |
                                                 /* Enable hysteresis */
                                                 IOCON_PIO_HYS_EN |
                                                 /* Input not invert */
                                                 IOCON_PIO_INV_DI |
                                                 /* Disables Open-drain function */
                                                 IOCON_PIO_OD_DI |
                                                 /* Bypass input filter */
                                                 IOCON_PIO_SMODE_BYPASS |
                                                 /* IOCONCLKDIV0 */
                                                 IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_28, IOCON_INDEX_PIO0_28_config);
    /**/

    /*
     * Board LEDS
     */
    const uint32_t IOCON_INDEX_PIO0_15_config = (/* Selects pull-up function */
                                                 IOCON_PIO_MODE_PULLUP |
                                                 /* Enable hysteresis */
                                                 IOCON_PIO_HYS_EN |
                                                 /* Input not invert */
                                                 IOCON_PIO_INV_DI |
                                                 /* Disables Open-drain function */
                                                 IOCON_PIO_OD_DI |
                                                 /* Bypass input filter */
                                                 IOCON_PIO_SMODE_BYPASS |
                                                 /* IOCONCLKDIV0 */
                                                 IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_15, IOCON_INDEX_PIO0_15_config);

    const uint32_t IOCON_INDEX_PIO0_16_config = (/* Selects pull-up function */
                                                 IOCON_PIO_MODE_PULLUP |
                                                 /* Enable hysteresis */
                                                 IOCON_PIO_HYS_EN |
                                                 /* Input not invert */
                                                 IOCON_PIO_INV_DI |
                                                 /* Disables Open-drain function */
                                                 IOCON_PIO_OD_DI |
                                                 /* Bypass input filter */
                                                 IOCON_PIO_SMODE_BYPASS |
                                                 /* IOCONCLKDIV0 */
                                                 IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_16, IOCON_INDEX_PIO0_16_config);

    const uint32_t IOCON_INDEX_PIO0_17_config = (/* Selects pull-up function */
                                                 IOCON_PIO_MODE_PULLUP |
                                                 /* Enable hysteresis */
                                                 IOCON_PIO_HYS_EN |
                                                 /* Input not invert */
                                                 IOCON_PIO_INV_DI |
                                                 /* Disables Open-drain function */
                                                 IOCON_PIO_OD_DI |
                                                 /* Bypass input filter */
                                                 IOCON_PIO_SMODE_BYPASS |
                                                 /* IOCONCLKDIV0 */
                                                 IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_17, IOCON_INDEX_PIO0_17_config);
    /**/

    /*
     * User buttons
     */
    const uint32_t pio1_config = (/* Selects pull-up function */
                                  IOCON_PIO_MODE_PULLUP |
                                  /* Enable hysteresis */
                                  IOCON_PIO_HYS_EN |
                                  /* Input not invert */
                                  IOCON_PIO_INV_DI |
                                  /* Disables Open-drain function */
                                  IOCON_PIO_OD_DI |
                                  /* Bypass input filter */
                                  IOCON_PIO_SMODE_BYPASS |
                                  /* IOCONCLKDIV0 */
                                  IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_1, pio1_config);

    const uint32_t pio6_config = (/* Selects pull-up function */
                                  IOCON_PIO_MODE_PULLUP |
                                  /* Enable hysteresis */
                                  IOCON_PIO_HYS_EN |
                                  /* Input not invert */
                                  IOCON_PIO_INV_DI |
                                  /* Disables Open-drain function */
                                  IOCON_PIO_OD_DI |
                                  /* Bypass input filter */
                                  IOCON_PIO_SMODE_BYPASS |
                                  /* IOCONCLKDIV0 */
                                  IOCON_PIO_CLKDIV0);
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_6, pio6_config);
    /**/

    /*
     * ADC
     */
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);

    /*
     * SPI
     */
    SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_SCK, kSWM_PortPin_P0_14);
    SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_MOSI, kSWM_PortPin_P0_12);
    SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_MISO, kSWM_PortPin_P0_13);
    SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_SSEL0, kSWM_PortPin_P0_15);

    /*
     * I2C
     */
    SWM_SetFixedPinSelect(SWM0, kSWM_I2C0_SDA, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_I2C0_SCL, true);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);

}
