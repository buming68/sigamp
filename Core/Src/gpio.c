/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, HMC0_Pin|HMC1_Pin|HMC2_Pin|HMC3_Pin
                          |HMC4_Pin|HMC5_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_SIGNAL_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, HMCB4_Pin|HMCB5_Pin|HMCB0_Pin|HMCB1_Pin
                          |HMCB2_Pin|HMCB3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = HMC0_Pin|HMC1_Pin|HMC2_Pin|HMC3_Pin
                          |HMC4_Pin|HMC5_Pin|LED_SIGNAL_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin */
  GPIO_InitStruct.Pin = HMCB4_Pin|HMCB5_Pin|HMCB0_Pin|HMCB1_Pin
                          |HMCB2_Pin|HMCB3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SET0_Pin|SET1_Pin|SET2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void HMC_WriteData_ALL(uint8_t data)
{
    if(data <= 0)    data = 0;
    if(data > 126)   data = 126;
    
    if(data > 63){
        HMCB_WriteData(63);
        HMC_WriteData(data - 63);
    }else{
        HMCB_WriteData(data);
        HMC_WriteData(0);    
    }

}


void HMC_WriteData(uint8_t data)
{
   //�ֱ�����벽��˥��оƬ���ӵ�IO�ڣ�˳�����ɸ�λ����λ
//    HAL_GPIO_WritePin(GPIOA, HMC5_Pin,((data & 0x20) >> 5 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOA, HMC4_Pin,((data & 0x10) >> 4 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOA, HMC3_Pin,((data & 0x08) >> 3 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOA, HMC2_Pin,((data & 0x04) >> 2 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOA, HMC1_Pin,((data & 0x02) >> 1 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOA, HMC0_Pin,((data & 0x01) >> 0 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    
    HAL_GPIO_WritePin(GPIOA, HMC0_Pin,((data & 0x20) >> 5 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(GPIOA, HMC1_Pin,((data & 0x10) >> 4 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(GPIOA, HMC2_Pin,((data & 0x08) >> 3 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(GPIOA, HMC3_Pin,((data & 0x04) >> 2 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(GPIOA, HMC4_Pin,((data & 0x02) >> 1 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(GPIOA, HMC5_Pin,((data & 0x01) >> 0 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    
}

void HMCB_WriteData(uint8_t data)
{
   //�ֱ�����벽��˥��оƬ���ӵ�IO�ڣ�˳�����ɸ�λ����λ
//    HAL_GPIO_WritePin(GPIOB, HMCB5_Pin,((data & 0x20) >> 5 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOB, HMCB4_Pin,((data & 0x10) >> 4 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOB, HMCB3_Pin,((data & 0x08) >> 3 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOB, HMCB2_Pin,((data & 0x04) >> 2 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOB, HMCB1_Pin,((data & 0x02) >> 1 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
//    HAL_GPIO_WritePin(GPIOB, HMCB0_Pin,((data & 0x01) >> 0 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    
    HAL_GPIO_WritePin(HMCB0_GPIO_Port, HMCB0_Pin,((data & 0x20) >> 5 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(HMCB1_GPIO_Port, HMCB1_Pin,((data & 0x10) >> 4 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(HMCB2_GPIO_Port, HMCB2_Pin,((data & 0x08) >> 3 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(HMCB3_GPIO_Port, HMCB3_Pin,((data & 0x04) >> 2 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(HMCB4_GPIO_Port, HMCB4_Pin,((data & 0x02) >> 1 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
    HAL_GPIO_WritePin(HMCB5_GPIO_Port, HMCB5_Pin,((data & 0x01) >> 0 == 1)? GPIO_PIN_RESET:GPIO_PIN_SET ); 
   
}

uint8_t SET_ReadData()
{
    uint8_t setdata;
    
    setdata = HAL_GPIO_ReadPin(SET0_GPIO_Port, SET0_Pin) + HAL_GPIO_ReadPin(SET1_GPIO_Port, SET1_Pin)*2 + HAL_GPIO_ReadPin(SET2_GPIO_Port, SET2_Pin)*4 ;
    
    return setdata;
    
}
/* USER CODE END 2 */
