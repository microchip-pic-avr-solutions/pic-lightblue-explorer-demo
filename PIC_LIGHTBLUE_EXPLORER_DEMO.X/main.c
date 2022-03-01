// Main File
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/application/LIGHTBLUE_service.h"
#include "mcc_generated_files/rn487x/rn487x_interface.h"
#include "mcc_generated_files/rn487x/rn487x.h"
#include "mcc_generated_files/drivers/uart.h"

/** MACRO used to reference Periodic Timer overflow flag Set. 
 *  This is used by the application to have a semi-accurate 
 *  periodic task execution rate. 
 *  Strict adherence to time interval is not required.
 */
#define TIMER_FLAG_SET()                (TMR0_HasOverflowOccured())
/** MACRO used to reset the Periodic Timer overflow flag.
 *  This is used by the application to reload the semi-accurate
 *  periodic task execution.
 *  The rate allows for a (100%) drift prior to error
 *  Is susceptible to effect by extended BLE communication. 
 */
#define RESET_TIMER_INTERRUPT_FLAG      (PIR0bits.TMR0IF = 0)
/** MACRO used to configure the application used buffer sizes.
 *  This is used by the application for communication buffers.
 */
#define MAX_BUFFER_SIZE                 (80)

static char statusBuffer[MAX_BUFFER_SIZE]; /**< Status Buffer instance passed to RN487X drive used for Asynchronous Message Handling (see *asyncBuffer in rn487x.c) */
static char lightBlueSerial[MAX_BUFFER_SIZE]; /**< Message Buffer used for CDC Serial communication when connected. Terminated by \r, \n, MAX character Passes messages to BLE for transmisison. */
static uint8_t serialIndex; /**< Local index value for serial communication buffer. */
uint8_t value;
bool init = false;
uint8_t *message;

/*
                         Main application
 */
int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    RN487X_SetAsyncMessageHandler(statusBuffer, sizeof (statusBuffer));

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    RN487X_Init();
    LIGHTBLUE_Initialize();

    serialIndex = 0;


    while (1) {
        if (RN487X_IsConnected() == true) {
            if (init == false) {
                init = true;
                message = "Hello";
                while (*message) {
                    value= *message++;
                    RN487X.Write(value);
                }
                value='\n';
                RN487X.Write(value);
            }
        }
        if (TIMER_FLAG_SET() == true) {
            RESET_TIMER_INTERRUPT_FLAG;
            
                                        LIGHTBLUE_TemperatureSensor();
                                        LIGHTBLUE_AccelSensor();
                                        LIGHTBLUE_PushButton();
                                        LIGHTBLUE_LedState();
                                        LIGHTBLUE_SendProtocolVersion();
        }
        //            else
        //            {
        //                while (RN487X_DataReady())
        //                {
        //                    LIGHTBLUE_ParseIncomingPacket(RN487X_Read());
        //                }
        //                while (uart[UART_CDC].DataReady())
        //                {
        //                    lightBlueSerial[serialIndex] = uart[UART_CDC].Read();
        //                    if ((lightBlueSerial[serialIndex] == '\r')
        //                        || (lightBlueSerial[serialIndex] == '\n')
        //                        || (serialIndex == (sizeof(lightBlueSerial) - 1)))
        //                    {
        //                        lightBlueSerial[serialIndex] = '\0';
        //                        LIGHTBLUE_SendSerialData(lightBlueSerial);
        //                        serialIndex = 0;
        //                    }
        //                    else
        //                    {
        //                        serialIndex++;
        //                    }
        //                }
        //                
        //            }
        //        } else {
        while (RN487X_DataReady()) {
            value = RN487X_Read();
            lightBlueSerial[serialIndex++] = value;
            uart[UART_CDC].Write(value);
            //            RN487X.Write(value);
        }
        while (uart[UART_CDC].DataReady()) {
            value = uart[UART_CDC].Read();
            RN487X.Write(value);
        }
    }
    //    }
    return 0;
}