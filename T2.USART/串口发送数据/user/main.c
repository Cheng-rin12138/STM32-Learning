#include "stm32f10x.h"
/**
   * @brief 使用串口一次性发送多个字节
   * @param	pData要发送的数据，Size字节的数量
   * @retval 无
   */
void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size)
{
	for(uint32_t i=0;i<Size;i++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		USART_SendData(USARTx,pData[i]);
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART_InitTypeDef USART_InitStruct;

	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;

	USART_Init(USART1,&USART_InitStruct);

	USART_Cmd(USART1,ENABLE);
	
//	GPIO_InitTypeDef GPIO_InitStruct;
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
	GPIO_InitTypeDef GPIO_InitStruct;
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
	
	USART_Cmd(USART1,ENABLE);
	
	uint8_t bytesToSend[]={1,2,3,4,5};
	My_USART_SendBytes(USART1,bytesToSend,5);
	while(1)
	{
	}
}
