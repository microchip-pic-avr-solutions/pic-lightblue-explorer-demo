/** \file   LIGHTBLUE_service.h
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

#ifndef LIGHTBLUE_SERVICE_H
#define	LIGHTBLUE_SERVICE_H

/**
 \ingroup LIGHTBLUE
 \brief  Public function used for initialization of RN487X module conditions. \n
 Application configures the GPIO state & value used for the demonstration. 
 \return void \n
 */
void LIGHTBLUE_Initialize(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Temperature Sensor data to the Light Blue application \n
 This function will request temperature data from the MCP9844, format it according to the Light Blue
 protocol requirements using 'T' for the ID, Temperature data is sent as a 16-bit hex value 
 representing the celsius value; fahrenheit is converted through the BLE application. 
 Actual Temperature value is provided as described below:
     b15        b14             b13           b12    b11    b10   b9    b8
 Ta vs TCrit  Ta vs TUpper   Ta vs TLower    Sign    2^7    2^6   2^5   2^4
     b7         b6              b5             b4     b3     b2    b1    b0 
     2^3        2^2             2^1           2^0    2^-1   2^-2  2^-3  2^-4
 \return void \n
 */
void LIGHTBLUE_TemperatureSensor(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Acceleration Sensor data to the Light Blue application \n
 This function will request X, Y, Z data from the BMA253, format it according to the Light Blue
 protocol requirements using 'A' for the ID, data is sent as a 16-bit hex values 
 representing the signed acceleration data in 12-bit form with top 4 bits cleared to 0's.
 Actual value is provided as described below:
     b15    b14     b13     b12    b11    b10   b9      b8
     0      0       0       0      Sign   1g    500mg   250mg
     b7     b6      b5      b4     b3     b2    b1      b0 
    125mg   62.5mg  31.2mg  15.6mg 7.8mg  3.9mg 1.9mg  0.97mg
 \return void \n
 */
void LIGHTBLUE_AccelSensor(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Push button state to the Light Blue application \n
 This function will read the SW0 state, format it according to the Light Blue
 protocol requirements using 'P' for the ID, data is sent as a 8-bit hex value 
 representing the button pressed state.
0 - Pressed
1- Released
 \return void \n
 */
void LIGHTBLUE_PushButton(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue LED state to the Light Blue application \n
 This function will read the (leds) state, format it according to the Light Blue
 protocol requirements using 'L' for the ID, data is sent as a 8-bit hex value 
 representing the LEDs status. DATA LED is always connected to the MCU and is read
through the LAT state, the ERR LED is controlled by the RN487X module; and requires
status request through GPIO register on the PIC board; ERR is connected to the MCU on
the AVR design. 
0 - ON
1- OFF
 \return void \n
 */
void LIGHTBLUE_LedState(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Light Blue application protocol version \n
The protocol version can be used to alter behavior, or presentation through the
 BLE application. Therefore this function is used to inform the application what
 protocol format to be expected from the end-device. The packet is formated using
 the 'V' for the ID. 
E.G. "1.0.0"
 \return void \n
 */
void LIGHTBLUE_SendProtocolVersion(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue serial messages to the Light Blue application \n
This function will receive string message via the CDC terminal when CONNECTED, 
format it according to the Light Blue protocol requirements using 'S' for the ID, 
data is sent represented as Character's. Serial Message entered through the 
terminal are sent upon reception through the message parser. No interrogation of
string or data is performed by the end-device. 
 \return void \n
 */
void LIGHTBLUE_SendSerialData(char* serialData);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to parse message received from the Light Blue application \n
This function will receive byte by byte the string message captured by the RN487X 
formated between the '[' Start and ']' End Characters it according to the Light Blue protocol. 
The ID passed will be stored into packetID and is used to determine method of
packet data capture. Based upon the requested action. The processing of requested
action is performed upon parsing. Occurs through PerformAction function.
Packet ID Options:
'L' : LEDs Status Update
'S' : Serial Data to be bridged through CDC
'R' : Error has occurred; LED action will be requested
 \return void \n
 */
void LIGHTBLUE_ParseIncomingPacket(char receivedByte);

#endif	/* LIGHTBLUE_SERVICE_H */

