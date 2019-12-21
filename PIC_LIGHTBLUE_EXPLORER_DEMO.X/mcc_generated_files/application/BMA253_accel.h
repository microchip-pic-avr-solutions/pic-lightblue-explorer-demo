/** \file   BMA253_accel.h
 *  \brief  This file contains APIs to access features supported by the BMA253
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
 
#ifndef BMA253_ACCEL_H
#define	BMA253_ACCEL_H

#include <stdint.h>
#include <stdbool.h>

/**
\ingroup BMA253
\def BMA253_ADDR
This is a macro for BMA253 I2C ADDRESS
*/
#define BMA253_ADDR                         (0x19)

/* DATA REGISTER ADDRESSES */

/**
\ingroup BMA253
\def BMA253_CHIP_ID_ADDR
This is a macro to access the BMA253 CHIP ID register
*/
#define BMA253_CHIP_ID_ADDR                 (0x00)
/**
\ingroup BMA253
\def BMA253_X_LSB_ADDR
This is a macro to access the BMA253 X-Axis Low (4bit) Data value register
*/
#define BMA253_X_LSB_ADDR                   (0x02)
/**
\ingroup BMA253
\def BMA253_X_MSB_ADDR
This is a macro to access the BMA253 X-Axis High (8bit) Data value register
*/
#define BMA253_X_MSB_ADDR                   (0x03)
/**
\ingroup BMA253
\def BMA253_Y_LSB_ADDR
This is a macro to access the BMA253 Y-Axis Low (4bit) Data value register
*/
#define BMA253_Y_LSB_ADDR                   (0x04)
/**
\ingroup BMA253
\def BMA253_Y_MSB_ADDR
This is a macro to access the BMA253 Y-Axis High (8bit) Data value register
*/
#define BMA253_Y_MSB_ADDR                   (0x05)
/**
\ingroup BMA253
\def BMA253_Z_LSB_ADDR
This is a macro to access the BMA253 Z-Axis Low (4bit) Data value register
*/
#define BMA253_Z_LSB_ADDR                   (0x06)
/**
\ingroup BMA253
\def BMA253_Z_MSB_ADDR
This is a macro to access the BMA253 Z-Axis High (8bit) Data value register
*/
#define BMA253_Z_MSB_ADDR                   (0x07)
/**
\ingroup BMA253
\def BMA253_BW_SELECT_ADDR
This is a macro to access the BMA253 Bandwidth Selection configuration register
*/
#define BMA253_BW_SELECT_ADDR               (0x10)
/**
\ingroup BMA253
\def BMA253_PWR_MODE_ADDR
This is a macro to access the BMA253 Power Mode configuration register
*/
#define BMA253_PWR_MODE_ADDR                (0x11)
/**
\ingroup BMA253
\def BMA253_RANGE_ADDR
This is a macro to access the BMA253 sensitivity range configuration register
*/
#define BMA253_RANGE_ADDR                   (0x0F)

/* INTERRUPT REGISTER ADRESSES */

/**
\ingroup BMA253
\def BMA253_INT_EN_1_ADDR
This is a macro to access the BMA253 Interrupt Enable (1) bit
bit 4 [data ready interrupt] '0'-> disabled '1'-> enabled
*/
#define BMA253_INT_EN_1_ADDR                (0x17) 
/**
\ingroup BMA253
\def BMA253_INT1_PIN_ADDR
This is a macro to access the BMA253 Interrupt Pin Configuration
*/
#define BMA253_INT1_PIN_ADDR                (0x19)
/**
\ingroup BMA253
\def BMA253_INT_DATA_ADDR
This is a macro to access the BMA253 Interrupt Data bit
bit 0 [data ready to int1 pin] '0'-> disabled '1'-> enabled
*/
#define BMA253_INT_DATA_ADDR                (0x1A)

/* POWER MODE DEFINITONS */

/**
\ingroup BMA253
\def BMA253_NORMAL_MODE
This is a macro is used to configure the BMA253 power mode for the application
default power mode
*/
#define BMA253_NORMAL_MODE                  (0x00)

/* BANDWITH SETTINGS */

