#include"mydefine.h"

//iic�豸 �����������������  
//����״̬ 1
//æ״̬   0
//ֻ���ڿ���״̬�Ƕ�ȡ����д������

void I2CDelay(uint i) /*��ʱ����*/
{
    while(i!=0)
        i--;
}

void iic_init(void)              //��ʼ��
{
       SCL=1;
       my_nop();
       SDA=1;
       my_nop();
}

void iic_start(void)
{
	SDA=1;
	SCL=1;
	I2CDelay(60);
	SDA=0;
	I2CDelay(60);
	SCL=0;
	I2CDelay(60);

	
}

void iic_stop(void)
{
	SDA=0;
	SCL=1;
	I2CDelay(60);
	SDA=1;
	I2CDelay(60);
	SCL=0;
	I2CDelay(60);

}


bit iic_respons(void)             //��Ӧ�ź�
{
	bit Ack_Flag;
	SDA=1;
	SCL=1;
	I2CDelay(30);
	Ack_Flag=~SDA;
	I2CDelay(30);
	SCL=0;
	I2CDelay(60);
	return Ack_Flag;

}

void SEND_0(void )   /* SEND ACK *//*����0����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
{
	SDA=0;
	SCL=1;
	I2CDelay(60);
	SCL=0;
	I2CDelay(60);
}

void SEND_1(void) /*����1����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
{
	SDA=1;
	SCL=1;
	I2CDelay(60);
	SCL=0;
	I2CDelay(60);
}

/*
void iic_no_respons() //��Ӧ��
{
	SCL = 1;//��sclΪ�ߵ�ƽ�ڼ䣬��������ӻ�����һ��1����Ӧ���ź�
	my_nop();
	SDA = 1;
	SCL = 0; //Ӧ��֮��scl����
	my_nop();
}
*/

void iic_write_byte(uchar x)
{
	uchar i;
	for(i=0;i<8;i++)
		if((x<<i)&0x80)
			SEND_1();
		else
			SEND_0();

	
}

uchar iic_read_byte(void)
{
	uchar x=0,i;
	bit Ack_Flag;
	for(i=0;i<8;i++)
	{
		SDA=1;	  /*�ͷ�����*/
		SCL=1;	  /*��������*/
		I2CDelay(10);
		Ack_Flag=SDA;
		I2CDelay(10);
		SCL=0;
		if(Ack_Flag)
			{
				x=x<<1;
				x=x|0x01;
			}
		else
			x=x<<1;
	}
	return x;

}

void iic_write(uchar addr,uchar info)
//ָ����ַдһ���ֽ�����
{
	bit acktemp=1;
	iic_start();
	iic_write_byte(0xa0);
	
	acktemp=iic_respons();
	
	iic_write_byte(addr);/*address*/
	acktemp=iic_respons();
	
	iic_write_byte(info);/*data*/
	acktemp=iic_respons();
	iic_stop();

}


uchar iic_read(uchar addr)
//ָ����ַ��һ���ֽ�����
{
	bit acktemp=1;
	char mydata;
	 /*read a byte from mem*/
	iic_start();
	iic_write_byte(0xa0);
	acktemp=iic_respons();
	
	iic_write_byte(addr);/*address*/
	acktemp=iic_respons();
	
	iic_start();
	iic_write_byte(0xa1);
	acktemp=iic_respons();
	
	mydata=iic_read_byte();
	acktemp=iic_respons();
	
	iic_stop();
	return mydata;

}

/*дn���ֽڵ����ݵ�ָ����ַ��ʼ�ĵ�Ԫ��*/
Write_Num_Byte(uchar *buffer,uchar cnt,uchar addr)
{
          bit acktemp=1; uchar i;
          iic_start();
          iic_write_byte(0xa0);
          acktemp=iic_respons();
          iic_write_byte(addr);/*address*/
          acktemp=iic_respons();
          for(i=0;i<cnt;i++)
          {
            iic_write_byte(buffer[i]);
            if(!iic_respons())   iic_stop();                
           }
          iic_stop();
}
/*��n����Ԫ������*/
void Read_N_Bytes(uchar *buffer,uchar n,uchar addr)
{
          bit acktemp=1;

          int i=0;
           /*read 8 bytes from mem*/
          iic_start();
          iic_write_byte(0xa0);
          acktemp=iic_respons();
          iic_write_byte(addr);/*address*/
          acktemp=iic_respons();
          iic_start();
          iic_write_byte(0xa1);
          acktemp=iic_respons();
          for(i=0;i<n;i++)
          {
                buffer[i]=iic_read_byte();
                if(i!=n-1)
                    SEND_0();    /*����Ӧ��*/
                else
                    SEND_1();    /*���ͷ�Ӧ��*/
          }

          iic_stop();
}

