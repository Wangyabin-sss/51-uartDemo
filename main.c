#include "reg51.h"
#include "stdio.h"



#define S_RECEVIER_SIZE  64
unsigned char RevBuffer[S_RECEVIER_SIZE];
int revDataCount = 0;
int revTempLength = 0;


void uart_init(void)  
{  
		SCON=0x50; //0101 0000.串口工作方式1，允许串行控制 
    TMOD=0x20;//即0010 0000,定时器/计数器1,工作方式2  
    TH1=0xFD;//设置波特率为9600  
    TL1=0xFD;  
    TR1=1;//启动定时器/计数器1  

    ES=1;
		EA=1;

    TI=1;//直接使用printf必须加入此句才能实现发送  
//    RI=1;
}  

void U1SendData(unsigned char ch)
{
		
    SBUF = ch;                  //写数据到UART数据寄存器
		while(TI == 0);             //在停止位没有发送时，TI为0即一直等待
		TI = 0;  	//清除TI位（该位必须软件清零）
		
}

void UartSendStr(unsigned char *p)
{
	while (*p)
	{
		U1SendData(*p++);
	}
}


char putchar(char c)
{
	U1SendData(c);
	return c;
}
	

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 160;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

void main()
{
	int i=5;
	uart_init();
	while(1)
	{		
//		for(i=0;i<revTempLength;i++)
//		{
//			U1SendData(RevBuffer[i]);
//		}
//			U1SendData('\r');
//			U1SendData('\n');
//			UartSendStr(RevBuffer);
//			U1SendData('4');
			printf("%s",RevBuffer);
			U1SendData('\r');
			U1SendData('\n');
			

			Delay100ms();
	}
}


 void Uart_Isr() interrupt 4 using 1//串口中断
{
    unsigned char temp; //临时变量，用于缓冲收发数据

    if(RI)  // 判断是串口接收产生中断
    {
        RI = 0;       // 清接收中断标志
        temp = SBUF;  // 接收到的数据写入缓冲BUF
        //SBUF = temp;  // 将收到的数据发回给电脑端

        if (temp == 0x0D0A) // 以‘\n’做为接收字符串结束标志 
        {
						revTempLength = revDataCount;
						for(revTempLength;revTempLength<S_RECEVIER_SIZE;revTempLength++)
						{
							RevBuffer[revTempLength]=0;
						}
            revDataCount = 0;
        } 
        else
        {     
						RevBuffer[revDataCount] = temp;
            revDataCount++;
        } 
    }
		
//		if (TI)    // 判断是串口发送产生中断
//		{
//			TI = 0; // 清发送中断
//		} 
		
	}		




























