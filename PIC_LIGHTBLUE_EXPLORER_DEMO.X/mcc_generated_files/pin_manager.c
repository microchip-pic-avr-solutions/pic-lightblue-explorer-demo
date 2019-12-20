/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16LF18456
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB             :  MPLAB X 5.20

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include "pin_manager.h"




void (*IOCAF6_InterruptHandler)(void);


void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATE = 0x00;
    LATA = 0x31;
    LATB = 0x00;
    LATC = 0xC1;

    /**
    TRISx registers
    */
    TRISE = 0x08;
    TRISA = 0xDE;
    TRISB = 0xEF;
    TRISC = 0x25;

    /**
    ANSELx registers
    */
    ANSELC = 0x00;
    ANSELB = 0xC9;
    ANSELA = 0x0E;

    /**
    WPUx registers
    */
    WPUE = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    WPUC = 0x00;

    /**
    ODx registers
    */
    ODCONE = 0x00;
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;

    /**
    SLRCONx registers
    */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;

    /**
    INLVLx registers
    */
    INLVLA = 0x3F;
    INLVLB = 0x3F;
    INLVLC = 0x3F;


    /**
    IOCx registers 
    */
    //interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF6 = 0;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN6 = 0;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP6 = 1;



    // register default IOC callback functions at runtime; use these methods to register a custom function
    IOCAF6_SetInterruptHandler(IOCAF6_DefaultInterruptHandler);
   
    // Enable IOCI interrupt 
    PIE0bits.IOCIE = 1; 
    
	
    SSP1CLKPPS = 0x09;   //RB1->MSSP1:SCL1;    
    SSP2DATPPS = 0x12;   //RC2->MSSP2:SDI2;    
    RB1PPS = 0x13;   //RB1->MSSP1:SCL1;    
    RC3PPS = 0x16;   //RC3->MSSP2:SDO2;    
    RB2PPS = 0x14;   //RB2->MSSP1:SDA1;    
    RC1PPS = 0x15;   //RC1->MSSP2:SCK2;    
    RC4PPS = 0x25;   //RC4->EUSART2:TX2;    
    RB4PPS = 0x0F;   //RB4->EUSART1:TX1;    
    RX2DTPPS = 0x15;   //RC5->EUSART2:RX2;    
    SSP1DATPPS = 0x0A;   //RB2->MSSP1:SDA1;    
    RX1DTPPS = 0x0D;   //RB5->EUSART1:RX1;    
    SSP2CLKPPS = 0x11;   //RC1->MSSP2:SCK2;    
}
  
void PIN_MANAGER_IOC(void)
{   
	// interrupt on change for pin IOCAF6
    if(IOCAFbits.IOCAF6 == 1)
    {
        IOCAF6_ISR();  
    }	
}

/**
   IOCAF6 Interrupt Service Routine
*/
void IOCAF6_ISR(void) {

    // Add custom IOCAF6 code

    // Call the interrupt handler for the callback registered at runtime
    if(IOCAF6_InterruptHandler)
    {
        IOCAF6_InterruptHandler();
    }
    IOCAFbits.IOCAF6 = 0;
}

/**
  Allows selecting an interrupt handler for IOCAF6 at application runtime
*/
void IOCAF6_SetInterruptHandler(void (* InterruptHandler)(void)){
    IOCAF6_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCAF6
*/
void IOCAF6_DefaultInterruptHandler(void){
    // add your IOCAF6 interrupt custom code
    // or set custom function using IOCAF6_SetInterruptHandler()
}

/**
 End of File
*/