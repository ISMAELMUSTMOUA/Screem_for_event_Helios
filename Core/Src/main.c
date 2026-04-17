/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "font.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

COM_InitTypeDef BspCOMInit;
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
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
	uint8_t estado_pantalla = 0;         // 0 = Fondo Rojo, 1 = Barras de color
	uint8_t actualizar_pantalla = 1;     // 1 = Fuerza a dibujar la primera vez al encender
	//uint8_t estado_boton_anterior = 1;   // Asumimos que el botón usa Pull-up (1 = sin pulsar)
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
  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */



  /* USER CODE END 2 */

  /* Initialize led */
  BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);

    // Ejecutando ILI9488_Init para la inicalizacion
    ILI9488_Init();
    botton_init();
    // 2. Dibujar el logo del Club Helios (Ajusta la X y la Y si hace falta centrarlo más)
    ILI9488_DrawImage(0, 0, 480, 320, club_helios_565);

    HAL_Delay(3000);


	  // Variables de simulación para la prueba
	        int velocidad_falsa = 60;
	        uint8_t modo_eco = 1;
	        int bateria_falsa = 100;
	        int temp_falsa = 70;
	        int pre_falsa = 2;
	        // -- NUEVOS SENSORES --
	        int rpm_falsas = 2000;
	        float volt_falso = 52.4; // Tensión batería
	        float amp_falso = 12.5;  // Consumo corriente
	        int eta_horas = 3;       // Tiempo restante estimado
	        int eta_minutos = 45;
			//Variables genericas
			uint8_t boton_anterior = 0;
			uint32_t ultimo_tiempo = 0;
			uint32_t puntos_juego = 0;
			uint8_t pulso_limpio = 0;
			int neumatico_seleccionado = 1;




  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  // ================================================================
	        // 1. LEER BOTONES A MÁXIMA VELOCIDAD (Millones de veces por segundo)
	        // ================================================================
	        uint8_t boton_actual = Leer_Botones();
	        pulso_limpio = 0;

	        if (boton_actual != 0 && boton_anterior == 0) {
	             pulso_limpio = boton_actual; // ¡Detectada una pulsación nueva!

	             // --- NAVEGACIÓN ENTRE PANTALLAS ---
	             if (pulso_limpio == 1) {
	                 // Botón 1 (PA1): Alterna Telemetría y Reglamento
	                 if (estado_pantalla == 0) estado_pantalla = 1;
	                 else if(estado_pantalla == 1 || estado_pantalla == 2) estado_pantalla = 0;
	                 actualizar_pantalla = 1;
	             }
	             else if (pulso_limpio == 4 && estado_pantalla == 0) {
	                 // Botón 4 (PB5): Si estamos en Telemetría, ENTRAR AL JUEGO
	                 estado_pantalla = 2;
	                 actualizar_pantalla = 1;
	             }
	             else if (pulso_limpio == 14 && estado_pantalla == 2) {
	                 // Botón 14 (PC9 - Extremo Derecho): BOTÓN DE PÁNICO. Salir del juego a la fuerza.
	                 estado_pantalla = 0;
	                 actualizar_pantalla = 1;
	             }

	             // --- ACCIONES DE TELEMETRÍA (Solo funcionan si vemos el Dashboard) ---
	             else if (estado_pantalla == 0) {
	                 switch (pulso_limpio) {
	                     case 2: // Botón 2 (PA4): Modo ECO / RACE
	                         if (modo_eco == 1) { modo_eco = 0; } else { modo_eco = 1; }
	                         break;
	                     case 3: // Botón 3 (PA5): Resetear reloj
	                         eta_horas = 3; eta_minutos = 0;
	                         break;
	                     // Aquí puedes añadir del case 5 al 14 para encender luces, bomba de agua, etc.
						   case 5:  // Botón 5 (PC0) cambia de rueda 1 -> 2 -> 3 -> 1...
							   neumatico_seleccionado++;
							   if (neumatico_seleccionado > 3) {
								   neumatico_seleccionado = 1;
							   }

							   // Borramos SOLO el cuadradito donde pone "PRS1:" (Fondo negro)
								   ILI9488_FillRect(325, 220, 65, 20, 0, 0, 0);

								   // Escribimos el nuevo texto de la rueda
								   if (neumatico_seleccionado == 1) ILI9488_DrawString(325, 220, "PRS1:", 180, 180, 180, 0, 0, 0, 2);
								   else if (neumatico_seleccionado == 2) ILI9488_DrawString(325, 220, "PRS2:", 180, 180, 180, 0, 0, 0, 2);
								   else ILI9488_DrawString(325, 220, "PRS3:", 180, 180, 180, 0, 0, 0, 2);

								   // Forzamos que se repinten los números de la telemetría (enviando el 1 al final)
								   Helios_UpdateTelemetry(velocidad_falsa, bateria_falsa, modo_eco,
														  temp_falsa, pre_falsa, rpm_falsas,
														  volt_falso, amp_falso, eta_horas, eta_minutos, 1);

							   // Levantamos bandera para que se repinte el fondo
							   //actualizar_pantalla = 1;
							   break;
	                 }
	             }
	         }
	         boton_anterior = boton_actual;

	         // ================================================================
	         // 2. DIBUJO ESTÁTICO DE PANTALLAS (Solo cuando se cambia)
	         // ================================================================
	         if (actualizar_pantalla == 1) {
	             if (estado_pantalla == 0) {
	                 Helios_DrawDashboard_Static(neumatico_seleccionado);
	                 // Forzamos el 1 al final para repintar la telemetría al volver
	                 Helios_UpdateTelemetry(velocidad_falsa, bateria_falsa, modo_eco,
	                                        temp_falsa, pre_falsa, rpm_falsas,
	                                        volt_falso, amp_falso, eta_horas, eta_minutos, 1);
	             } else if (estado_pantalla == 1) {
	                 Helios_DrawRules_Static();
	             } else if (estado_pantalla == 2) {
	                 Helios_DrawGame_Static();
	                 puntos_juego = 0; // El juego empieza con 0 puntos
	                 Helios_RunGame(0, &puntos_juego, 1);
	                 }
	             actualizar_pantalla = 0; // Bajamos la bandera
	         }


	         // ================================================================
	         // 3. EJECUCIÓN DEL JUEGO (Alta Frecuencia)
	         // ================================================================
	         if (estado_pantalla == 2) {
	             // El juego recibe el pulso_limpio para saber si el público ha pulsado los botones 1, 2, 3 o 4
	             Helios_RunGame(pulso_limpio, &puntos_juego, 0);
	         }


	         // ================================================================
	         // 4. CÁLCULO DE FÍSICAS (Solo cada 500ms)
	         // ================================================================
	         if (HAL_GetTick() - ultimo_tiempo >= 500) {

	             ultimo_tiempo = HAL_GetTick(); // Reiniciar cronómetro

	             // Simular físicas del coche (sigue pasando aunque estemos jugando)
	             velocidad_falsa++;
	             if (velocidad_falsa > 99) velocidad_falsa = 40;
	             rpm_falsas = velocidad_falsa * 60;

	             if (velocidad_falsa % 2 == 0) {
	                 bateria_falsa--; volt_falso -= 0.1;
	                 if (bateria_falsa < 0) { bateria_falsa = 100; volt_falso = 54.0; }
	             }
	             amp_falso = 10.0 + (velocidad_falsa / 10.0);

	             temp_falsa++;
	             if (temp_falsa > 85) temp_falsa = 70;

	             eta_minutos--;
	             if (eta_minutos < 0) {
	                 eta_minutos = 59; eta_horas--;
	                 if (eta_horas < 0) eta_horas = 3;
	             }

	             // Pintar métricas solo si estamos viendo el Dashboard
	             if (estado_pantalla == 0 && actualizar_pantalla == 0) {
	                 // Le pasamos 0 al final (anti-flickering activado)
	                 Helios_UpdateTelemetry(velocidad_falsa, bateria_falsa, modo_eco,
	                                        temp_falsa, pre_falsa, rpm_falsas,
	                                        volt_falso, amp_falso, eta_horas, eta_minutos, 0);
	             }
	         }
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TFT_CS_Pin|TFT_DC_Pin|TFT_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : TOUCH_CS_Pin */
  GPIO_InitStruct.Pin = TOUCH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TOUCH_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_CS_Pin */
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TFT_CS_Pin TFT_DC_Pin */
  GPIO_InitStruct.Pin = TFT_CS_Pin|TFT_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_RST_Pin */
  GPIO_InitStruct.Pin = TFT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_RST_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
