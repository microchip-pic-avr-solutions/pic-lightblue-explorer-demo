/** \file   mcp9844_temp_sensor.h
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

#ifndef MCP9844_TEMP_SENSOR_H
#define	MCP9844_TEMP_SENSOR_H

#include <stdint.h>

/**
\ingroup MCP9844
\def MCP9844_ADDR
This is a macro for MCP9844 I2C ADDRESS
*/
#define MCP9844_ADDR                        (0x18)

/* MCP9844 Register Addresses */

/**
\ingroup MCP9844
\def MCP9844_CAPACITY_REG
This is a macro for the address for the Capacity Register (read-Only)
*/
#define MCP9844_CAPACITY_REG                (0x00)
/**
\ingroup MCP9844
\def MCP9844_CAPACITY_REG
This is a macro for the address for the Configuration Register
*/
#define MCP9844_CONFIG_REG                  (0x01)
/**
\ingroup MCP9844
\def MCP9844_CAPACITY_REG
Temperature Upper Bound Trip Limit Register
*/
#define MCP9844_TEMP_UPPER_BOUND_TRIP_REG   (0x02)
/**
\ingroup MCP9844
\def MCP9844_TEMP_LOWER_BOUND_TRIP_REG
Temperature Lower Bound Trip Limit Register
*/
#define MCP9844_TEMP_LOWER_BOUND_TRIP_REG   (0x03)
/**
\ingroup MCP9844
\def MCP9844_TEMP_CRITICAL_TRIP_REG
Temperature Critical Trip Limit Register
*/
#define MCP9844_TEMP_CRITICAL_TRIP_REG      (0x04)
/**
\ingroup MCP9844
\def MCP9844_TEMP_AMBIENT_REG
Ambient Temperature Register
*/
#define MCP9844_TEMP_AMBIENT_REG            (0x05)
/**
\ingroup MCP9844
\def MCP9844_MANUFACTURER_REG
Manufacture ID Register 
*/
#define MCP9844_MANUFACTURER_REG            (0x06)
/**
\ingroup MCP9844
\def MCP9844_DEVICE_REG
Device ID & Revision Register
*/
#define MCP9844_DEVICE_REG                  (0x07)
/**
\ingroup MCP9844
\def MCP9844_RESERVED_REG
Reserved Register
*/
#define MCP9844_RESERVED_REG                (0x08)
/**
\ingroup MCP9844
\def MCP9844_RESOLUTION_REG
Resolution (2-Bit)
*/
#define MCP9844_RESOLUTION_REG              (0x09)

/* MCP9844 Public APIs */

/**
 \ingroup MCP9844
 \brief \n
 * This API is called to request the full WORD value for the Ambient Temperature
 * returned by the MCP9844. Uses private function @ref MCP9844_CalcTemperature
 \param[in] temperature - variable used to store, and return read temperature
 \return void \n
 */
void MCP9844_GetTemperatureValue(int16_t *temperature);
/**
 \ingroup MCP9844
 \brief \n
 * This API validates request the Manufacture ID value from the MCP9844 register (0x06)
 \param[in] void
 \return 16-bit hex value representing the temperature data from the MCP9844 register \n 
 \retval \ref uint16_t - read value from MCP9844_MANUFACTURER_REG location \n 
 */
uint16_t MCP9844_GetManufacture(void);
/**
 \ingroup MCP9844
 \brief \n
 * This API validates request the Device ID value from the MCP9844 register (0x07)
 \param[in] void
 \return 16-bit hex value representing the Device ID & Revision values read from the register \n 
 \retval \ref uint16_t - read value from MCP9844_DEVICE_REG location \n 
 */
uint16_t MCP9844_GetDevice(void);

#endif	/* MCP9844_TEMP_SENSOR_H */

