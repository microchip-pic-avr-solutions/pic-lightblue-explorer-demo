/**
  @Generated PIC10 / PIC12 / PIC16 / PIC18 MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16LF18456
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above or later
        MPLAB             :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc.h"


void SYSTEM_Initialize(void)
{
    PMD_Initialize();
    I2C1_Initialize();
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    TMR0_Initialize();
    EUSART1_Initialize();
    EUSART2_Initialize();
}

void OSCILLATOR_Initialize(void)
{
    // NOSC HFINTOSC; NDIV 1; 
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power; 
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
    OSCEN = 0x00;
    // HFFRQ 32_MHz; 
    OSCFRQ = 0x06;
    // MFOR not ready; 
    OSCSTAT = 0x00;
    // HFTUN 0; 
    OSCTUNE = 0x00;
}

void PMD_Initialize(void)
{
    // CLKRMD CLKR enabled; SYSCMD SYSCLK enabled; FVRMD FVR enabled; IOCMD IOC enabled; NVMMD NVM enabled; 
    PMD0 = 0x00;
    // TMR0MD TMR0 enabled; TMR1MD TMR1 enabled; TMR4MD TMR4 enabled; TMR5MD TMR5 enabled; TMR2MD TMR2 enabled; TMR3MD TMR3 enabled; TMR6MD TMR6 enabled; 
    PMD1 = 0x00;
    // NCO1MD NCO1 enabled; 
    PMD2 = 0x00;
    // ZCDMD ZCD enabled; CMP1MD CMP1 enabled; ADCMD ADC enabled; CMP2MD CMP2 enabled; DAC1MD DAC1 enabled; 
    PMD3 = 0x00;
    // CCP2MD CCP2 enabled; CCP1MD CCP1 enabled; CCP4MD CCP4 enabled; CCP3MD CCP3 enabled; CCP5MD CCP5 enabled; PWM6MD PWM6 enabled; PWM7MD PWM7 enabled; 
    PMD4 = 0x00;
    // CWG3MD CWG3 enabled; CWG2MD CWG2 enabled; CWG1MD CWG1 enabled; 
    PMD5 = 0x00;
    // U2MD EUSART2 enabled; U1MD EUSART1 enabled; MSSP1MD MSSP1 enabled; MSSP2MD MSSP2 enabled; 
    PMD6 = 0x00;
    // CLC3MD CLC3 enabled; CLC4MD CLC4 enabled; DSM1MD DSM enabled; SMT1MD SMT1 enabled; SMU2MD SMT2 enabled; CLC1MD CLC1 enabled; CLC2MD CLC2 enabled; 
    PMD7 = 0x00;
}


/**
 End of File
*/
