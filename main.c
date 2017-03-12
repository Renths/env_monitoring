#include "mydefine.h"





void delayms(int num)
{
	uchar i,j,k;
	for(i=0; i<num; i++)
		for(j=0; j<80; j++)
			for(k=0; k<10; k++)
			;
}


/*
只是将温度阻值特性表从code代码中copy到了外部RAM中，只要不做修改
应该是可以省略的
*/
void cp_temperature_tab2ram(void)
{
	uint i;
	for(i=0; i<sizeof(float)*temperature_tab_len; i++)
	{
		XBYTE[TEMP_TAB_ADDR_START + sizeof(float)*i] = temperature_tab[i];
	}
	
}


void read_limit_val_to_ram()
{
	Read_N_Bytes((uchar *)TEMP_LIMIT_ADR,Length*3,TEMP_LIMIT_ROM_ADR);
	
#if 0
	uchar i;
	//读取温度限制值
	for(i=0;i<Length;i++)
	{
		XBYTE[TEMP_LIMIT_ADR+i] = iic_read(BRT_LIMIT_ROM_ADR+2);
	}
	
	//读取亮度
	for(i=0;i<Length;i++)
	{
		XBYTE[BRT_LIMIT_ADR+i] = iic_read(BRT_LIMIT_ROM_ADR+2);
		//XBYTE[BRT_LIMIT_ADR+i] = iic_read(BRT_LIMIT_ROM_ADR+i);
	}
	
	//读取电压
	for(i=0;i<Length;i++)
	{
		XBYTE[VCC_LIMIT_ADR+i] = iic_read(BRT_LIMIT_ROM_ADR+2);
		//XBYTE[VCC_LIMIT_ADR+i] = iic_read(VCC_LIMIT_ROM_ADR+i);
	}
#endif	
}


void save_limit_val_to_rom()
{
	Write_Num_Byte((uchar*)TEMP_LIMIT_ADR,Length*3,TEMP_LIMIT_ROM_ADR);
#if 0
	uchar i;
	//保存温度限制值
	for(i=0; i<Length; i++)
	{
		iic_write(TEMP_LIMIT_ROM_ADR+i, XBYTE[TEMP_LIMIT_ADR+i]);
	}
	
	//保存亮度
	for(i=0; i<Length; i++)
	{
		iic_write(BRT_LIMIT_ROM_ADR+i, XBYTE[BRT_LIMIT_ADR+i]);
	}
	
	//保存电压
	for(i=0; i<Length; i++)
	{
		iic_write(VCC_LIMIT_ROM_ADR+i, XBYTE[VCC_LIMIT_ADR+i]);
	}
#endif	
}



