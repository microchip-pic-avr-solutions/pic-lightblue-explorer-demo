/** \file   mcp9844_temp_sensor.c
 *  \brief  This file contains APIs to access features supported by the MCP9844
 */
/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries.

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

#include "MCP9844_temp_sensor.h"
#include "drivers/i2c_simple_master.h"

/* MASK DEFINITIONS */

/**
\ingroup MCP9844
\def CLEAR_FLAG_BITS_MASK
This is a macro is used as a mask to CLEAR the FLAG bits during
temperature conversion capture. 
*/
#define CLEAR_FLAG_BITS_MASK                (0x1F)
/**
\ingroup MCP9844
\def MSB_MASK
This is a macro for masking access to only the MSB values returned
by the ambient temperature register.
*/
#define MSB_MASK                            (0xFF00)

/**
 \ingroup MCP9844
 \brief \n
 This function is private to the source file. It is called to process the task
 to retrieve the ambient temperature from the MCP9844 device. It then formats
 the returned value into a signed 16-bit value returned via the 
 \ref MCP9844_GetManufacture public function. 
 \param[in] temperature - variable used to store, and return read temperature
 \return Signed temperature value representing celsius value measured by MCP9844\n 
 \retval int16_t temperatureData, Formated representation of ambient temperature
 */
static int16_t MCP9844_CalcTemperature(void);

uint16_t MCP9844_GetManufacture(void)
{
    return i2c_read2ByteRegister(MCP9844_ADDR, MCP9844_MANUFACTURER_REG);
}

uint16_t MCP9844_GetDevice(void)
{
    return i2c_read2ByteRegister(MCP9844_ADDR, MCP9844_DEVICE_REG);
}

void MCP9844_GetTemperatureValue(int16_t *temperature)
{
    *temperature = MCP9844_CalcTemperature();
}

static int16_t MCP9844_CalcTemperature(void)
{
    int16_t temperatureData;
    uint8_t upperByte;
    uint8_t lowerByte;
    
    temperatureData = i2c_read2ByteRegister(MCP9844_ADDR, MCP9844_TEMP_AMBIENT_REG);
    upperByte = ((temperatureData & MSB_MASK) >> 8) & CLEAR_FLAG_BITS_MASK;
    lowerByte = (uint8_t)temperatureData;
    
    temperatureData = ((int16_t)(upperByte << 8) | lowerByte);
    
    return temperatureData;
}