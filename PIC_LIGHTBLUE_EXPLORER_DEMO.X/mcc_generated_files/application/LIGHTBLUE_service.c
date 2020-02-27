/** \file   LIGHTBLUE_service.c
 *  \brief  This file contains APIs to access features for the Light Blue Explorer Example
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

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "LIGHTBLUE_service.h"

// APP Dependencies
#include "../rn4870-1-ble-module/rn487x.h"
#include "BMA253_accel.h"
#include "MCP9844_temp_sensor.h"
#include "../pin_manager.h"
#include "../drivers/uart.h"

/**
\ingroup LIGHTBLUE
\def Hex(x) 
 * Macro used to MASK upper Nibble and extract HEX value of lower Nibble
 \param[in] x - 8bit unsigned char \n
 \return MASKED (4) bit value \n
 \retval MASKED lower Nibble Character \n
*/
#define Hex(x)                      (_hex[(x) & 0xF])
/**
\ingroup LIGHTBLUE
\def Ascii2Decimal(c) 
 * Macro used to MASK upper Nibble and extract HEX value of lower Nibble
 \param[in] x - 8bit unsigned char \n
 \return 8bit decimal value converted from 'char' ANCI Byte \n
 \retval decimal value of ancii character \n
*/
#define Ascii2Decimal(c)            (((c) <= '9') ? (c) - '0' : (c & 0x5f) - 'A' + 10)
/**
\ingroup LIGHTBLUE
\def DataLedOn() 
 * Macro used to turn ON the DATA LED. This will configured the connected pin
 * to be driven in the direction which turns the LED ON.
 * This LED is ACTIVE_LOW \n
 \return void \n
*/
#define DataLedOn()                 DATA_LED_SetLow()
/**
\ingroup LIGHTBLUE
\def DataLedOff() 
 * Macro used to turn OFF the DATA LED. This will configured the connected pin
 * to be driven in the direction which turns the LED OFF.
 * This LED is ACTIVE_LOW \n
 \return void \n
*/
#define DataLedOff()                DATA_LED_SetHigh()
/**
\ingroup LIGHTBLUE
\def START_BYTE
 * Macro used to add the character used as the Starting BYTE used for framing the
 LightBlue Application Packets. \n
 \return void \n
*/
#define START_BYTE                  ('[')
/**
\ingroup LIGHTBLUE
\def TERMINATION_BYTE
 * Macro used to add the character used as the Terminating BYTE used for framing the
 LightBlue Application Packets. \n
 \return void \n
*/
#define TERMINATION_BYTE            (']')
/**
\ingroup LIGHTBLUE
\def ERROR_LED_VALUE
 * Macro used to access the representation of the ERROR LED State.
 * On the PIC this is through the GPIO bitMap exchanged with the RN487X module.
 * On the AVR this is connected to a pin through the MCU device. \n
 \return void \n
*/
#define ERROR_LED_VALUE             (bitMap.ioStateBitMap.p2_2_state)
/**
\ingroup LIGHTBLUE
\def CMD_MODE_DELAY_TIME
 * Macro used to configure the MCUs BLOCKING delay length (mS) between MCU and
 * RN487X module communication. This delay is required for processing of some
 * requested actions. \n
 \return void \n
*/
#define CMD_MODE_DELAY_TIME         (20)
/**
\ingroup LIGHTBLUE
\def LED_OFF_STATE
 * Macro used to indicate the Board Specific Package (BSP) setting for the LED
 * bias configuration, determined on the ACTIVE_STATE tied High/Low. 
 * On this board, the LED is ACTIVE LOW, so HIGH will be the OFF_STATE \n
 \return void \n
*/
#define LED_OFF_STATE               (0x01)
/**
\ingroup LIGHTBLUE
\def NOT_PRESSED_STATE
 * Macro used to indicate the Board Specific Package (BSP) setting for the BUTTON
 * bias configuration, determined on the ACTIVE_STATE pull up/down.
 * On this board, the BUTTON (switch) is ACTIVE LOW, so HIGH will be the NOT PRESSED \n
 \return void \n
*/
#define NOT_PRESSED_STATE           (0x01)
/**
\ingroup LIGHTBLUE
\def LIGHTBLUE_OFF
 * Macro used to MASK the value of the LED.
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
*/
#define LIGHTBLUE_OFF               (0x00)
/**
\ingroup LIGHTBLUE
\def DATA_LED_IDENTIFIER
 * Macro used to MASK the value of the DATA LED.
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
*/
#define DATA_LED_IDENTIFIER         (0x00)
/**
\ingroup LIGHTBLUE
\def ERROR_LED_IDENTIFIER
 * Macro used to MASK the value of the ERROR LED. 
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
*/
#define ERROR_LED_IDENTIFIER        (0x10)
/**
\ingroup LIGHTBLUE
\def NIBBLE_MASK
 * Macro used to MASK the value of the LEDs as they are being toggle. 
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
*/
#define NIBBLE_MASK                 (0x01)
/**
 \ingroup LIGHTBLUE
*! \struct PROTOCOL_PACKET_TYPES_t
* A struct used to store possible Light Blue Protocol Packet ID types.
Specific processing of task coorilated to packet type handled in \ref 
LIGHTBLUE_PerformAction private function. Use of types is handled in packet
format functions used to specific application transmission features. 
*/
typedef enum
{
    PROTOCOL_VERSION_ID    = 'V',
    LED_STATE_ID           = 'L',
    BUTTON_STATE_ID        = 'P',
    TEMPERATURE_DATA_ID    = 'T',
    ACCEL_DATA_ID          = 'X',
    SERIAL_DATA_ID         = 'S',
    ERROR_ID               = 'R',
    UI_CONFIG_DATA_ID      = 'U'
}PROTOCOL_PACKET_TYPES_t;
/**
 \ingroup LIGHTBLUE
*! \struct PACKET_PARSER_STATE_t
* A struct used to manage the Packet Processor State upon reception of a framed
message from the RN487X device. These states are used to extract packet data in
the \ref LIGHTBLUE_PerformAction task processing function. 
*/
typedef enum
{
    IDLE                    = 0,
    SEQUENCE_NUMBER         = 1,
    PACKET_ID               = 2,
    PAYLOAD_SIZE_0          = 3,
    PAYLOAD_SIZE_1          = 4,
    PAYLOAD_0               = 5,
    PAYLOAD_1               = 6        
           
}PACKET_PARSER_STATE_t;

