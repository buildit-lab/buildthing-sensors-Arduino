/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "BCACC316G.h"
#include "BCALS083K.h"
#include "BCAQS1187.h"
#include "BCDPDC100.h"
#include "BCHTS4085.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BCACC316G_ADDR 0x53 //ADXL343
#define BCALS083K_ADDR 0x44 //OPT3001Q1
#define BCAQS1187_ADDR 0x5A  //CSS811
#define BCDPDC100_ADDR 0x39	//TMD2671
#define BCHTS4085_ADDR 0x40 //HDC2010

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
void sensorInitialize(void);
void readSensorData(void);

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
	
	return ch;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_I2C2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	sensorInitialize();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		readSensorData();
		HAL_Delay(1000);
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{
  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void sensorInitialize(void){
	
	/*BCACC316G sensorInitialize*/
  printf("[BCACC316G] 3axis accletormeter Enabled\n\r");
  
	BCACC316G_init(BCACC316G_ADDR, &hi2c2);
	
	BCACC316G_powerOn();
  
  //set activity/ inactivity thresholds (0-255)
  BCACC316G_setActivityThreshold(75); //62.5mg per increment
  BCACC316G_setInactivityThreshold(75); //62.5mg per increment
  BCACC316G_setTimeInactivity(10); // how many seconds of no activity is inactive?
  
  //look of activity movement on this axes - 1 == on; 0 == off 
  BCACC316G_setActivityX(1);
  BCACC316G_setActivityY(1);
  BCACC316G_setActivityZ(1);

  //look of inactivity movement on this axes - 1 == on; 0 == off
  BCACC316G_setInactivityX(1);
  BCACC316G_setInactivityY(1);
  BCACC316G_setInactivityZ(1);
  
  //look of tap movement on this axes - 1 == on; 0 == off
  BCACC316G_setTapDetectionOnX(0);
  BCACC316G_setTapDetectionOnY(0);
  BCACC316G_setTapDetectionOnZ(1);
  
  //set values for what is a tap, and what is a double tap (0-255)
  BCACC316G_setTapThreshold(50); //62.5mg per increment
  BCACC316G_setTapDuration(15); //625¥ìs per increment
  BCACC316G_setDoubleTapLatency(80); //1.25ms per increment
  BCACC316G_setDoubleTapWindow(200); //1.25ms per increment

  //set values for what is considered freefall (0-255)
  BCACC316G_setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  BCACC316G_setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
  
  //setting all interupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  BCACC316G_setInterruptMapping( BCACC316G_INT_SINGLE_TAP_BIT, BCACC316G_INT1_PIN );
  BCACC316G_setInterruptMapping( BCACC316G_INT_DOUBLE_TAP_BIT, BCACC316G_INT1_PIN );
  BCACC316G_setInterruptMapping( BCACC316G_INT_FREE_FALL_BIT, BCACC316G_INT1_PIN );
  BCACC316G_setInterruptMapping( BCACC316G_INT_ACTIVITY_BIT, BCACC316G_INT1_PIN );
  BCACC316G_setInterruptMapping( BCACC316G_INT_INACTIVITY_BIT, BCACC316G_INT1_PIN );

  //register interupt actions - 1 == on; 0 == off 
  BCACC316G_setInterrupt( BCACC316G_INT_SINGLE_TAP_BIT, 1);
  BCACC316G_setInterrupt( BCACC316G_INT_DOUBLE_TAP_BIT, 1);
  BCACC316G_setInterrupt( BCACC316G_INT_FREE_FALL_BIT, 1);
  BCACC316G_setInterrupt( BCACC316G_INT_ACTIVITY_BIT, 1);
  BCACC316G_setInterrupt( BCACC316G_INT_INACTIVITY_BIT, 1);
	
  printf("Initialized BC Sensors Series\n\r");  
  printf("\n\r");
  printf("\n\r");
	
	
	/* BCALS083K sensorinitialize */
	printf("[BCALS083K] Ambient Light Sensor Enabled\n\r");
	
	BCALS083K_init(BCALS083K_ADDR, &hi2c2);
  BCALS083K_setRange(12); //Auto full scale
  
	BCALS083K_setTime(1);   //set time 800ms
	HAL_Delay(20);
  BCALS083K_setMode(BCALS083K_CONTINUOUS);
  BCALS083K_setPolarity(BCALS083K_ACTIVE_HIGH);
  BCALS083K_setLatch(BCALS083K_WINDOW_STYLE);
  BCALS083K_setMaskExponent(1); //Range set to less than 14
  BCALS083K_setFaultCount(BCALS083K_FAULT_CNT_ONE);

  printf("Initialized BC Sensors Series\n\r");  
  printf("\n\r");
  printf("\n\r");
	
	
	/*BCAQS1187 sensorInitialize*/
	printf("[BCAQS1187] Indoor Air Quarity Sensor Enabled\n\r");
	
	BCAQS1187_init(BCAQS1187_ADDR, &hi2c2);
  BCAQS1187_SWReset();
  HAL_Delay(100);
  BCAQS1187_appStart();
  BCAQS1187_setThresholds((uint16_t) 1500, (uint16_t) 2500, (uint8_t) 50);
  BCAQS1187_setDriveMode(BCAQS1187_DRIVE_MODE_1SEC); 

  printf("Initialized BC Sensors Series\n\r"); 
	printf("\n\r");
	printf("\n\r");
	
	
	/*BCDPDC100 sensorInitialize*/
	printf("[BCDPDC100] Digital Proximity Detector Enabled\n\r");
  
	BCDPDC100_init(BCDPDC100_ADDR, &hi2c2);
  BCDPDC100_powerOn();
  BCDPDC100_enableInterruptMask();
  BCDPDC100_enableWait();    
  BCDPDC100_disableProximity();
  BCDPDC100_writeAtime(0xFF);
  BCDPDC100_writePtime(0xFF);
  BCDPDC100_writeWtime(0xFF);
  BCDPDC100_setLedPower(BCDPDC100_LED100P);
  BCDPDC100_setDiode(BCDPDC100_BOTH);
  BCDPDC100_setPpulse(32);    
  BCDPDC100_setLowThresold(0);    
  BCDPDC100_setHighThresold(1000);  
  BCDPDC100_setPersistence(10);
  BCDPDC100_enableProximity();    
	
  printf("Initialized BC Sensors Series\n\r");  
  printf("\n\r");
  printf("\n\r");
	
	
	/*BCHTS4085 sensorInitialize*/
	printf("[BCHTS4085] Temperature & Humidity Sensor Enabled\n\r");
	BCHTS4085_init(BCHTS4085_ADDR, &hi2c2);
	BCHTS4085_setTempRes(BCHTS4085_FOURTEEN_BIT);
  BCHTS4085_setHumidRes(BCHTS4085_FOURTEEN_BIT);
  BCHTS4085_setRate(BCHTS4085_ONE_HZ);
  BCHTS4085_triggerMeasurement();
	printf("Initialized BC Sensors Series\n\r");
	printf("\n\r");
	printf("\n\r");
}

void readSensorData()
{  
	/* BCACC316G read Sensor Data */
  int x,y,z; 
  BCACC316G_readAccel_seperate(&x, &y, &z); //read the accelerometer values and store them in variables x,y,z
  
  // Output x,y,z values - Commented out
  printf("[BCACC316G] x : %d   y : %d   z : %d", x, y, z);
	

  //Fun Stuff! 
  //read interrupts source and look for triggerd actions
  
  //getInterruptSource clears all triggered actions after returning value
  //so do not call again until you need to recheck for triggered actions
  uint8_t interrupts = BCACC316G_getInterruptSource(0, 0);
  
  // freefall
  if(BCACC316G_triggered(interrupts, BCACC316G_FREE_FALL))
  {
      printf(" (freefall)");
  } 
  
  //inactivity
  if(BCACC316G_triggered(interrupts, BCACC316G_INACTIVITY))
  {
      printf(" (inactivity)");
  }
  
  //activity
  if(BCACC316G_triggered(interrupts, BCACC316G_ACTIVITY))
  {
      printf(" (activity)"); 
  }
  
  //double tap
  if(BCACC316G_triggered(interrupts, BCACC316G_DOUBLE_TAP))
  {
      printf(" (double tap)");
  }
  
  //tap
  if(BCACC316G_triggered(interrupts, BCACC316G_SINGLE_TAP))
  {
      printf("(tap)");
  }
  printf("\n\r");
	
	
	/* BCALS083K read Sensor Data */
	printf("[BCALS083K] ALS : %0.2f lux\n\r", BCALS083K_readResult());
	
	
	/* BCAQS1187 read Sensor Data */
	BCAQS1187_readResultData();
  printf("[BCAQS1187] TVOC : %d ppb     eCO2 :  %d ppm\n\r", BCAQS1187_getTVOC(), BCAQS1187_geteCO2());
	
	
	/* BCDPDC100 read Sensor Data */
	printf("[BCDPDC100] Proximity ADC : %d", BCDPDC100_readProximityAdc());
       
   if(BCDPDC100_readInterrupt())//Proximity inturrupt
  {
		printf(" (proximity) ");
  }
	printf("\n\r");
	
		/* BCHTS4085 read Sensor Data */
		printf("[BCHTS4085] Temp : %0.2f  C   Humid : %0.2f %%\n\r", BCHTS4085_readTemp(), BCHTS4085_readHumidity());
	
	printf("\n\r");
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