/*
初始化 完成的工作:
1.将代码段数据读到外部ram中 code修饰的字段与程序代码相似为代码段  只读 
2.初始化定时器
定时器 中断等等开关
*/
void init(void)
{
	 EA=0;
	 P1=P3=0XFF;
	 iic_init();

//#if 0	 
	 //手动修改数据  在后面中需要引用变量的方式
	 XBYTE[DISPADR+0]=0X0; //在显示缓冲中 存放的是数码管的段码
	 XBYTE[DISPADR+1]=0X0;	//其他的数据通过段码数据转换
	 XBYTE[DISPADR+2]=0X0;
	 XBYTE[DISPADR+3]=0X0;

	 XBYTE[TEMPADR+0]=0X0;  //在其他的缓冲中主要是放的10进制数据 一个字节放一位数
	 /*XBYTE[TEMPADR+1]=0X3;	//有点浪费
	 XBYTE[TEMPADR+2]=0X3;
	 XBYTE[TEMPADR+3]=0X3;*/

	 XBYTE[BRTADR+0]=0X1;
	 /*XBYTE[BRTADR+1]=0X0;
	 XBYTE[BRTADR+2]=0X0;
	 XBYTE[BRTADR+3]=0X1;*/

	 XBYTE[VCCADR+0]=0X2;
	/* XBYTE[VCCADR+1]=0X4;
	 XBYTE[VCCADR+2]=0X9;
	 XBYTE[VCCADR+3]=0X5;*/

	 //初始化各参数报警门限  ---在后期要求在EEROM中读取
	 XBYTE[TEMP_LIMIT_ADR+0]=0X0;  //在其他的缓冲中主要是放的10进制数据 一个字节放一位数
	 /*XBYTE[TEMP_LIMIT_ADR+1]=0X4;	//有点浪费
	 XBYTE[TEMP_LIMIT_ADR+2]=0X0;
	 XBYTE[TEMP_LIMIT_ADR+3]=0X0;*/

	 XBYTE[BRT_LIMIT_ADR+0]=0X1;
	 /*XBYTE[BRT_LIMIT_ADR+1]=0X0;
	 XBYTE[BRT_LIMIT_ADR+2]=0X0;
	 XBYTE[BRT_LIMIT_ADR+3]=0X5;*/

	 XBYTE[VCC_LIMIT_ADR+0]=0X2;
	 /*XBYTE[VCC_LIMIT_ADR+1]=0X5;
	 XBYTE[VCC_LIMIT_ADR+2]=0X0;
	 XBYTE[VCC_LIMIT_ADR+3]=0X0;	*/
//#endif
	 read_limit_val_to_ram();
	 cp_temperature_tab2ram();
	 
	 TMOD=0X01;
	 SET_HTIME0_PARA(FOSC,10)
	 ET0=1;PT0=0;TR0=1;
	 EA=1;
	 

	 //开外部中断1 用于按键中断
	 EX1 = 1;
	 IT1 = 1; // 1下降沿触发 0边沿触发
}

/*有源蜂鸣器
*/
void speaker(uchar state,uchar type)
{
	speaker_state = state;
	speaker_type = type;
			
}


/*函数说明
dis_mod 用来控制选择显示温度 亮度 还是 电压
flash_postion 闪烁位置 取值范围为0~4
	0 不闪烁
	1~4对应位置闪烁
*/
void display_flash(uchar dis_mod,uchar flash_postion)
{
	uchar i,j,temp;
	/*    选择显示数据位 读  0温度位? 1亮度位?  2还是电压位?
	外部RAM地址	0~3          4~7       8~11     12~15
	宏定义的地址DISPADR	   TEMPADR   BRTADR    VCCADR
	这个选择由dismod决定
	*/

	//检查闪烁位置 如果不在范围内 不予显示  否则地址会指向其他地方造成段错误
	if(flash_postion <0 || flash_postion > 4)
		flash_postion = 0;
	
	switch(dis_mod) 
	{
		case temperature:
		/*处理温度数据*/
			XBYTE[DISPADR+0]=chartab[XBYTE[TEMP_LIMIT_ADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[TEMP_LIMIT_ADR+1]];
			XBYTE[DISPADR+2]=digital_dot[XBYTE[TEMP_LIMIT_ADR+2]];
		 	XBYTE[DISPADR+3]=digital[XBYTE[TEMP_LIMIT_ADR+3]];
			if(1 == B20ms && flash_postion)
				XBYTE[DISPADR+flash_postion]=digital[10]; //灭
		break;
		case light:
		/*处理亮度数据*/
			XBYTE[DISPADR+0]=chartab[XBYTE[BRT_LIMIT_ADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[BRT_LIMIT_ADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[BRT_LIMIT_ADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[BRT_LIMIT_ADR+3]];
			if(1 == B20ms && flash_postion)
				XBYTE[DISPADR+flash_postion]=digital[10]; //灭
		break;
		default :
		/*处理电压数据*/
			XBYTE[DISPADR+0]=chartab[XBYTE[VCC_LIMIT_ADR+0]];
			XBYTE[DISPADR+1]=digital_dot[XBYTE[VCC_LIMIT_ADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[VCC_LIMIT_ADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[VCC_LIMIT_ADR+3]];
			if(1 == B20ms && flash_postion)
				XBYTE[DISPADR+flash_postion]=digital[10]; //灭
		break;
		
	}

	//将串行数据送入到 串转并 
	for(i=0; i<Length; i++)
	{
		temp=XBYTE[DISPADR+i];
		for(j=0; j<8; j++)
		{
			CLK=0;CLK=1;
		}
	}
	ST=1;ST=0; //给一个下降沿信号 4个串转并同时锁存 完成转换
	
}



#if 0
/*函数说明
dis_mod 用来控制选择显示温度 亮度 还是 电压
*/
void display(const uchar dis_mod)
{
	uchar i,j,temp;
	/*    选择显示数据位 读  0温度位? 1亮度位?  2还是电压位?
	外部RAM地址	0~3          4~7       8~11     12~15
	宏定义的地址DISPADR	   TEMPADR   BRTADR    VCCADR
	这个选择由dismod决定
	*/
	switch(dis_mod) 
	{
		case temperature:
		/*处理温度数据*/
			XBYTE[DISPADR+0]=chartab[XBYTE[TEMPADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[TEMPADR+1]];
			XBYTE[DISPADR+2]=digital_dot[XBYTE[TEMPADR+2]];
		 	XBYTE[DISPADR+3]=digital[XBYTE[TEMPADR+3]];
			break;
		case light:
		/*处理亮度数据*/
			XBYTE[DISPADR+0]=chartab[XBYTE[BRTADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[BRTADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[BRTADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[BRTADR+3]];
			break;
		case voltage :
		/*处理电压数据*/
			XBYTE[DISPADR+0]=chartab[XBYTE[VCCADR+0]];
			XBYTE[DISPADR+1]=digital_dot[XBYTE[VCCADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[VCCADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[VCCADR+3]];
			break;
		default: 
			XBYTE[DISPADR+0]=digital[10]; //灭
			XBYTE[DISPADR+1]=digital[10]; //灭
			XBYTE[DISPADR+2]=digital[10]; //灭
			XBYTE[DISPADR+3]=digital[10]; //灭
			break;
		
	}

	//将串行数据送入到 串转并 
	for(i=0;i<Length;i++)
	{
		temp=XBYTE[DISPADR+i];
		for(j=0;j<8;j++)
		{
			CLK=0;CLK=1;
		}
	}
	ST=1;ST=0; //给一个下降沿信号 4个串转并同时锁存 完成转换
	
}
#endif

void display(const uchar dis_mode )
{
	uchar i,j,temp;
	switch(dis_mode)
	{
		case 0:
			XBYTE[DISPADR+0]=chartab[XBYTE[TEMPADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[TEMPADR+1]];
			XBYTE[DISPADR+2]=digital_dot[XBYTE[TEMPADR+2]];
		 	XBYTE[DISPADR+3]=digital[XBYTE[TEMPADR+3]];
			break;
		case 1:
			XBYTE[DISPADR+0]=chartab[XBYTE[BRTADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[BRTADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[BRTADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[BRTADR+3]];
			break;
		default:
			XBYTE[DISPADR+0]=chartab[XBYTE[VCCADR+0]];
			XBYTE[DISPADR+1]=digital_dot[XBYTE[VCCADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[VCCADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[VCCADR+3]];
			break;
	}

	//将串行数据送入到 串转并 
	for(i=0;i<Length;i++)
	{
		temp=XBYTE[DISPADR+i];
		for(j=0;j<8;j++)
		{
			CLK=0;CLK=1;
		}
	}
	ST=1;ST=0; //给一个下降沿信号 4个串转并同时锁存 完成转换
}



//功能 判断检测的值是否有大于 设置好了的限制值
void monitor(void)
{
	//读数据
	uint temper_current,light_current,voltage_currnt;
	uint temper_limit,light_limit,voltage_limit;
	temper_current = GET_RAM_VLA(TEMPADR);
	light_current = GET_RAM_VLA(BRTADR);
	voltage_currnt = GET_RAM_VLA(VCCADR);

	temper_limit = GET_RAM_VLA(TEMP_LIMIT_ADR);
	light_limit = GET_RAM_VLA(BRT_LIMIT_ADR);
	voltage_limit = GET_RAM_VLA(VCC_LIMIT_ADR);

	//比较数据 注意 数据以在原来的基础上放大了10倍 去掉了小数点后一位
	if((temper_current >= temper_limit)  
		|| (light_current >= light_limit)
		|| (voltage_currnt >= voltage_limit))
	{
		if(temper_current >= temper_limit) speaker(on,temperature);
		else if(light_current >= light_limit) speaker(on,light);
		else if(voltage_currnt >= voltage_limit) speaker(on,voltage);
	}
	else speaker(off,0);
	
}


uchar check_table(const float *temp)
{
	uchar i;
	uint retval;
	for(i=0; i<sizeof(float)*temperature_tab_len; i++)
	{
		if(*temp >= temperature_tab[i])
		{
			retval = i*5 + 
				( (*temp - temperature_tab[i])/ //分子
				(temperature_tab[i]-temperature_tab[i+1])/*分母*/)*5; /*中间占比部分*/
			//本来是0.5 放大了10倍 返回的值也是放大了10倍
			//比如 实际值为25.5度  但是返回了255
			break;
		}
		
	}
	return retval;
	
}
void read_adc(uchar select)
{
	uchar temp;
	uint changed_val;
	double Vres15,Ires15,Res14;
	temp =adc(select);
	//将ad数值转换成物理单位参考值 精确度一点都不高
	switch(select)
	{
		case temperature:
			//求 R14电阻
			Vres15 = (5.0/256.0) * temp;
			Ires15 = Vres15 /(10.0*1000.0);
			Res14 = (5.0-Vres15) / Ires15;
			Res14 = Res14/1000.0;
			//查表
			changed_val = check_table(&Res14);
			break;
		case light://要求0~9个亮度级别 实际测量最大ad值193 最小ad值143 每5个变换值表示一个亮度级别。
			changed_val = (temp-143)/5;
			break;
		case voltage://假设传感器为线性 参考电压为5V ad在0~255变换
			changed_val = (500/255) * temp; //放大了100倍对应显示小数点后两位
			break;
	}
	XBYTE[TEMPADR+select*Length +1] = changed_val/100;
	XBYTE[TEMPADR+select*Length +2] = (changed_val/10)%10;
	XBYTE[TEMPADR+select*Length +3] = (changed_val%100)%10;
}
int main(void)
{
	volatile uchar temp_conut = 0;
	init();	
	while(1)
	{
		
		//正常显示
		if(no==modify_falg)
		{
			read_adc(temperature);
			monitor();//监控值是否超过变化 
			display(temperature);
			while(0 != time_cnt) //只是起一个延时1s作用
				if(yes == modify_falg) goto flash;
			time_cnt++;//主动加一 
			
			
			read_adc(light);
			monitor();//监控值是否超过变化 
			display(light);
			while(0 != time_cnt)
				if(yes == modify_falg) goto flash;
			time_cnt++;
			

			read_adc(voltage);
			monitor();//监控值是否超过变化 
			display(voltage);
			while(0 != time_cnt)
				if(yes == modify_falg) goto flash;
			time_cnt++;
			
			
			#if 0  // 已解决
			if(0 == B1s)
			{	
				B1s = ~B1s;
				delayms(1);//短暂延时 减缓CPU负载
				read_adc(temp_conut);
				monitor();//监控值是否超过变化 
				//没过1s循环显示数据 并检查是否超过限制
				display(temp_conut);	
				temp_conut++;
				if(temp_conut > 3) temp_conut = 0;
				
			}
			#endif
		}
		else
		{
flash:
			//闪烁修改值
			speaker(off,0);
			display_flash(mod_key_cnt%3,postion_cnt%3+1);
			
		}
		if(yes == save_flag )
		{
			save_flag =no;
			save_limit_val_to_rom();
		}
		
	}
	return 0;
}	
