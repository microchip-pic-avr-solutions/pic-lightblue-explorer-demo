/**
\file
\addtogroup doc_driver_uart_code
\brief This file contains the function names of the operations that can be carried out by the UART Foundation Services .
\copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
\page License
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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
**/

/**
  Section: Included Files
 */

#include "uart.h"   


const uart_functions_t uart[] = {   
    {EUSART1_Read, EUSART1_Write, EUSART1_is_tx_ready, EUSART1_is_rx_ready, EUSART1_SetTxInterruptHandler, EUSART1_Receive_ISR, EUSART1_SetRxInterruptHandler, EUSART1_is_tx_done, EUSART1_Transmit_ISR, EUSART1_Initialize },
    {EUSART2_Read, EUSART2_Write, EUSART2_is_tx_ready, EUSART2_is_rx_ready, EUSART2_SetTxInterruptHandler, EUSART2_Receive_ISR, EUSART2_SetRxInterruptHandler, EUSART2_is_tx_done, EUSART2_Transmit_ISR, EUSART2_Initialize }
};

/**
 End of File
 */
