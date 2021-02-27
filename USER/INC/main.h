#ifndef _MAIN_H
#define _MAIN_H

#include "can1.h"
#include "led.h"
#include "beep.h"
#include "timer.h"
#include "usart2.h"
#include "usart1.h"
#include "visual_scope.h"

//START任务
#define START_TASK_PRIO            5
#define START_TASK_STK_SIZE        256
__align(8) OS_STK start_task_stk[START_TASK_STK_SIZE];
static void TaskStart(void *pdata);

//线性拟合任务
#define CurveFit_TASK_PRIO         10
#define CurveFit_STK_SIZE          256
__align(8) OS_STK CurveFit_task_stk[CurveFit_STK_SIZE];
static void TaskCurveFit(void *pdata);

//CAN1发送任务
#define CAN1Tx_TASK_PRIO           20
#define CAN1Tx_TASK_STK_SIZE       256
__align(8) OS_STK CAN1Tx_task_stk[CAN1Tx_TASK_STK_SIZE];
static void TaskCAN1Tx(void *pdata);

//LCD任务
#define LCD_TASK_PRIO       	   25
#define LCD_STK_SIZE     		   512
__align(8) OS_STK LCD_TASK_STK[LCD_STK_SIZE];
static void Task_Lcd(void *pdata);

//USART1任务
#define USART1_TASK_PRIO       	   55
#define USART1_STK_SIZE     	   512
__align(8) OS_STK USART1_TASK_STK[USART1_STK_SIZE];
static void Task_USART1(void *pdata);

//虚拟示波器
#define SCOPE_TASK_PRIO			55
#define SCOPE_STK_SIZE			128
__align(8) OS_STK SCOPE_TASK_STK[SCOPE_STK_SIZE];
static void Task_Scope(void *pdata);

OS_EVENT *RUN;

#endif