const char * const protocol_version_number = "1.0.0";   /**< Local Const Variable used to represent Light Blue Protocol version used by application */
static char _hex[] = "0123456789ABCDEF";                /**< Local Variable used for Masking a Hex value result */
static uint8_t sequenceNumber = 0;                      /**< Local Variable used to keep track of the number of TRANSMIT packets sent from device*/
static volatile rn487x_gpio_bitmap_t bitMap;            /**< Local Variable used managing stored state of GPIO pin controlled by RN487X */

/**
 \ingroup LIGHTBLUE
 \brief  Private function to send a properly formated Light Blue Application Packet  \n
This function is used to properly format the Light Blue Packet into the desire format.
Format is: [0]    [1]     [2]  [ 3 + 4 ]    [5 -> nSize]  [n+5]
            [    SeqID   Type  Payload Size    Payload       ]
   [0]: '['              : Start Bracket Character
   [1]: '#'              : (0-F) Hex value used for Sequence ID used to maintain track of Rx/Tx Order
   [2]: 'Type ID'        : (V,L,P,T,X,S,R,U) Char used to Identify Data/Packet Type
 [3+4]: 'Payload Size'   : Size of Data Payload expected starting with Byte 4 --> N
[5->n]: 'Payload'        : Data of Payload
 [n+5]: ']'              : End Bracket Character
 \return void \n
 */
static void LIGHTBLUE_SendPacket(char packetID, char* payload);
/**
 \ingroup LIGHTBLUE
 \brief  Private function to split a Word (16bit) value into (2) (8bit) Byte values.  \n
This function is used break a (4) digit, 16bit value into pieces and load it back into
the payload as a 'char' representation of the number presented as a hex value. This is later
broken down into Nibble format through the SplitByte code. 
 \return void \n
 */
static void LIGHTBLUE_SplitWord(char* payload, int16_t value);
/**
 \ingroup LIGHTBLUE
 \brief  Private function to split a Byte (8bit) value into (2) (4bit) Nibble values.  \n
This function is used break a (2) digit, 8bit value into pieces and load it back into
the payload as a 'char' representation of the number presented as a hex value. 
 \return void \n
 */
static void LIGHTBLUE_SplitByte(char* payload, int8_t value);
/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request status of PUSH BUTTON reading the PORT value. \n
This function is used to read PORT value read from pin connected to PUSH BUTTON masked against NOT_PRESSED state.
 \return uint8_t BUTTON STATUS - Status 0 (Pressed) | 1 (Release) \n
 \retval Status 0 (OFF) | 1 (ON) \n
 */
static uint8_t LIGHTBLUE_GetButtonValue(void);

