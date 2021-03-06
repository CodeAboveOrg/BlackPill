#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Application/Application.h"
#include "Application/Display/DisplayBuffer.h"
#include "Application/Display/impl/ST7789Display.h"
#include "Application/Servo/impl/PCA9685Servo.h"
#include "Drivers/ST7789/ST7789.h"
#include "Drivers/ST7789_impl/ST7789Pin.h"
#include "Drivers/ST7789_impl/ST7789Spi.h"
#include "Libraries/UGUI/ugui.h"
#include "Mcu/Flash.h"
#include "Mcu/Gpio.h"
#include "Mcu/Pwr.h"
#include "Mcu/Rcc.h"
#include "Mcu/Spi.h"
#include "Mcu/Core/Systick.h"
#include "Drivers/Led/Led.h"
#include "Drivers/Switch/Switch.h"
#include "stm32f4xx.h"

int main(void)
{
    Rcc &rcc = *reinterpret_cast<Rcc *>(RCC_BASE);
    rcc.EnableGpioA();
    rcc.EnableGpioB();
    rcc.EnableGpioC();
    rcc.EnablePowerInterface();
    rcc.EnableSpi1();
    rcc.EnableI2c1();

    Flash &flash = *reinterpret_cast<Flash *>(FLASH_R_BASE);
    flash.SetLatency(Flash::WaitStates::WS_3);

    Pwr &pwr = *reinterpret_cast<Pwr *>(PWR_BASE);
    pwr.SetRegulatorVoltageScaling(Pwr::RegulatorVoltageScaling::Scale_1);

    rcc.EnableHighSpeedExternalClock();
    rcc.ConfigurePll(Rcc::PllClockSource::HSE, Rcc::PllFactors(12, 96, Rcc::PllFactorP::P_2));
    rcc.EnablePll();
    rcc.SetAhbPrescaler(Rcc::AhbPrescaler::Prescaler_1);
    rcc.SetApb1Prescaler(Rcc::Apb1Prescaler::Prescaler_2);
    rcc.SetApb2Prescaler(Rcc::Apb2Prescaler::Prescaler_1);
    rcc.SetSystemClockSource(Rcc::SystemClockSource::PLL);

    Systick &systick = *reinterpret_cast<Systick *>(SysTick_BASE);
    systick.SetReload1msTick(100000000);
    systick.SetClockSource(Systick::ClockSource::AHB);
    systick.EnableInterrupt();
    systick.Enable();

    Gpio &gpioA = *reinterpret_cast<Gpio *>(GPIOA_BASE);
    Gpio &gpioB = *reinterpret_cast<Gpio *>(GPIOB_BASE);
    Gpio &gpioC = *reinterpret_cast<Gpio *>(GPIOC_BASE);
    GpioPin pinA0 {gpioA, Gpio::Pin::Pin_0}; // User KEY switch
    GpioPin pinA5 {gpioA, Gpio::Pin::Pin_5}; // Display: SCL SPI1
    GpioPin pinA6 {gpioA, Gpio::Pin::Pin_6}; // Display: DC
    GpioPin pinA7 {gpioA, Gpio::Pin::Pin_7}; // Display: SDA SPI1
    GpioPin pinB0 {gpioB, Gpio::Pin::Pin_0}; // Display: RES
    GpioPin pinB6 {gpioB, Gpio::Pin::Pin_6}; // Servo Controller: SCL I2C1
    GpioPin pinB7 {gpioB, Gpio::Pin::Pin_7}; // Servo Controller: SDA I2C1
    GpioPin pinC13 {gpioC, Gpio::Pin::Pin_13}; // User LED

    pinB0.SetMode(Gpio::Mode::Output);
    pinA6.SetMode(Gpio::Mode::Output);
    pinA5.SetMode(Gpio::Mode::AlternateFunction);
    pinA5.SetAlternateFunction(Gpio::AlternateFunction::AF05_SPI1_I2S1SPI2_I2S2_SPI3_I2S3);
    pinA7.SetMode(Gpio::Mode::AlternateFunction);
    pinA7.SetAlternateFunction(Gpio::AlternateFunction::AF05_SPI1_I2S1SPI2_I2S2_SPI3_I2S3);
    pinB6.SetMode(Gpio::Mode::AlternateFunction);
    pinB6.SetOutputSpeed(Gpio::OutputSpeed::High);
    pinB6.SetOutputType(Gpio::OutputType::OpenDrain);
    pinB6.SetPull(Gpio::Pull::Up);
    pinB6.SetAlternateFunction(Gpio::AlternateFunction::AF04_I2C1_I2C2_I2C3);
    pinB7.SetMode(Gpio::Mode::AlternateFunction);
    pinB7.SetOutputSpeed(Gpio::OutputSpeed::High);
    pinB7.SetOutputType(Gpio::OutputType::OpenDrain);
    pinB7.SetPull(Gpio::Pull::Up);
    pinB7.SetAlternateFunction(Gpio::AlternateFunction::AF04_I2C1_I2C2_I2C3);

    Spi &spi1 = *reinterpret_cast<Spi *>(SPI1_BASE);
    // spi1.SetBaudRate(Spi::BaudRate::PCLK_div8);
    // spi1.SetClockPolarity(Spi::ClockPolarity::CPOL_0);
    // spi1.SetClockPhase(Spi::ClockPhase::CPHA_0);
    // spi1.SetDataFrameFormat(Spi::DataFrameFormat::Format_8BitData);
    // spi1.SetFrameFormat(Spi::FrameFormat::MSB);
    // spi1.SetSoftwareNSS();
    // spi1.SetTransferMode(Spi::TransferMode::FullDuplex);
    // spi1.SetMasterSelection(Spi::MasterSelection::Master);
    // spi1.Enable();
    spi1.Enable_Hardcoded();

    Led blueLed(pinC13);
    Switch userSwitch(pinA0);

    ST7789Spi st7789Spi { spi1 };
    ST7789Pin rstPin { pinB0 };
    ST7789Pin dcPin { pinA6 };
    // ST7789 st7789 { st7789Spi, rstPin, dcPin, SetPixelAdapter::displayBuffer.u8 };
    // st7789.Init();
    // SetPixelAdapter::pST7789 = &st7789;
    
    UG_GUI gui;
    UG_Init(&gui, DisplayBuffer<240,240>::SetPixel, 240, 240);
    UG_FontSelect(&FONT_8X14);
    UG_ConsoleSetBackcolor(C_BLACK);
    UG_ConsoleSetForecolor(C_WHITE);

    ST7789Display ips { 240, 240, st7789Spi, rstPin, dcPin, DisplayBuffer<240,240>::buf.u8 };

    CodeAbove::Drivers::PCA9685ServoController servoCtrl;
    CodeAbove::PCA9685Servo servo { servoCtrl, 0 };

    Application app { blueLed, ips, servo };

    char number[11];
    uint32_t i = 0u;
    while(1)
    {   
        sprintf(number, "%lu\n", i++);
        UG_PutString(0,0,number);
        app.Perform();
    }
}

extern "C" void SysTick_Handler(void)
{
    SystickInterruptHandler::ISR();
    ST7789::Task1ms();
}
