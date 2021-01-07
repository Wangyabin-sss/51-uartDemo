#include "reg51.h"
#include "stdio.h"



#define S_RECEVIER_SIZE  64
unsigned char RevBuffer[S_RECEVIER_SIZE];
int revDataCount = 0;
int revTempLength = 0;


void uart_init(void)  
{  
		SCON=0x50; //0101 0000.���ڹ�����ʽ1�������п��� 
    TMOD=0x20;//��0010 0000,��ʱ��/������1,������ʽ2  
    TH1=0xFD;//���ò�����Ϊ9600  
    TL1=0xFD;  
    TR1=1;//������ʱ��/������1  

    ES=1;
		EA=1;

    TI=1;//ֱ��ʹ��printf�������˾����ʵ�ַ���  
//    RI=1;
}  

void U1SendData(unsigned char ch)
{
		
    SBUF = ch;                  //д���ݵ�UART���ݼĴ���
		while(TI == 0);             //��ֹͣλû�з���ʱ��TIΪ0��һֱ�ȴ�
		TI = 0;  	//���TIλ����λ����������㣩
		
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


 void Uart_Isr() interrupt 4 using 1//�����ж�
{
    unsigned char temp; //��ʱ���������ڻ����շ�����

    if(RI)  // �ж��Ǵ��ڽ��ղ����ж�
    {
        RI = 0;       // ������жϱ�־
        temp = SBUF;  // ���յ�������д�뻺��BUF
        //SBUF = temp;  // ���յ������ݷ��ظ����Զ�

        if (temp == 0x0D0A) // �ԡ�\n����Ϊ�����ַ���������־ 
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
		
//		if (TI)    // �ж��Ǵ��ڷ��Ͳ����ж�
//		{
//			TI = 0; // �巢���ж�
//		} 
		
	}		




























