# RN487X Driver Library
### RN4871 and RN4870
[Microchip Bluetooth](#https://www.microchip.com/design-centers/wireless-connectivity/bluetooth)
[RN Data Sheet](#http://ww1.microchip.com/downloads/en/DeviceDoc/RN4870-71-Bluetooth-Low-Energy-Module-Data-Sheet-DS50002489D.pdf)
[RN User Guide](#http://ww1.microchip.com/downloads/en/DeviceDoc/RN4870-71-Bluetooth-Low-Energy-Module-User-Guide-DS50002466C.pdf)

---
Devices (Device): \| **RN4870** \| **RN4871** \|

---

## Release Version
    - Version: 1.0.0
    - Change Log: 
        * 1.0.0: Initial Release Version; Supporting LightBlue(tm) Explorer Demonstration

---

## Driver Information
The purpose of the functions created within the RN487X driver library are to allow required level of support necessary to operate the RN4870/1 module for simple applications. 
Further support features will be added in future iterations to the library.

### Public In Header
```
#define RN487X_RESET_DELAY_TIME    1
#define RN487X_STARTUP_DELAY       200

bool RN487X_Init(void);
```
**Module Initialization**
This API initializes RN487x Device. Configures for APPLICATION MODE.

```
void RN487X_SendCmd(const uint8_t *cmd, uint8_t cmdLen);
```
**Send Command**
This API takes command and its length as input and sends that command to RN487x.

```
uint8_t RN487X_GetCmd(const char *getCmd, uint8_t getCmdLen, char *getCmdResp);
```
**Get Command**
This API gets the config value from RN487x by sending get command. 
For more details, refer section 2.5 GET COMMANDS in RN4870-71 user guide.

```
bool RN487X_ReadMsg(const uint8_t *expectedMsg, uint8_t msgLen);
```
**Read Message**
This API takes input from application on the expected response/status message. 
It starts read RN487X host interface immediately and look for expected message.

```
bool RN487X_ReadDefaultResponse(void);
```
**Read Default Response**
This API takes input from application on the expected response/status message. 
It waits until it receives expected message from RN487x. 
This helps to read the RN487x status messages.

```
void RN487X_WaitForMsg(const char *expectedMsg, uint8_t msgLen);
```
**Wait For Message**
This API reads RN487x interface for default response which is AOK or ERR.

```
bool RN487X_EnterCmdMode(void);
```
**Enter Command Mode**
'$$$' This API puts the RN487x in command mode. 

```
bool RN487X_EnterDataMode(void);
```
**Enter Data Mode**
'---' This API puts the RN487x in data mode.

```
typedef union 
{
    uint16_t gpioMap;
    struct
    {
        rn487x_gpio_ioBitMap_t ioBitMap;
        rn487x_gpio_stateBitMap_t ioStateBitMap;
    };
}rn487x_gpio_bitmap_t;

bool RN487X_SetOutputs(rn487x_gpio_bitmap_t bitMap);
```
**Set Outputs**
This API configures the GPIO pins of the RN487X module as Outputs. 
This API configures the High/Low state of pins set as outputs

```
typedef union
{
    uint8_t gpioStateBitMap;
    struct
    {
        unsigned p2_2_state : 1;
        unsigned p2_4_state : 1;
        unsigned p3_5_state : 1;
        unsigned p1_2_state : 1;
        unsigned p1_3_state : 1;
        unsigned reserved : 3;
    };
}rn487x_gpio_stateBitMap_t;

typedef union
{
    uint8_t gpioBitMap;
    struct
    {
        unsigned p2_2 : 1;          // 01
        unsigned p2_4 : 1;          // 02
        unsigned p3_5 : 1;          // 04
        unsigned p1_2 : 1;          // 08
        unsigned p1_3 : 1;          // 10
        unsigned reserved : 3;
    };
}rn487x_gpio_ioBitMap_t;

rn487x_gpio_stateBitMap_t RN487X_GetInputsValues(rn487x_gpio_ioBitMap_t getGPIOs);
```
**Get Inputs Values**
This API request the GPIO pins state from the RN487X module Inputs. 
This API reads specified GPIO pins current (high/low) state

```
bool RN487X_RebootCmd(void);
```
**Reboot Command**
This API resets RN487x. 
For more details, refer R command in RN4870-71 user guide.

```
bool RN487X_Disconnect(void);
```
**Disconnect Command**
This API disconnects the BLE link between RN487x and remote device. 
For more details, refer K command in RN4870-71 user guide.

```
bool RN487X_SetAsyncMessageHandler(char* pBuffer, uint8_t len);
```
**Set Asynchronous Message Handler**
This API sets up the Buffer and the Buffer size to be used by the Asynchronous Message Handler.

```
bool RN487X_DataReady(void);
```
**Data Ready**
This API checks to see if there is Data ready on the line using Async Message Handling. This API follows the standard UART form.

```
uint8_t RN487X_Read(void);
```
**Read**
This API reads incoming data using Async Message Handling. This API follows the standard UART form.

---

### Private In Source
```
static bool RN487X_FilterData(void)
```
**Filter Data**
This function filters status messages from RN487X data. Called from RN487X_DataReady.

---

## RN487X Interface
The RN Interface functions as the Implementation of the Driver in code. This Interface allows for segregation
between driver and created use case. This Interface allows for easy configuration of communication peripheral
and behavior operations unique to the implementation design. 

The interface at its core allow abstraction for RN related BUS, GPIO, and Asynchronous Message Handle behaviors.

### Public In Header
```
bool RN487X_IsConnected(void);
```
**IsConnected**
Checks Connected State of RN487X

```
// Enum of the RN487X System Configuration Modes
typedef enum
{
    TEST_MODE           = 0x00,
    APPLICATION_MODE    = 0x01 
}RN487X_SYSTEM_MODES_t;

// Struct of RN487X Interface Function Pointer Prototypes
typedef struct
{
    // RN487x UART interface control
    void (*Write)(uint8_t);
    uint8_t (*Read)(void);
    bool (*TransmitDone)(void);
    bool (*DataReady)(void);
    // RN487x RX_IND pin control
    void (*IndicateRx)(bool);
    // RN487x Reset pin control
    void (*Reset)(bool);
    // RN487x Mode pin set
    void (*SetSystemMode)(RN487X_SYSTEM_MODES_t);
    // Delay API
    void (*DelayMs)(uint16_t);
    // Status Message Handler
    void (*AsyncHandler)(char*);
}iRN487X_FunctionPtrs_t;

// Variable Exist Created within Interface Source
extern const iRN487X_FunctionPtrs_t RN487X;
```

---

### Private In Source

### Peripheral/Library Implementations

```
void EUSART2_Write(uint8_t txData)      // PIC
void USART0_Write(const uint8_t data)   // AVR
```
**Write**
This API is configured for to use the 'Write' Function provided by the EUSART BUS configured, and generated through Foundation Service MCC Library. 
Unsigned Byte Data TRANSMIT is API Interface requirement. 

```
uint8_t EUSART2_Read(void)      // PIC
uint8_t USART0_Read(void)       // AVR
```
**Read**
This API is configured for to use the 'Read' Function provided by the EUSART BUS configured, and generated through Foundation Service MCC Library. 
Unsigned Byte Data RETURN is API Interface requirement. 

```
bool EUSART2_is_tx_done(void)       // PIC
bool USART0_IsTxDone(void)          // AVR
```
**Transmit Done**
This API is configured to provided status update from the EUSART BUS when it is finished transmitting. 
Boolean status result is return as true/false. 

```
bool EUSART2_is_rx_ready(void)      // PIC
bool USART0_IsRxReady(void)         // AVR
```
**Data Ready**
This API is configured to provided status update from the EUSART BUS is ready to receive. 
Boolean status result is return as true/false. 
    
---

### Feature Implementation 

```
static void RN487X_Reset(bool value);
```
**Reset**
This API is used to issue a Reset to the RN487X device through hardware GPIO state.

```
static void RN487X_IndicateRx(bool value);
```
**Indicate Receive** 
This API notifies the RN487X device to expect incoming Data through hardware GPIO state.

```
static void RN487X_IndicateRx(bool value);
```
**Set System Mode**
This API sets the System Configuration Mode to Either Normal application Mode or Test Mode/Flash Update/EEPROM configuration

```
void DELAY_milliseconds(uint16_t milliseconds) 
```
**Delay Milliseconds**
This API is configured for DELAY_milliseconds(uint16_t milliseconds) which exist within delay.c; generated through Foundation Services MCC Library.  

```
static void RN487X_MessageHandler(char* message);
```
**Asynchronous Handler**
This API is call through the RN Driver withi the static bool RN487X_FilterData(void) function. 
This function is called when STATUS_MESSAGE_DELIMITER is receieved. 
This interface uses '%' as this character.