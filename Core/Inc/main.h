/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define HMC0_Pin GPIO_PIN_0
#define HMC0_GPIO_Port GPIOA
#define HMC1_Pin GPIO_PIN_1
#define HMC1_GPIO_Port GPIOA
#define HMC2_Pin GPIO_PIN_2
#define HMC2_GPIO_Port GPIOA
#define HMC3_Pin GPIO_PIN_3
#define HMC3_GPIO_Port GPIOA
#define HMC4_Pin GPIO_PIN_4
#define HMC4_GPIO_Port GPIOA
#define HMC5_Pin GPIO_PIN_5
#define HMC5_GPIO_Port GPIOA
#define LED_SIGNAL_Pin GPIO_PIN_7
#define LED_SIGNAL_GPIO_Port GPIOA
#define HMCB4_Pin GPIO_PIN_10
#define HMCB4_GPIO_Port GPIOB
#define HMCB5_Pin GPIO_PIN_11
#define HMCB5_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define SET0_Pin GPIO_PIN_3
#define SET0_GPIO_Port GPIOB
#define SET1_Pin GPIO_PIN_4
#define SET1_GPIO_Port GPIOB
#define SET2_Pin GPIO_PIN_5
#define SET2_GPIO_Port GPIOB
#define HMCB0_Pin GPIO_PIN_6
#define HMCB0_GPIO_Port GPIOB
#define HMCB1_Pin GPIO_PIN_7
#define HMCB1_GPIO_Port GPIOB
#define HMCB2_Pin GPIO_PIN_8
#define HMCB2_GPIO_Port GPIOB
#define HMCB3_Pin GPIO_PIN_9
#define HMCB3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
//# define EEPROM_SAVE						//启用EEPROM保存频率、衰减数据 
# define ATTENINIT         			20		//缺省衰减值
# define FREQUENCYINIT  			736	    //缺省频率	
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
