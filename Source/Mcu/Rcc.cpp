/*****************************************************************
 * Copyright (c) 2020 CodeAbove.
 * All rights reserved.
 *
 * This software component is licensed by CodeAbove under
 * BSD 3-Clause license, the 'License'. You may not use
 * this file except in compliance with the License. You
 * may obtain a copy of the License at:
 *                  opensource.org/licenses/BSD-3-Clause
 ****************************************************************/
#include "Rcc.h"

void Rcc::EnableHighSpeedInternalClock(void)
{
    CR.bits.HSION = 1;

    while(CR.bits.HSIRDY != 1)
    {
        // Wait for it!
    }
}

void Rcc::SetHighSpeedInternalClockTrimming(uint8_t trimming)
{
    CR.bits.HSITRIM = trimming;
}

void Rcc::EnableHighSpeedExternalClock(void)
{
    CR.bits.HSEON = 1;

    while(CR.bits.HSERDY != 1)
    {
        // Wait for it!
    }
}

void Rcc::EnablePll(void)
{
    CR.bits.PLLON = 1;

    while(CR.bits.PLLRDY != 1)
    {
        // Wait for it!
    }
}

void Rcc::ConfigurePll(PllClockSource source, PllFactors factors)
{
    SetPllClockSource(source);
    SetPllFactors(factors);
}

void Rcc::SetAhbPrescaler(AhbPrescaler prescaler)
{
    CFGR.bits.HPRE = prescaler;
}

void Rcc::SetApb1Prescaler(Apb1Prescaler prescaler)
{
    CFGR.bits.PPRE1 = prescaler;
}

void Rcc::SetApb2Prescaler(Apb2Prescaler prescaler)
{
    CFGR.bits.PPRE2 = prescaler;
}

void Rcc::SetSystemClockSource(SystemClockSource source)
{
    CFGR.bits.SW = source;

    while(CFGR.bits.SWS != source)
    {
        // Wait for it!
    }
}

void Rcc::EnableGpioA(void)
{
    AHB1ENR.bits.GPIOAEN = 1;
}

void Rcc::EnableGpioB(void)
{
    AHB1ENR.bits.GPIOBEN = 1;
}

void Rcc::EnableGpioC(void)
{
    AHB1ENR.bits.GPIOCEN = 1;
}

void Rcc::EnableI2c1(void)
{
    APB1ENR.bits.I2C1EN = 1;
}

void Rcc::EnablePowerInterface(void)
{
    APB1ENR.bits.PWREN = 1;
}

void Rcc::EnableSpi1(void)
{
    APB2ENR.bits.SPI1EN = 1;
}

void Rcc::SetPllClockSource(PllClockSource source)
{
    PLLCFGR.bits.PLLSRC = source;
}

void Rcc::SetPllFactors(PllFactors factors)
{
    PLLCFGR.bits.PLLM = factors.m;
    PLLCFGR.bits.PLLN = factors.n;
    PLLCFGR.bits.PLLP = factors.p;
}

