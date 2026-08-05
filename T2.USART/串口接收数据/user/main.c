#include "stm32f10x.h"

void My_USART_Init(void);
void My_OnBoardLED_Init(void);
int main(void)
{
	
	My_USART_Init();
	My_OnBoardLED_Init();
	while(1)
	{
	//1.等待接收数据寄存器RDR非空
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
	//2.读取数据
	uint8_t byteRcvd=USART_ReceiveData(USART1);
	//3.对数据进行处理
	if(byteRcvd=='0')
	{GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);}//灭灯
	else if(byteRcvd=='1')
	{GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);}//亮灯
	}
}

/**
   * @brief 对USART1进行初始化，PB6-Tx，PB7-Rx
	115200，8，1，None，双向
   * @param	无
   * @retval 无
   */
void My_USART_Init(void)
{
	//1.初始化PB6,PB7
	GPIO_InitTypeDef GPIO_InitStruct;
//	//Tx PA9 复用输出推挽
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	//Rx PA10输入浮空 输入上拉
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
//	GPIO_InitTypeDef GPIO_InitStruct;
	//Tx PB6 输出推挽
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//Rx PB7输入浮空 输入上拉
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	//2.初始化USART串口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART_InitTypeDef USART_InitStruct;

	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;

	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}
void My_OnBoardLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
}