/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request status of DATA LED from reading the PORT value. \n
This function is used to read PORT value from pin connected to LED masked against OFF state.
 \return uint8_t DATA LED - Status 0 (OFF) | 1 (ON) \n
 \retval Status 0 (OFF) | 1 (ON) \n
 */
static uint8_t LIGHTBLUE_GetDataLedValue(void);

/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request status of ERROR LED from local maintain variable. \n
This function is used to return ERROR LED status as the END-DEVICE knows it masked against OFF state.
 \return uint8_t ERROR LED - Status 0 (OFF) | 1 (ON) \n
 \retval Status 0 (OFF) | 1 (ON) \n
 */
static uint8_t LIGHTBLUE_GetErrorLedValue(void);
/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request action from the RN487X module \n
This function is used to enter device into Command Mode, Update GPIO State, and return to Data Mode.
 \param[in] value - bool Set ERROR LED State as ON  \n
 \return void \n
 */
static void LIGHTBLUE_SetErrorLedValue(bool value);
/**
 \ingroup LIGHTBLUE
 \brief  Private function used process requested actions from the Light Blue application \n
This function is used to update the state of the ERROR LED which is controlled via the
 * RN487X through the GPIO command on the PIC platform. On the AVR board this is MCU (LAT) controlled.
 \return void \n
 */
static void LIGHTBLUE_UpdateErrorLed(void); 
/**
 \ingroup LIGHTBLUE
 \brief  Private function used process requested actions from the Light Blue application \n
This function inspect the Packet Type ID to determine coordinated action.
The responsibilities include LED State Control, and Serial Message Bridge.
For Serial Message Bridge, the string 'message' is iterated through byte by byte.
Supported Type ID Options:
'L' : LEDs Status Update
'S' : Serial Data to be bridged through CDC
 \param[in] id - char representing the Type ID ('L' || 'S')  \n
 \param[in] data - 8bit unsigned value; (- - - - - - ERR DATA), (Single Char from String)\n
 \return void \n
 */
static void LIGHTBLUE_PerformAction(char id, uint8_t data);

void LIGHTBLUE_Initialize(void)
{
    bitMap.ioBitMap.gpioBitMap = 0x01;
    bitMap.ioStateBitMap.gpioStateBitMap = 0x01; 
}

void LIGHTBLUE_TemperatureSensor(void)
{
    char payload[5];
    int16_t temperature;
    
    *payload = '\0';
    MCP9844_GetTemperatureValue(&temperature);
    
    LIGHTBLUE_SplitWord(payload, temperature);
    
    LIGHTBLUE_SendPacket(TEMPERATURE_DATA_ID, payload);
}

void LIGHTBLUE_AccelSensor(void)
{
    char payload[13];
    BMA253_ACCEL_DATA_t accelData;
    
    *payload = '\0';
    BMA253_GetAccelDataXYZ(&accelData);
    // Masking to ensure top nibble is always 0 as light blue expects
    // Exception may occur when highest byte is not 0
    LIGHTBLUE_SplitWord(payload, (accelData.x & 0x0FFF)); 
    LIGHTBLUE_SplitWord(payload, (accelData.y & 0x0FFF));
    LIGHTBLUE_SplitWord(payload, (accelData.z & 0x0FFF));
    
    LIGHTBLUE_SendPacket(ACCEL_DATA_ID, payload);
}

void LIGHTBLUE_PushButton(void)
{
    char payload[3];
    uint8_t button = LIGHTBLUE_GetButtonValue();
    
    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, button);
    
    LIGHTBLUE_SendPacket(BUTTON_STATE_ID, payload);
}

void LIGHTBLUE_LedState(void)
{
    char payload[3];
    uint8_t led;
    
    led = DATA_LED_IDENTIFIER + LIGHTBLUE_GetDataLedValue();
    
    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, led);
    
    LIGHTBLUE_SendPacket(LED_STATE_ID, payload);
    
    led = ERROR_LED_IDENTIFIER + LIGHTBLUE_GetErrorLedValue();
    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, led);
    
    LIGHTBLUE_SendPacket(LED_STATE_ID, payload); 
}

void LIGHTBLUE_SendProtocolVersion(void)
{
    char payload[19];
    uint8_t value;
    uint8_t dataIndex;
    
    *payload = '\0';
    
    for(dataIndex = 0; dataIndex < strlen(protocol_version_number); dataIndex++)
    {
        value = protocol_version_number[dataIndex];
        LIGHTBLUE_SplitByte(payload, value);
    }
    
    LIGHTBLUE_SendPacket(PROTOCOL_VERSION_ID, payload);
}

