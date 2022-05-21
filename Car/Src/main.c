/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "driver.h"

#include "nrf24l01.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

DRIVER *driver;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim3)
  {
    timer_running();
    run_inner_timer_node();
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
  case M1_EA_I1_Pin:
    /* code */
    break;
  case M1_EB_I0_Pin:
    /* code */
    break;
  case M2_EA_I7_Pin:
    /* code */
    break;
  case M2_EB_I6_Pin:
    /* code */
    break;
  default:
    break;
  }
}

// #define TEST_NRF 1
#define TEST 1

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint8_t recivedDataStruct[32];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  #ifndef TEST
  MX_IWDG_Init();
  #endif
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  DRIVER_Init();
  PROC_Init();

  // 复位转向

  // -1.5
  DriverCar(driver, 0, 0);

  HAL_Delay(2000);


  while (NRF24L01_Check() == 1)
  {
    HAL_Delay(1000);
    /* code */
  };
  
  HAL_Delay(200);
  
  NRF24L01_RX_Mode();

  #ifdef TEST_NRF
  #endif 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #ifndef TEST
    HAL_IWDG_Refresh(&hiwdg);
    #endif

    if (htim3.State == HAL_TIM_STATE_BUSY)
    {
      run_timer_node();
    }

    #ifdef TEST_MOTOR
    for (size_t i = 0; i < 100; i++)
    {
      DriverPower(driver, 0, i * 0.01, i * 0.01);
      HAL_Delay(50);
    }
    HAL_Delay(300);
    for (size_t i = 0; i < 100; i++)
    {
      DriverPower(driver, 0, -i * 0.01, -i * 0.01);
      HAL_Delay(50);
    }
    HAL_Delay(300);
    for (size_t i = 100; i > 1; i--)
    {
      DriverPower(driver, 0, i * 0.01, i * 0.01);
      HAL_Delay(50);
    }
    HAL_Delay(300);
    for (size_t i = 100; i > 1; i--)
    {
      DriverPower(driver, 0, -i * 0.01, -i * 0.01);
      HAL_Delay(50);
    }
    HAL_Delay(300);
    #endif

    #ifdef TEST_SERVO
      DriverPower(driver, 0, 0, 0);
    for (size_t i = 0; i < 100; i++)
    {
      DriverPower(driver, (float) i / 100, 0, 0);
      HAL_Delay(200);
    }
    HAL_Delay(2000);
    for (size_t i = 100; i > 0; i--)
    {
      DriverPower(driver, (float) i / 100, 0, 0);
      HAL_Delay(20);
    }
    DriverPower(driver, -1, 0, 0);

    HAL_Delay(2000);
    #endif

    #ifdef TEST_NRF
    auto irq = NRF24L01_IRQ_PIN_READ();

    if (irq == 0)
    {
      // recived
      HAL_Delay(2000);
    }

    uint8_t dt = NRF24L01_RxPacket(recivedDataStruct);
    if (dt == 0) {
      HAL_Delay(100);
    }
    #endif 
    // DriverPower(driver, 0, 0.5, -0.5); // 左

    // HAL_Delay(5000);
    // DriverPower(driver, 1, 0, 0); // 左
    // HAL_Delay(5000);

    // DriverPower(driver, 0, 0, 0); // 向
    // HAL_Delay(5000);
    // DriverPower(driver, -1, 0.5, 0.5); // 向
    // HAL_Delay(5000);
    // DriverPower(driver, 0, 0, 0); // 向
    // HAL_Delay(5000);
    // HAL_Delay(5000);
    // DriverPower(driver, -1, 0, 0); // 向右
    // HAL_Delay(5000);
    // DriverPower(driver, 0, 0, 0); // 向
    // HAL_Delay(5000);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
    HAL_Delay(500);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */