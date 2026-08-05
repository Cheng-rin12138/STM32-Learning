#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
void My_USART_Init(void);
void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size);
int fputc(int ch,FILE*f);
int main(void)
{
	Delay_Init();
	My_USART_Init();
	while(1)
{
	uint32_t currentTick=GetTick();
	uint32_t miliseconds=currentTick%1000;currentTick/=1000;
	uint32_t seconds=currentTick%60;currentTick/=60;
	uint32_t minute=currentTick%60;currentTick/=60;
	uint32_t hour=currentTick;
	printf("%02u:%02u:%02u.%03u\r\n",hour,minute,seconds,miliseconds);
	Delay(100);//每100ms打印一次
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
/**
   * @brief 使用串口一次性发送多个字节
* @param	USARTx填写串口名称，pData要发送的数据，Size字节的数量
   * @retval 无
   */
void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size)
{
	for(uint32_t i=0;i<Size;i++)
	{
		//1.等待发送数据寄存器为空
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		//2.写入发数据寄存器中
		USART_SendData(USARTx,pData[i]);
	}
	//等待发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}
int fputc(int ch,FILE*f)
{
	//1.等待TDR为空
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	//2.发送
	USART_SendData(USART1,(uint8_t)ch);
	return ch;
}
