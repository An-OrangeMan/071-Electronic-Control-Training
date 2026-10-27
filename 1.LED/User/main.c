#include "stm32f10x.h"                  // 包含 STM32F10x 标准外设库的头文件

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 1. 开启 GPIOC 端口的时钟,STM32 的外设都需要先开启时钟才能使用，这里使能 APB2 总线上的 GPIOC
	GPIO_InitTypeDef GPIO_InitStructure;                  // 2. 定义 GPIO 初始化结构体变量
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // 3. 配置 GPIO 的模式,GPIO_Mode_Out_PP 表示推挽输出模式 (Push-Pull Output)，可以输出强高低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            // 4. 配置PC13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // 5. 配置 GPIO 的响应速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);                // 6. 调用初始化函数，把结构体里的参数写入到 GPIOC 的寄存器中
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);                   //低电平点亮
	while (1)
	{
		
	}
}