/**
\ingroup BMA253
\def BMA253_BW_7_81HZ
This is a macro is used to configure the BMA253 Bandwidth to 7.81 Hz
*/
#define BMA253_BW_7_81HZ                    (0x08)
/**
\ingroup BMA253
\def BMA253_BW_15_63HZ
This is a macro is used to configure the BMA253 Bandwidth to 15.63 Hz
*/
#define BMA253_BW_15_63HZ                   (0x09)
/**
\ingroup BMA253
\def BMA253_BW_31_25HZ
This is a macro is used to configure the BMA253 Bandwidth to 31.25 Hz
*/
#define BMA253_BW_31_25HZ                   (0x0A)
/**
\ingroup BMA253
\def BMA253_BW_62_5HZ
This is a macro is used to configure the BMA253 Bandwidth to 62.5 Hz
*/
#define BMA253_BW_62_5HZ                    (0x0B)
/**
\ingroup BMA253
\def BMA253_BW_125HZ
This is a macro is used to configure the BMA253 Bandwidth to 125 Hz
*/
#define BMA253_BW_125HZ                     (0x0C)
/**
\ingroup BMA253
\def BMA253_BW_250HZ
This is a macro is used to configure the BMA253 Bandwidth to 250 Hz
*/
#define BMA253_BW_250HZ                     (0x0D)
/**
\ingroup BMA253
\def BMA253_BW_500HZ
This is a macro is used to configure the BMA253 Bandwidth to 500 Hz
*/
#define BMA253_BW_500HZ                     (0x0E)
/**
\ingroup BMA253
\def BMA253_BW_1KHZ
This is a macro is used to configure the BMA253 Bandwidth to 1000 Hz
*/
#define BMA253_BW_1KHZ                      (0x0F)

/* ACCEL G-RANGE SETTINGS */

/**
\ingroup BMA253
\def BMA253_2G_RANGE
This is a macro is used to configure the BMA253 sensitivity for 2 G's
default range| bottom (3) bits are 'cut'
*/
#define BMA253_2G_RANGE                     (0x03) // default range
/**
\ingroup BMA253
\def BMA253_4G_RANGE
This is a macro is used to configure the BMA253 sensitivity for 4 G's
bottom (2) bits are 'cut'
*/
#define BMA253_4G_RANGE                     (0x05)
/**
\ingroup BMA253
\def BMA253_8G_RANGE
This is a macro is used to configure the BMA253 sensitivity for 8 G's
bottom (1) bits are 'cut'
*/
#define BMA253_8G_RANGE                     (0x08)
/**
\ingroup BMA253
\def BMA253_16G_RANGE
This is a macro is used to configure the BMA253 sensitivity for 16 G's
(12)-bits considered | (0) bits cut
*/
#define BMA253_16G_RANGE                    (0x0C)

/**
*! \struct BMA253_ACCEL_DATA_t
* A struct used to store the full (48-bit) accelerometer data values 
for x, y, z axis
*/
typedef struct  
{
    int16_t x;
    int16_t y;
    int16_t z;
}BMA253_ACCEL_DATA_t;

/**
 \ingroup BMA253
 \brief This public function is used to initialize the BMA253 according to the demonstration
 requirements. \n
 Normal Mode of operation, using 7.81 Hz bandwidth supporting 2 G's measurement. 
 \return null \n 
 */
void BMA253_Initialize(void);
/**
 \ingroup BMA253
 \brief  This is the public function called to capture the Signed 
 X-Axis Acceleration Value \n
 \param[in] xAccelData - pointer to singed 16bit variable used to return X Accel Data value. \n
 \return int16_t* data value loaded into passed pointer \n 
 \retval void \n
 */
void BMA253_GetAccelDataX(int16_t *xAccelData);
/**
 \ingroup BMA253
 \brief  This is the public function called to capture the Signed 
 Y-Axis Acceleration Value \n
 \param[in] yAccelData - pointer to singed 16bit variable used to return Y Accel Data value. \n
 \return int16_t* data value loaded into passed pointer \n 
 \retval void \n
 */
void BMA253_GetAccelDataY(int16_t *yAccelData);
/**
 \ingroup BMA253
 \brief  This is the public function called to capture the Signed 
 Z-Axis Acceleration Value \n
 \param[in] zAccelData - pointer to singed 16bit variable used to return Z Accel Data value. \n
 \return int16_t* data value loaded into passed pointer \n 
 \retval void \n
 */
void BMA253_GetAccelDataZ(int16_t *zAccelData);
/**
 \ingroup BMA253
 \brief  This is the public function called to capture the Signed 
 Z-Axis Acceleration Value \n
 \param[in] BMA253_ACCEL_DATA_t - *accelData; (48-bit structure used for X,Y,Z Data)
 This passed parameter is used to pass updated values back up through function call using
 the *pointer reference to the object variable \n
 \return *accelData - through passed pointer reference \n 
 \retval void \n
 */
void BMA253_GetAccelDataXYZ(BMA253_ACCEL_DATA_t *accelData);
/**
 \ingroup BMA253
 \brief  This is the public function used to retrieve BMA chip id value \n
 \return Response from the BMA chip id register \n 
 \retval unsigned 8 bit value used to represent BMA Chip ID value \n
 */
uint8_t BMA253_GetAccelChipId(void);

#endif	/* BMA253_ACCEL_H */

