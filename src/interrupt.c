#include"mydefine.h"
/*
中断0的中断号是0，定时器0的中断号是1，
外部中断1的中断号是2，定时器1的中断号是3

*/
timer0_interrupt() interrupt 1 using 1   //定时器0 的入口编号
{
	SET_HTIME0_PARA(FOSC,10)
	time_cnt++;
	if(0 == time_cnt%20)
		B20ms = ~B20ms;
	if(100 == time_cnt)
	{
		time_cnt=0;
		B1s=~B1s;
		second++;		
	}


	//-------处理扬声器
	if(off == speaker_state) 
	{
		SPEAKER = 1;
		return; //off状态提前返回
	}
	switch(speaker_type)
	{
		
		case temperature:
				if(0 == time_cnt%20)	SPEAKER = ~SPEAKER;
			break;
		case light:
				if(0 == time_cnt%60)	SPEAKER = ~SPEAKER;
			break;
		case voltage:
				if(0 == time_cnt)	SPEAKER = ~SPEAKER;
			break;
		default: break;
	}
}

key_interrut() interrupt 2 using 2 
{
	uchar temp,temp_data;
	if(!KEYSIG)
	{	
		delayms(20);
		if(!KEYSIG)
		{
			temp = XBYTE[0x8000]; //d15 为高
			switch(temp & 0x0f)
			{
				case 0x0e: //MODKEY
					mod_key_cnt++;
					modify_falg = yes; //修改修改标记
					speaker(off,0);
					break;
					
				case 0x0d:	//RLKEY
					if(no == modify_falg) break; //没有修改标志什么都不做
					postion_cnt++;
					break;
					
				case 0x0b:	//ASKEY
					if(no == modify_falg) break; //没有修改标志什么都不做
					temp_data = XBYTE[TEMP_LIMIT_ADR + (mod_key_cnt%3)*Length +postion_cnt%3 + 1];
					if( ++temp_data > 9)
						temp_data = 0;
					XBYTE[TEMP_LIMIT_ADR + (mod_key_cnt%3)*Length + postion_cnt%3 + 1] = temp_data;						
					break;
					
				case 0x07: //OKKEY
					mod_key_cnt = 0;
					postion_cnt = 0;
					modify_falg = no;//关闭修改标记	
					save_flag = yes;
					//save_limit_val_to_rom();
					break;
					
				default: break; //可能会有其他值 但是什么都不做为好
					
			}

		}		
	}		
}


