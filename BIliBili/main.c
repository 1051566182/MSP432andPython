/****************************************************************
 @Copyright     	: Personal development, prohibited for commercial use
 @File_Name 		: main.c
 @Description    : OLED实时显示B站粉丝数和关注数
 @Date     		: 2023年1月18日
 @Author     	: Xiang Song
 @Mail			: songxiang@whu.edu.cn
 ****************************************************************/

//-----------------------------------文件包含-------------------------------------------------
//-----------------------------------file including------------------------------------------
#include "msp.h"
#include "oled\oled.h"
//-----------------------------------本文件内函数声明------------------------------------------
//-----------------------------------function declaration------------------------------------
//UART初始化
void initUART();
//-----------------------------------本文件内变量声明------------------------------------------
//-----------------------------------variable declaration------------------------------------
char string[20];
char following[10];
char follower[10];
int j=0;
int i=0;
int separator=0;//记录分隔符的位置，分开两组数据
//-----------------------------------本文件内变量定义------------------------------------------
//-----------------------------------Variable definitions------------------------------------

//-----------------------------------main function-------------------------------------------
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    CS->KEY = CS_KEY_VAL;                // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3; // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                      // Lock CS module from unintended accesses

    initUART();
    P5DIR |= BIT1;
    P5OUT &= ~BIT1;
    OLED_Init();/*init OLED*/
    OLED_Clear(); /*clear OLED screen*/
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt
    OLED_ShowString(5, 1,"following:");
    OLED_ShowString(5, 3,"follower:");
    //OLED_ShowString(85, 1,"none");
    //OLED_ShowString(85, 3,"none");
    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);

    // Enable sleep on exit from ISR
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures SLEEPONEXIT occurs immediately
    __DSB();

    // Enter LPM0
    __sleep();
    __no_operation();
    // For debugger
}
// UART interrupt service routine
void EUSCIA0_IRQHandler(void)
{

    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
       {
           while (!(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG))
               ;
           string[j] = EUSCI_A0->RXBUF;
           j++;
           if(string[j-1]==',')
           {
               separator = j-1;
           }
       }

    if(string[j-1]=='S')
    {
        for( i=0;i<j-1;i++)
        {
            if(i<separator)
                following[i]=string[i];
            if(i>separator)
                follower[i-separator-1]=string[i];
        }
        OLED_ShowString(85, 1,following);
        OLED_ShowString(85, 3,follower);
        j=0;

    }

}
//-----------------------------------函数定义-------------------------------------------------
//-----------------------------------function definition-------------------------------------
/****************************************************************
 @Function：
 @Input：
 @Output：
 @Author：
 ****************************************************************/
void initUART(void)
{
    // Configure UART pins
    P1->SEL0 |= BIT2 | BIT3;             // set 2-UART pin as secondary function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK; // Configure eUSCI clock source for SMCLK
    // Baud Rate calculation
    // 12000000/(16*9600) = 78.125
    // Fractional portion = 0.125
    // User's Guide Table 21-4: UCBRSx = 0x10
    // UCBRFx = int ( (78.125-78)*16) = 2
    EUSCI_A0->BRW = 78;                     // 12000000/16/9600
    EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
    EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
}

//----------------------end of this file-----------------------------------------------------
