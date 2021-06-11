/**
 * \file rn487x_example.c
 * \brief This file contains the APIs required to communicate with the RN487X driver library to 
 *        create, and open a basic Transparent EUSART demonstration.
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

#include <stdbool.h>
#include "rn487x_example.h"
#include "../drivers/uart.h"
#include "../rn487x/rn487x_interface.h"
#include "../rn487x/rn487x.h"
#include "../../mcc_generated_files/interrupt_manager.h"

/** MACRO used to configure the application used buffer sizes.
 *  This is used by the application for communication buffers.
 */
#define MAX_BUFFER_SIZE                 (80)

/**< Status Buffer instance passed to RN487X drive used for Asynchronous Message Handling (see *asyncBuffer in rn487x.c) */
static char statusBuffer[MAX_BUFFER_SIZE];    

/**
 * \ingroup RN487X_Example
 * \brief Example Implmentation of Transparent UART
 *        This can be connected to a Smart BLE 'Terminal' 
 *        application for simple data exchange demostration.
 *
 * For more details, refer Chapter 4.2 in RN487X user guide.
 * \return Connected Status
 * \retval true - Connected.
 * \retval false - Not Connected
 */  
static bool RN487X_Example_TransparentUart(void);
/**
 * \ingroup RN487X_Example_Run
 * \brief This 'Runs' the example applications While(1) loop
 *
 * For more details, refer Chapter 4.2 in RN487X user guide.
 * \param none
 * \return void
 */  
static void RN487X_Example_Run(void);

bool RN487X_Example_Initialized(void)
{
    bool exampleIsInitialized = false;
    
    exampleIsInitialized = RN487X_SetAsyncMessageHandler(statusBuffer, sizeof(statusBuffer));
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    if (exampleIsInitialized == true)
    {
        RN487X_Example_Run();
    }
    return (false);     // ^ Held if Successful; Return failure if reaching this.
}

static void RN487X_Example_Run(void)
{
    while(1)
    {
        if (true == RN487X_Example_TransparentUart())
        {
            // Connected
        }
        else
        {
            // Not Connected
        }
    }
}

static bool RN487X_Example_TransparentUart(void)
{
   bool isConnected;
   isConnected = RN487X_IsConnected();

   if (true == isConnected)
   {
       while (RN487X_DataReady())
       {
           uart[UART_CDC].Write(RN487X_Read());
       }
       while (uart[UART_CDC].DataReady())
       {
           RN487X.Write(uart[UART_CDC].Read());
       }
   }
   else
    {
        while(RN487X_DataReady())
        {
            uart[UART_CDC].Write(RN487X_Read());
        }
        while (uart[UART_CDC].DataReady())
        {
            RN487X.Write(uart[UART_CDC].Read());
        }
    }

    return isConnected;
}