void LIGHTBLUE_SendSerialData(char* serialData)
{
    uint8_t length = strlen(serialData) * 2;
    
    RN487X.Write(START_BYTE);
    RN487X.Write(Hex(sequenceNumber++));
    RN487X.Write(SERIAL_DATA_ID);
    RN487X.Write(Hex(length >> 4));
    RN487X.Write(Hex(length));
    while(*serialData)
    {
        RN487X.Write(Hex(*serialData >> 4));
        RN487X.Write(Hex(*serialData++));
    }
    RN487X.Write(TERMINATION_BYTE);
}

void LIGHTBLUE_ParseIncomingPacket(char receivedByte)
{
    static PACKET_PARSER_STATE_t parserState = IDLE;
    static uint8_t length = 0;
    static uint16_t data = 0;
    static char packetID = '\0';

    switch(parserState) 
    {
        case SEQUENCE_NUMBER:
            //ignore sequence
            parserState = PACKET_ID;
            break;
        case PACKET_ID:
            packetID = receivedByte;
            parserState = PAYLOAD_SIZE_0;
            break;
        case PAYLOAD_SIZE_0:
            length = Ascii2Decimal(receivedByte);
            parserState = PAYLOAD_SIZE_1;
            break;
        case PAYLOAD_SIZE_1:
            length = (length << 4) + Ascii2Decimal(receivedByte);
            parserState = PAYLOAD_0;
            break;
        case PAYLOAD_0:
            data = Ascii2Decimal(receivedByte);
            length--;
            if (length == 0)
            {
                parserState = IDLE;
            }
            else
            {
                parserState = PAYLOAD_1;
            }
            break;
        case PAYLOAD_1:
            data = (data << 4) + Ascii2Decimal(receivedByte);
            LIGHTBLUE_PerformAction(packetID, data);
            length--;
            if (length == 0)
            {
                parserState = IDLE;
            }
            else
            {
                parserState = PAYLOAD_0;
            }
            break;
        case IDLE:
        default:
            if (receivedByte == START_BYTE)
            {
                parserState = SEQUENCE_NUMBER;
            }
            break;
    }
}

static void LIGHTBLUE_SendPacket(char packetID, char* payload)
{
    RN487X.Write(START_BYTE);
    RN487X.Write(Hex(sequenceNumber++));
    RN487X.Write(packetID);
    RN487X.Write(Hex(strlen(payload) >> 4));
    RN487X.Write(Hex(strlen(payload)));
    while (*payload)
    {
        RN487X.Write((*(uint8_t *)payload++));
    }
    RN487X.Write(TERMINATION_BYTE);
}

static void LIGHTBLUE_SplitWord(char* payload, int16_t value)
{
    LIGHTBLUE_SplitByte(payload, value);
    LIGHTBLUE_SplitByte(payload, value >> 8);
}

static void LIGHTBLUE_SplitByte(char* payload, int8_t value)
{
    payload += strlen(payload);
    *payload++ = Hex(value >> 4);
    *payload++ = Hex(value);
    *payload = '\0';
}

static uint8_t LIGHTBLUE_GetButtonValue(void)
{
    return NOT_PRESSED_STATE - SW0_GetValue(); // This is forcing proper data for LightBlue
}

static uint8_t LIGHTBLUE_GetDataLedValue(void)
{
    return LED_OFF_STATE - DATA_LED_GetValue(); // This is forcing proper data for LightBlue
}

static uint8_t LIGHTBLUE_GetErrorLedValue(void)
{
    return LED_OFF_STATE - ERROR_LED_VALUE;
}

static void LIGHTBLUE_SetErrorLedValue(bool value)
{
    ERROR_LED_VALUE = LED_OFF_STATE - value;
    LIGHTBLUE_UpdateErrorLed();
}

static void LIGHTBLUE_UpdateErrorLed(void)
{
    RN487X_EnterCmdMode();
    RN487X.DelayMs(CMD_MODE_DELAY_TIME);
    RN487X_SetOutputs(bitMap);
    RN487X_EnterDataMode();
}

static void LIGHTBLUE_PerformAction(char id, uint8_t data)
{
    uint8_t led;
    
    switch(id)
    {
        case LED_STATE_ID:
            led = (data >> 4) & NIBBLE_MASK;
            if(led == DATA_LED_IDENTIFIER)
            {
                if((data & NIBBLE_MASK) == LIGHTBLUE_OFF)
                {
                    DataLedOff();
                }
                else
                {
                    DataLedOn();
                }
            }
            else
            {
                LIGHTBLUE_SetErrorLedValue(data & NIBBLE_MASK);
            }
            break;
        case SERIAL_DATA_ID:
            uart[UART_CDC].Write(data); // echo out the terminal for now
            break;
        default:
            break;
    }
}