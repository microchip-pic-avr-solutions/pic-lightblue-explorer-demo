/**
 * \file rn487x_interface.c
 * \brief This file provides and interface between the RN487X and the hardware.
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

#include <string.h>
#include "rn487x_interface.h"
#include "../mcc.h"
#include "../drivers/uart.h"

static bool connected = false; //**< RN487X connection state */

/**
 * \ingroup RN487X_INTERFACE
 * \brief Initiate Hardware Reset of RN487X
 * 
 * This API is used to issue a hardware Reset to the RN487X device.
 *
 * 
 * \param value true - Reset State false - Normal State
 * \return Nothing
 */
static void RN487X_Reset(bool value);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Notify RN487X to expect incoming data
 * 
 * This API notifies the RN487X device to expect incoming Data.
 * 
 * \param value true - RX Incoming false - No Data Incoming
 * \return Nothing
 * 
 */
static void RN487X_IndicateRx(bool value);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Set the RN487X System Configuration Mode
 * 
 * This API sets the System Configuration Mode to Either Normal application Mode 
 * or Test Mode/Flash Update/EEPROM configuration
 * 
 * \param mode APPLICATION_MODE - Normal Application Mode TEST_MODE - Test Mode/Flash Update/EEPROM Configuration
 * \return Nothing
 */
static void RN487X_SetSystemMode(RN487X_SYSTEM_MODES_t mode);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Handle RN487X Status Message
 * 
 * This API is used to handle incoming status messages.
 * It prints all status messages, If DISCONNECT or STREAM_OPEN is received it manages
 * the state of bool connected.
 * 
 * \param message Passed status message
 * \return Nothing
 */
static void RN487X_MessageHandler(char* message);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Write API transmitting to RN487X module
 * 
 * This API is used to send data bytes to the RN487X module
 *
 * HINT: This API is in place to give compile time memory allocation.
 *       Functionality exist locally within file.
 *       Use of IN LINE to prevent additional stack depth requirement. 
 *       APIs can be injected in place if suitable to save (1) stack depth level
 * 
 * \param txData - data byte to send
 * \return Nothing
 */
static inline void RN487X_Write(uint8_t txData);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Read API to capture data bytes from RN487X module
 * 
 * This API is used to receive data bytes to the RN487X module
 *
 * HINT: This API is in place to give compile time memory allocation.
 *       Functionality exist locally within file.
 *       Use of IN LINE to prevent additional stack depth requirement. 
 *       APIs can be injected in place if suitable to save (1) stack depth level
 * 
 * \param N/A
 * \return uint8_t readDataByte - Byte captured from RN487X module
 */
static inline uint8_t RN487X_Read(void);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Returns if Write to RN487X module was completed
 * 
 * This API is used to receive status of communication with RN487X module
 *
 * HINT: This API is in place to give compile time memory allocation.
 *       Functionality exist locally within file.
 *       Use of IN LINE to prevent additional stack depth requirement. 
 *       APIs can be injected in place if suitable to save (1) stack depth level
 * 
 * \param N/A
 * \return bool status - RN487X is ready for another data byte
 */
static inline bool RN487X_is_tx_done(void);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Returns if Read from RN487X module is ready
 * 
 * This API is used to receive status of communication with RN487X module
 *
 * HINT: This API is in place to give compile time memory allocation.
 *       Functionality exist locally within file.
 *       Use of IN LINE to prevent additional stack depth requirement. 
 *       APIs can be injected in place if suitable to save (1) stack depth level
 * 
 * \param N/A
 * \return bool status - RN487X is ready to provide another byte
 */
static inline bool RN487X_is_rx_ready(void);

/**
 * \ingroup RN487X_INTERFACE
 * \brief Returns if Read from RN487X module is ready
 * 
 * This API is used to receive status of communication with RN487X module
 *
 * HINT: This API is in place to give compile time memory allocation.
 *       Functionality exist locally within file.
 *       Use of IN LINE to prevent additional stack depth requirement. 
 *       APIs can be injected in place if suitable to save (1) stack depth level
 * 
 * \param N/A
 * \return bool status - RN487X is ready to provide another byte
 */
static inline void RN487X_Delay(uint16_t delayCount);


/*****************************************************
*   Driver Instance Declaration(s) API(s)
******************************************************/  

const iRN487X_FunctionPtrs_t RN487X = {
    .Write = RN487X_Write,
    .Read = RN487X_Read,
    .TransmitDone = RN487X_is_tx_done,
    .DataReady = RN487X_is_rx_ready,
    .IndicateRx = RN487X_IndicateRx,
    .ResetModule = RN487X_Reset,
    .SetSystemMode = RN487X_SetSystemMode,
    .DelayMs = RN487X_Delay,
    .AsyncHandler = RN487X_MessageHandler
};

/*****************************************************
*   Driver Public API
******************************************************/  

bool RN487X_IsConnected(void)
{
    return connected;
}

/*****************************************************
*   Driver Implementation Private API(s)
******************************************************/  

static inline void RN487X_Write(uint8_t txData)
{
    uart[UART_BLE].Write(txData);
}

static inline uint8_t RN487X_Read(void)
{
    return uart[UART_BLE].Read();
}

static inline bool RN487X_is_tx_done(void)
{
    return uart[UART_BLE].TransmitDone();
}

static inline bool RN487X_is_rx_ready(void)
{
    return uart[UART_BLE].DataReady();
}

static inline void RN487X_Delay(uint16_t delayCount)
{
    return DELAY_milliseconds(delayCount);
}

static void RN487X_Reset(bool value)
{
    if (true == value)
    {
        BT_RST_SetLow();
    }
    else
    {
        BT_RST_SetHigh();
    }
}

static void RN487X_IndicateRx(bool value)
{
    if (true == value)
    {
        BT_RX_IND_SetLow();
    }
    else
    {
        BT_RX_IND_SetHigh();
    }
}

static void RN487X_SetSystemMode(RN487X_SYSTEM_MODES_t mode)
{
    // Set as Output; Will only exist if a switch is tied to RN487X and Microcontroller
    BT_MODE_SetDigitalOutput(); 

    if (APPLICATION_MODE == mode)
    {
        BT_MODE_SetHigh();
    }
    else
    {
        BT_MODE_SetLow();
    }

    // Set as Input; Will only exist if a switch is tied to RN487X and Microcontroller
    BT_MODE_SetDigitalInput();
}


static void RN487X_MessageHandler(char* message)
{
    if (strstr(message, "DISCONNECT"))
    {
        connected = false;
    }
    else if (strstr(message, "STREAM_OPEN"))
    {
        connected = true;
    }
    else
    {
        // Left Intentionally Blank: For General Messages
    }
}
