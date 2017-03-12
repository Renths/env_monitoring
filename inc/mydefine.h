#ifndef _MYDEFINE_H_
#define _MYDEFINE_H_

#include<reg52.h>
#include<absacc.h>
#include<math.h>
#include "intrins.h"


typedef unsigned char 	uchar; //
typedef unsigned int	uint ; //2个字节
typedef unsigned long 	ulong ;

//常量定义
#define FOSC (22.1184)
#define Length 4  //数码管位数
//#define 

/* 位寄存器*///SDAat
sbit SCL =	  P1^0;
sbit SDA = 	  P1^1;
sbit AD_CS =  P1^2;
sbit AD_CLK = P1^3;
sbit AD_DO =  P1^4;
sbit ST = 	  P1^5;
sbit CLK = 	  P1^6;
sbit KEYSIG	= P3^3;

sbit SPEAKER= P3^4;

sbit AD_SET_IN_A = P1^7;
sbit AD_SET_IN_B = P3^5;


//iic device
#define iic_addr_reaed 0xa1
#define iic_addr_write 0xa0



//display mode control
#define true 			1
#define false 			0

#define temperature 0
#define light 		1
#define voltage		2

#define yes 1
#define no 0
#define on 1
#define off 0

#define temperature_tab_len 80
/*
在外部ram中使用规定:	
0~3		存放显示数据 dispadr
4~7 	存放温度数据 tempadr
8~11	存放亮度数据 breadr
12~15 	存放电压数据 vccadr
在后面对变量中，读到的数据都放在这几个固定的段
*/

#define DISPADR 0 
#define TEMPADR DISPADR+Length
#define BRTADR TEMPADR+Length
#define VCCADR BRTADR+Length

/*保存报警门限的RAM地址*/  //预定大小也是4个字节   但是远远够了
#define TEMP_LIMIT_ADR VCCADR+Length+4
#define BRT_LIMIT_ADR TEMP_LIMIT_ADR+Length
#define VCC_LIMIT_ADR BRT_LIMIT_ADR+Length

/*保存报警门限的ROM地址*/
#define TEMP_LIMIT_ROM_ADR 0x00
#define BRT_LIMIT_ROM_ADR 0x20 //TEMP_LIMIT_ROM_ADR+Length
#define VCC_LIMIT_ROM_ADR 0x40 //BRT_LIMIT_ROM_ADR+Length



//温度表段
#define TEMP_TAB_ADDR_START VCC_LIMIT_ADR+Length
#define TEMP_TAB_ADDR_END TEMP_TAB_ADDR_START + sizeof(float)*temperature_tab_len


#define	SET_HTIME0_PARA(fosc/*uint is MHZ*/,time/*unit is ms*/)\
		TL0 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))%0x100;	\
		TH0 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))/0x100;


//由于数据类型限制  现在将单精度数据比较  转换成 整形数据比较  
//这里的办法是将数据放大10倍比较
#define GET_RAM_VLA(addr) (XBYTE[addr+1]*100 + XBYTE[addr+2]*10 + XBYTE[addr+3])


/*code数据*/
extern uchar code digital[];
extern uchar code digital_dot[];
extern uchar code chartab[];
extern float code temperature_tab[];


//外部变量声明

extern uchar mod_key_cnt;
extern uchar postion_cnt;
extern uchar modify_falg;


extern uchar time_cnt;
extern bit B1s;
extern bit B20ms;
extern uchar second;
extern uchar speaker_type;
extern uchar speaker_state;
extern uchar save_flag;



//外部函数声明
extern void display(uchar dis_mod);
extern void display_flash(uchar dis_mod,uchar flash_postion);
extern void speaker(uchar state,uchar type);
extern void delayms(int num);
extern uchar adc(uchar select);
extern void my_nop(void);
extern uchar iic_read(uchar address);
extern void iic_write(uchar address,uchar info);
extern void save_limit_val_to_rom();
extern void iic_init(void);
extern Write_Num_Byte(uchar * buffer,uchar cnt,uchar addr);
extern void Read_N_Bytes(uchar *buffer,uchar n,uchar addr);


#endif



#if 0
#ifndef	NEW_TYPE_DEFINE_HEADER_FILE
	#define	NEW_TYPE_DEFINE_HEADER_FILE

	typedef unsigned char 	uchar;
	typedef unsigned int 	uint;
	typedef unsigned long 	ulong;

	

	#define	BYTE_OF_VAL(val, index)		*((uchar *)&val + index)
	#define	HI_BYTE_OF_WORD(val)		BYTE_OF_VAL(val,0)
	#define	LO_BYTE_OF_WORD(val)		BYTE_OF_VAL(val,1)
	#define	HI_BYTE_OF_LONG(val)		BYTE_OF_VAL(val,0)
	#define	BYTE2ND_OF_LONG(val)		BYTE_OF_VAL(val,1)
	#define BYTE3TH_OF_LONG(val)		BYTE_OF_VAL(val,2)
	#define LO_BYTE_OF_LONG(val)		BYTE_OF_VAL(val,3)

	#define HI_WORD_OF_LONG(val)		*((uint *)&val)
	#define LO_WORD_OF_LONG(val)		*((uint *)&val +1)
        #define	SET_TIME2_PARA(fosc/*uint is MHZ*/,time/*unit is ms*/)\
		T2CON = 0;\
		RCAP2L 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))%0x100;	\
		RCAP2H 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))/0x100;
        #define	SET_HTIME0_PARA(fosc/*uint is MHZ*/,time/*unit is ms*/)\
		TL0 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))%0x100;	\
		TH0 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))/0x100;
        #define	SET_ATIME0_PARA(fosc/*uint is MHZ*/,time/*unit is ms*/)\
		TL0 	= (uint)(0x100 - (float)(time*1000)/((float)12/fosc))%0x100;	\
		TH0 	= (uint)(0x100 - (float)(time*1000)/((float)12/fosc))/0x100;
        #define	SET_HTIME1_PARA(fosc/*uint is MHZ*/,time/*unit is ms*/)\
		TL1 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))%0x100;	\
		TH1 	= (uint)(0x10000 - (float)(time*1000)/((float)12/fosc))/0x100;
        #define	SET_ATIME1_PARA(fosc/*uint is MHZ*/,time/*unit is ms*/)\
		TL1 	= (uint)(0x100 - (float)(time*1000)/((float)12/fosc))%0x100;	\
		TH1 	= (uint)(0x100 - (float)(time*1000)/((float)12/fosc))/0x100;
        #define SET_T1SBAUD(fosc,baud) \
        PCON=0;\
        TH1=TL1=(uchar)((float)256-(float)fosc*1000000/((float)32*baud*12));
        #define SET_T1DBAUD(fosc,baud) \
        PCON=0X80;\
        TH1=TL1=(uchar)((float)256-(float)fosc*1000000/((float)16*baud*12));
        #define SET_T2BAUD(fosc,baud) \
        PCON=0;\
        RCAP2L 	=(uint) ((float)65536 - (float)(fosc*1000000)/32/baud)%0x100;  \
	RCAP2H 	=(uint) ((float)65536 - (float)(fosc*1000000)/32/baud)/0x100;  \
        TH2=RCAP2H;TL2=RCAP2L;
#endif
#endif



