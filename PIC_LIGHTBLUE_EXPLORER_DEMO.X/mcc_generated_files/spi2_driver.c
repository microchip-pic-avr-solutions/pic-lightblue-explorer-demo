/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "spi2_driver.h"

#pragma warning disable 520     

void (*spi2_interruptHandler)(void);   

inline void spi2_close(void)
{
    SSP2CON1bits.SSPEN = 0;
}

//con1 == SSPxCON1, stat == SSPxSTAT, add == SSPxADD, operation == Master/Slave
typedef struct { uint8_t con1; uint8_t stat; uint8_t add; uint8_t operation; } spi2_configuration_t;
static const spi2_configuration_t spi2_configuration[] = {
    { 0x1a, 0x80, 0x3f, 0 },
    { 0xa, 0x40, 0x3f, 0 }
};

//Setup SPI
bool spi2_open(spi2_modes spiUniqueConfiguration)
{
    if(!SSP2CON1bits.SSPEN)
    {
        //setup PPS pins
        SSP2CLKPPS = 17;
        SSP2DATPPS = 18;
        RC1PPS = 21;
        RC3PPS = 22;

        //setup SPI
        SSP2STAT = spi2_configuration[spiUniqueConfiguration].stat;
        SSP2CON1 = (uint8_t)(spi2_configuration[spiUniqueConfiguration].con1 | 0x20);
        SSP2CON2 = 0x00;
        SSP2ADD  = (uint8_t)(spi2_configuration[spiUniqueConfiguration].add);

        TRISCbits.TRISC1 = (uint8_t)(spi2_configuration[spiUniqueConfiguration].operation);
        return true;
    }
    return false;
}

// Full Duplex SPI Functions
uint8_t spi2_exchangeByte(uint8_t b)
{
    SSP2BUF = b;
    while(!PIR3bits.SSP2IF);
    PIR3bits.SSP2IF = 0;
    return SSP2BUF;
}

void spi2_exchangeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data = spi2_exchangeByte(*data );
        data++;
    }
}

// Half Duplex SPI Functions
void spi2_writeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        spi2_exchangeByte(*data++);
    }
}

void spi2_readBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data++ = spi2_exchangeByte(0);
    }
}

void spi2_writeByte(uint8_t byte){
    SSP2BUF = byte;
}

uint8_t spi2_readByte(void){
    return SSP2BUF;
}

/**
 * Interrupt from MSSP on bit 8 received and SR moved to buffer
 * If interrupts are not being used, then call this method from the main while(1) loop
 */
void spi2_isr(void){
    if(PIR3bits.SSP2IF == 1){
        if(spi2_interruptHandler){
            spi2_interruptHandler();
        }
        PIR3bits.SSP2IF = 0;
    }
}

void spi2_setSpiISR(void(*handler)(void)){
    spi2_interruptHandler = handler;
}