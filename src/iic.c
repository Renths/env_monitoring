#include"mydefine.h"

//iic设备 两条线外接上啦电阻  
//空闲状态 1
//忙状态   0
//只有在空闲状态是读取或者写入数据

void I2CDelay(uint i) /*延时函数*/
{
    while(i!=0)
        i--;
}

void iic_init(void)              //初始化
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


bit iic_respons(void)             //回应信号
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

void SEND_0(void )   /* SEND ACK *//*发送0，在SCL为高电平时使SDA信号为低*/
{
	SDA=0;
	SCL=1;
	I2CDelay(60);
	SCL=0;
	I2CDelay(60);
}

void SEND_1(void) /*发送1，在SCL为高电平时使SDA信号为高*/
{
	SDA=1;
	SCL=1;
	I2CDelay(60);
	SCL=0;
	I2CDelay(60);
}

/*
void iic_no_respons() //非应答
{
	SCL = 1;//在scl为高电平期间，由主机向从机发送一个1，非应答信号
	my_nop();
	SDA = 1;
	SCL = 0; //应答之后将scl拉低
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
		SDA=1;	  /*释放总线*/
		SCL=1;	  /*接受数据*/
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
//指定地址写一个字节数据
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
//指定地址读一个字节数据
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

/*写n个字节的数据到指定地址开始的单元中*/
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
/*读n个单元的数据*/
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
                    SEND_0();    /*发送应答*/
                else
                    SEND_1();    /*发送非应答*/
          }

          iic_stop();
}

