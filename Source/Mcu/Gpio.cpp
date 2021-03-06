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
#include "stm32f4xx.h"
#include "Gpio.h"

template <typename E>
constexpr auto etoi(E const value)
{
    return static_cast<std::underlying_type_t<E>>(value);
}

void Gpio::SetPinMode(const Pin pin, const Mode mode)
{
    MODER = ((MODER & ~(0x00000003 << (etoi(pin) * 2))) | (etoi(mode) << (etoi(pin) * 2)));
}

void Gpio::SetPinOutputType(const Pin pin, const OutputType type)
{
    OTYPER = ((OTYPER & ~(0x00000001 << etoi(pin))) | (etoi(type) << etoi(pin)));
}

void Gpio::SetPinOutputSpeed(const Pin pin, const OutputSpeed speed)
{
    OSPEEDR = ((OSPEEDR & ~(0x00000003 << (etoi(pin) * 2))) | (etoi(speed) << (etoi(pin) * 2)));
}

void Gpio::SetPinPull(const Pin pin, const Pull pull)
{
    PUPDR = ((PUPDR & ~(0x00000003 << (etoi(pin) * 2))) | (etoi(pull) << (etoi(pin) * 2)));
}

void Gpio::SetPin(const Pin pin)
{
    BSRR |= (1 << etoi(pin));
}

void Gpio::ResetPin(const Pin pin)
{
    BSRR |= ((1 << etoi(pin)) << 16);
}

void Gpio::TogglePin(const Pin pin)
{
    ODR = ODR ^ (1 << etoi(pin));
}

void Gpio::SetAlternateFunction(const Pin pin, const AlternateFunction function)
{
    if(pin < Pin::Pin_8)
    {
        AFRL = ((AFRL & ~(0x0000000F << (etoi(pin) * 4))) | (etoi(function) << (etoi(pin) * 4)));
    }
    else
    {
        AFRH = ((AFRH & ~(0x0000000F << ((etoi(pin)-etoi(Pin::Pin_8)) * 4))) | (etoi(function) << ((etoi(pin)-etoi(Pin::Pin_8)) * 4)));
    }
}

bool Gpio::ReadPin(const Pin pin)
{
    return (IDR & (1 << etoi(pin))) == 0 ? false : true;
}

GpioPin::GpioPin(Gpio& gpio, const Gpio::Pin pin)
    :gpio(gpio), pin(pin)
{

}

void GpioPin::Set(void) const
{
    gpio.SetPin(pin);
}

void GpioPin::Reset(void) const
{
    gpio.ResetPin(pin);
}

void GpioPin::Toggle(void) const
{
    gpio.TogglePin(pin);
}

bool GpioPin::GetState(void) const
{
    return gpio.ReadPin(pin);
}

void GpioPin::SetMode(const Gpio::Mode mode) const
{
    gpio.SetPinMode(pin, mode);
}

void GpioPin::SetOutputType(const Gpio::OutputType type) const
{
    gpio.SetPinOutputType(pin, type);
}

void GpioPin::SetOutputSpeed(const Gpio::OutputSpeed speed) const
{
    gpio.SetPinOutputSpeed(pin, speed);
}

void GpioPin::SetPull(const Gpio::Pull pull) const
{
    gpio.SetPinPull(pin, pull);
}

void GpioPin::SetAlternateFunction(const Gpio::AlternateFunction function) const
{
    gpio.SetAlternateFunction(pin, function);
}
