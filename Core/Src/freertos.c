/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iwdg.h"
#include "usbd_cdc_if.h"
#include "ADF4351.h"
#include "gpio.h"
#include "i2c.h"
//#include "usart.h"
#include "adc.h"
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
/* USER CODE BEGIN Variables */
float freq = FREQUENCYINIT;
int atten;

//extern I2C_HandleTypeDef hi2c1;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId mainTaskHandle;
osThreadId usbcomTaskHandle;
osThreadId autoattenTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
float FreqGet(char *freqstr);       //获取USB串口收到的频率MHz
float AttenGet(char *attenstr);     //获取USB串口收到的衰减值DB
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void MainTask(void const * argument);
void USBComTask(void const * argument);
void AutoAttenTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of mainTask */
  osThreadDef(mainTask, MainTask, osPriorityNormal, 0, 128);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

  /* definition and creation of usbcomTask */
  osThreadDef(usbcomTask, USBComTask, osPriorityNormal, 0, 128);
  usbcomTaskHandle = osThreadCreate(osThread(usbcomTask), NULL);

  /* definition and creation of autoattenTask */
  osThreadDef(autoattenTask, AutoAttenTask, osPriorityNormal, 0, 128);
  autoattenTaskHandle = osThreadCreate(osThread(autoattenTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_MainTask */
/**
* @brief Function implementing the mainTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MainTask */
void MainTask(void const * argument)
{
  /* USER CODE BEGIN MainTask */
    int countnum=0;

	if((freq < 35) || (freq >= 4400))  	freq  = FREQUENCYINIT;    	//场强超出
    ADF4351Init();
    ADF4351WriteFreq(freq);   
	usb_printf("Current Frequency= %.2f\r\n", freq);        
    
  /* Infinite loop */
  for(;;)
  {      
    if(countnum % 20 == 0){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);           //工作指示灯
        if((AD_value[0]*3.3f/4096) < 0.9 ){                   //中频工作电压临界点
            HAL_GPIO_TogglePin(LED_SIGNAL_GPIO_Port, LED_SIGNAL_Pin);               //信号强度指示灯闪烁，信号差
        }else{
            HAL_GPIO_WritePin(LED_SIGNAL_GPIO_Port, LED_SIGNAL_Pin, GPIO_PIN_RESET); //信号信号强度指示灯常亮（低电平输出），信号正常
        }
        
//        if((AD_value[0]*3.3f/4096) < 0.15 ){
//            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);               //信号强度指示灯闪烁，信号差
//        }else{
//            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); //信号信号强度指示灯常亮（低电平输出），信号正常
//        }        
    }   
      
    if(countnum % 50 == 0) HAL_IWDG_Refresh(&hiwdg);    					//0.5秒喂狗，1.6秒溢出
    
    //if(countnum % 100 == 10) usb_printf("countnum=%d\r\n", countnum);     //1秒,输出计数，测试用
	  
    if(countnum % 6000 == 20){
        usb_printf("RF signal process ver 1.0B   designed by zjjohn WXBTV 2023-04-23\r\n");
		usb_printf("Frequency=%.2f atten=%d \r\n", freq, atten);        	//60秒,调试输出  
  	} 
	
    if(countnum % 3600000 == 10000){	                 //定时10小时间隔，刷新频率，初始化
        ADF4351Init();
        ADF4351WriteFreq(freq);    
    }
           
    if(countnum % 360000 == 5000){	                     //定时1小时间隔，刷新频率
        ADF4351WriteFreq(freq);    
    }      
      
    countnum++;
    if(countnum > 8640000)   countnum = 0;      		//24小时清0
      
    osDelay(10);                                		//10毫秒定时
  }
  /* USER CODE END MainTask */
}

/* USER CODE BEGIN Header_USBComTask */
/**
* @brief Function implementing the usbcomTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USBComTask */
void USBComTask(void const * argument)
{
  /* USER CODE BEGIN USBComTask */
  const char *strptr;	    //指针变量；
  char tempstr[30];         //临时字符串（USB串口接收到的数据缓存）    
  int i;                    //临时变量
  float FAtemp;
  uint32_t NotifyValue;

  /* Infinite loop */
  for(;;)
  {
      NotifyValue=ulTaskNotifyTake(pdTRUE,portMAX_DELAY);       //等待通知  from usbd_cdc_if.c
	  if(NotifyValue == 1){	
          if(RxLen){                                          //接收到usb口的指令
            usb_printf("recevied command= %s  rxlen = %d\r\n", pRx, RxLen);     //收到的命令
            if((RxLen < 2) || (RxLen > 10)){                          //命令长度不合法
                usb_printf("Length Wrong!!\r\n");
            }else{
                for (i = 0; i < RxLen; i++ )   tempstr[i] = *(pRx+i);	   //拷贝接收到的数据。 
                tempstr[RxLen] = 0;
                if(tempstr[0]==102) tempstr[0]=70;      //f-->F
                if(tempstr[0]== 97) tempstr[0]=65;       //a-->A
                /*判断频率数据*/
                strptr = strstr (tempstr, "F");         //判断收到频率数据
                if (strptr){
                    FAtemp = FreqGet(tempstr);
                    if(FAtemp != -1){                   //获取当前的频率
                        ADF4351WriteFreq(FAtemp);       //设置新的频率
						freq = FAtemp;
                        usb_printf("Get Frequency！%.2f\r\n", FAtemp);   
					
                    } else{
                        usb_printf("Frequency wrong!(F35-F4400 MHz)\r\n");
                      }   
                    tempstr[0] = 0;                     //将临时字符串清0
				}    
       
            }
            RxLen = 0;
        }
    } 
      
    osDelay(20);
  }
  /* USER CODE END USBComTask */
}

/* USER CODE BEGIN Header_AutoAttenTask */
/**
* @brief Function implementing the autoattenTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AutoAttenTask */
void AutoAttenTask(void const * argument)
{
  /* USER CODE BEGIN AutoAttenTask */
	int att_flag;
    uint8_t setdata;
    int debugcount = 0;
        
	atten = 20;
    HMC_WriteData(atten);  
    HMCB_WriteData(atten);   
	
  /* Infinite loop */
  for(;;)
  {
    //SET_ReadData();
    setdata = SET_ReadData();
      
    debugcount++;  
    if(debugcount > 36000)  debugcount = 0;
    if(debugcount % 20 == 0)    usb_printf("set=%d  atten=%d\r\n",setdata, atten);
      
    switch(setdata){
        case 0:
            if(debugcount % 200 == 0)    usb_printf("A1 A2 all atten 31.5 dB\r\n");  
            HMC_WriteData(63);
            HMCB_WriteData(63);        
            break;
        case 1:
            if(debugcount % 200 == 0)    usb_printf("A1 atten 10.5 dB\r\n");  
            HMC_WriteData(21);
            HMCB_WriteData(0);  
            break;
        case 2:
            if(debugcount % 200 == 0)    usb_printf("A2 atten 10.5 dB\r\n");  
            HMC_WriteData(0);
            HMCB_WriteData(21);   
            break;
        case 3:
            if(debugcount % 200 == 0)    usb_printf("A2 atten 5 dB\r\n");  
            HMC_WriteData(0);
            HMCB_WriteData(10);   
            break;
        default:
            if(debugcount % 20 == 0){
                //usb_printf("auto atten mode \r\n");  
                usb_printf("PB0=%d     PB1=%d\r\n",AD_value[0],AD_value[1]);
                usb_printf("ch0=%1.3fV  ch1=%1.3fV\r\n",AD_value[0]*3.3f/4096, AD_value[1]*3.3f/4096);            
            }

            att_flag = (int)(((float)(AD_value[0]))/1850 *89 +1);	//计算场强标志值，90为1.499V 控制点
            if(att_flag > 189){                                     //场强标志值超出上限,衰减增加16 = 8dB
                atten = atten + 16;
                if(atten > 126)     atten = 126;
                //usb_printf("atten_flag > 189 %d\r\n", atten);  
                HMC_WriteData_ALL(atten);
                break;
            }
            if(att_flag < 21){                                      //场强标志值低于下限，衰减减少29 = 14.5dB
                atten = atten - 29; 
                if(atten < 0)       atten = 0;
                //usb_printf("atten_flag < 21 %d\r\n", atten);  
                HMC_WriteData_ALL(atten);
                break;
            }
            switch(att_flag){
                case 21:
                    atten = atten -28;
                    break;
                case 22:
                    atten = atten -27;
                    break;
                case 23:
                    atten = atten -26;
                    break;
                case 24:
                    atten = atten -25;
                    break;
                case 25:
                case 26:                    
                    atten = atten -24;
                    break;
                case 27:                    
                    atten = atten -23;
                    break;                
                case 28:
                case 29:                    
                    atten = atten -22;
                    break;
                case 30:
                case 31:                    
                    atten = atten -21;
                    break;               
                case 32:                    
                    atten = atten -20;
                    break;               
                case 33:
                case 34:                    
                    atten = atten -19;
                    break;  
                case 35:
                case 36:                    
                    atten = atten -18;
                    break; 
                case 37:
                case 38:                    
                    atten = atten -17;
                    break; 
                case 39:
                case 40:                    
                    atten = atten -16;
                    break; 
                case 41:
                case 42:                    
                    atten = atten -15;
                    break;           
                case 43:
                case 44:                    
                    atten = atten -14;
                    break; 
                case 45:
                case 46:                    
                    atten = atten -13;
                    break;           
                case 47:    case 48:    case 49:
                    atten = atten -12;
                    break;                 
                case 50:     case 51:   case 52:
                    atten = atten -11;
                    break;  
                case 53:    case 54:    case 55:
                    atten = atten -10;
                    break;  
                case 56:
                case 57:   
                    atten = atten -9;
                    break;  
                case 58:    case 59:    case 60:    case 61:      
                    atten = atten -8;
                    break;                  
                case 62:    case 63:    case 64:
                    atten = atten -7;
                    break;           
                case 65:    case 66:    case 67:    case 68:      
                    atten = atten -6;
                    break;   
                case 69:    case 70:    case 71:
                    atten = atten -5;
                    break;          
                case 72:    case 73:    case 74:    case 75:      
                    atten = atten -4;
                    break;   
                case 76:    case 77:    case 78:    case 79:    case 80:                      
                    atten = atten -3;
                    break;                   
                case 81:    case 82:    case 83:    case 84:      
                    atten = atten -2;
                    break;                  
                case 85:    case 86:    case 87:    case 88:
                case 89:                      
                    atten = atten -1;
                    break;                
                case 90:    case 91:    case 92:    case 93:    case 94:                          
                    atten = atten ;
                    break;   
                case 95:    case 96:    case 97:    case 98:    case 99:                          
                    atten = atten + 1;
                    break;   
                case 100:   case 101:   case 102:   case 103:   case 104:
                case 105: 
                    atten = atten + 2;
                    break;   
                case 106:   case 107:  case 108:    case 109:   case 110:
                case 111: 
                    atten = atten + 3;
                    break;                   
                case 112:   case 113:  case 114:    case 115:   case 116:
                case 117: 
                    atten = atten + 4;
                    break;    
                case 118:   case 119:  case 120:    case 121:    case 122:
                case 123:   case 124:  
                    atten = atten + 5;
                    break;                 
                case 125:   case 126:  case 127:    case 128:    case 129:
                case 130:   case 131:  
                    atten = atten + 6;
                    break;     
                case 132:   case 133:  case 134:    case 135:    case 136:
                case 137:    case 138:  
                    atten = atten + 7;
                    break;                  
                case 139:   case 140:  case 141:    case 142:    case 143:
                case 144:   case 145:  case 146:    case 147:    case 148: 
                    atten = atten + 8;
                    break;    
                case 149:   case 150:  case 151:    case 152:    case 153:
                case 154:   case 155:  case 156:    case 157: 
                    atten = atten + 9;
                    break;  
                case 158:   case 159:  case 160:    case 161:    case 162:
                case 163:   case 164:  case 165:    case 166:    case 167: 
                    atten = atten + 10;
                    break;  
                case 168:   case 169:  case 170:    case 171:    case 172:
                case 173:   case 174:  case 175:    case 176:    case 177: 
                    atten = atten + 11;
                    break;  
                case 178:   case 179:  case 180:    case 181:    case 182: 
                    atten = atten + 12;
                    break;  
                case 183:   case 184:  case 185:    case 186:    case 187: 
                    atten = atten + 13;
                    break; 
                case 188:    
                    atten = atten + 14;
                    break; 
                case 189:    
                    atten = atten + 15;
                    break;                 
                
            }
            if(atten < 0)       atten = 0;
            if(atten > 126)     atten = 126;
            HMC_WriteData_ALL(atten);
        
    }
	    
	osDelay(50);
  }
  /* USER CODE END AutoAttenTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
* @brief 获取字符串中的频率，
* @param freqstr: 字符串   格式：F2546.1
* @retval 正常：浮点数代表频率，单位MHz; 不正常返回：-1
*/
float FreqGet(char *freqstr)
{
    const char *i;
    float freqtemp;
	i = strstr (freqstr , "F");							        //截取场强数据 +CSQ: 21,99
    
    freqtemp = atof(i+1);
    if((freqtemp < 35) || (freqtemp >= 4400))  freqtemp = -1;     //场强超出，设成一个标志为10
    return freqtemp;
}

/**
* @brief 获取字符串中的衰减值
* @param freqstr: 字符串   格式：a11   11 dB
* @retval 正常：浮点数代表衰减值，单位dB  不正常返回：-1
*/
float AttenGet(char *attenstr)
{
    const char *i;
    float attentemp;
	i = strstr (attenstr , "A");							    //截取场强数据 +CSQ: 21,99
    
    attentemp = atof(i+1);
    if((attentemp < 0) ||(attentemp > 32))  attentemp = -1;      //场强超出，设成一个标志为10
    return attentemp;
}



/* USER CODE END Application */

