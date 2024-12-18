/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h> // For printf

/* Defines -------------------------------------------------------------------*/
#define MAX7219_CS_PIN GPIO_PIN_0
#define MAX7219_CS_PORT GPIOB

#define JOYSTICK_UP_PIN GPIO_PIN_1
#define JOYSTICK_DOWN_PIN GPIO_PIN_2
#define JOYSTICK_LEFT_PIN GPIO_PIN_3
#define JOYSTICK_RIGHT_PIN GPIO_PIN_4
#define JOYSTICK_PORT GPIOA

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart2;

/* Function prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);

void MAX7219_Init(void);
void MAX7219_SetAllLEDs(uint8_t state);
void MAX7219_TurnOnEffect(void);
void MAX7219_TurnOffEffect(void);
uint8_t ReadJoystickState(void);

/* Debug UART support --------------------------------------------------------*/
#ifdef __GNUC__
int _write(int file, char *data, int len) {
    return HAL_UART_Transmit(&huart2, (uint8_t *)data, len, HAL_MAX_DELAY);
}
#endif

int main(void) {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART2_UART_Init();

    printf("Initializing peripherals...\r\n");

    MAX7219_Init(); // Initialize MAX7219 for the LED matrix

    while (1) {
        // Read joystick state
        uint8_t joystickPressed = ReadJoystickState();

        if (joystickPressed) {
            MAX7219_TurnOnEffect(); // Turn on all LEDs with effect
            printf("Joystick pressed: Turning on LEDs with effect\r\n");
        } else {
            MAX7219_TurnOffEffect(); // Turn off all LEDs with effect
            printf("No joystick press: Turning off LEDs with effect\r\n");
        }

        HAL_Delay(100); // Delay for debounce
    }
}

/* MAX7219 Functions ---------------------------------------------------------*/
void MAX7219_Init(void) {
    HAL_GPIO_WritePin(MAX7219_CS_PORT, MAX7219_CS_PIN, GPIO_PIN_SET); // CS high
    MAX7219_SetAllLEDs(0); // Turn off all LEDs initially
}

void MAX7219_Send(uint8_t address, uint8_t data) {
    uint8_t spiData[2] = {address, data};
    HAL_GPIO_WritePin(MAX7219_CS_PORT, MAX7219_CS_PIN, GPIO_PIN_RESET); // CS low
    HAL_SPI_Transmit(&hspi1, spiData, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(MAX7219_CS_PORT, MAX7219_CS_PIN, GPIO_PIN_SET);   // CS high
}

void MAX7219_SetAllLEDs(uint8_t state) {
    uint8_t value = state ? 0xFF : 0x00; // 0xFF for ON, 0x00 for OFF
    for (uint8_t i = 1; i <= 8; i++) {
        MAX7219_Send(i, value); // Set all rows
    }
    MAX7219_Send(0x0C, 0x01); // Ensure normal operation mode
}

void MAX7219_TurnOnEffect(void) {
    for (uint8_t i = 1; i <= 8; i++) {
        MAX7219_Send(i, 0xFF); // Gradually turn on rows
        HAL_Delay(50);
    }
    MAX7219_Send(0x0C, 0x01); // Ensure normal operation mode
}

void MAX7219_TurnOffEffect(void) {
    for (uint8_t i = 8; i >= 1; i--) {
        MAX7219_Send(i, 0x00); // Gradually turn off rows
        HAL_Delay(50);
    }
    MAX7219_Send(0x0C, 0x01); // Ensure normal operation mode
}

/* Joystick Functions --------------------------------------------------------*/
uint8_t ReadJoystickState(void) {
    if (HAL_GPIO_ReadPin(JOYSTICK_PORT, JOYSTICK_UP_PIN) == GPIO_PIN_RESET ||
        HAL_GPIO_ReadPin(JOYSTICK_PORT, JOYSTICK_DOWN_PIN) == GPIO_PIN_RESET ||
        HAL_GPIO_ReadPin(JOYSTICK_PORT, JOYSTICK_LEFT_PIN) == GPIO_PIN_RESET ||
        HAL_GPIO_ReadPin(JOYSTICK_PORT, JOYSTICK_RIGHT_PIN) == GPIO_PIN_RESET) {
        return 1; // Joystick is pressed in any direction
    }
    return 0; // No joystick press
}

/* Peripheral Initialization -------------------------------------------------*/
void SystemClock_Config(void) {
    // Configure system clock (same as your existing template)
}

static void MX_SPI1_Init(void) {
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure GPIO pin for MAX7219 CS */
    HAL_GPIO_WritePin(MAX7219_CS_PORT, MAX7219_CS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = MAX7219_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(MAX7219_CS_PORT, &GPIO_InitStruct);

    /* Configure GPIO pins for Joystick */
    GPIO_InitStruct.Pin = JOYSTICK_UP_PIN | JOYSTICK_DOWN_PIN | JOYSTICK_LEFT_PIN | JOYSTICK_RIGHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(JOYSTICK_PORT, &GPIO_InitStruct);
}

/* Error Handler -------------------------------------------------------------*/
void Error_Handler(void) {
    while (1) {
    }
}
