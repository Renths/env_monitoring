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
ֻ�ǽ��¶���ֵ���Ա��code������copy�����ⲿRAM�У�ֻҪ�����޸�
Ӧ���ǿ���ʡ�Ե�
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
	//��ȡ�¶�����ֵ
	for(i=0;i<Length;i++)
	{
		XBYTE[TEMP_LIMIT_ADR+i] = iic_read(BRT_LIMIT_ROM_ADR+2);
	}
	
	//��ȡ����
	for(i=0;i<Length;i++)
	{
		XBYTE[BRT_LIMIT_ADR+i] = iic_read(BRT_LIMIT_ROM_ADR+2);
		//XBYTE[BRT_LIMIT_ADR+i] = iic_read(BRT_LIMIT_ROM_ADR+i);
	}
	
	//��ȡ��ѹ
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
	//�����¶�����ֵ
	for(i=0; i<Length; i++)
	{
		iic_write(TEMP_LIMIT_ROM_ADR+i, XBYTE[TEMP_LIMIT_ADR+i]);
	}
	
	//��������
	for(i=0; i<Length; i++)
	{
		iic_write(BRT_LIMIT_ROM_ADR+i, XBYTE[BRT_LIMIT_ADR+i]);
	}
	
	//�����ѹ
	for(i=0; i<Length; i++)
	{
		iic_write(VCC_LIMIT_ROM_ADR+i, XBYTE[VCC_LIMIT_ADR+i]);
	}
#endif	
}



/*
��ʼ�� ��ɵĹ���:
1.����������ݶ����ⲿram�� code���ε��ֶ�������������Ϊ�����  ֻ�� 
2.��ʼ����ʱ��
��ʱ�� �жϵȵȿ���
*/
void init(void)
{
	 EA=0;
	 P1=P3=0XFF;
	 iic_init();

//#if 0	 
	 //�ֶ��޸�����  �ں�������Ҫ���ñ����ķ�ʽ
	 XBYTE[DISPADR+0]=0X0; //����ʾ������ ��ŵ�������ܵĶ���
	 XBYTE[DISPADR+1]=0X0;	//����������ͨ����������ת��
	 XBYTE[DISPADR+2]=0X0;
	 XBYTE[DISPADR+3]=0X0;

	 XBYTE[TEMPADR+0]=0X0;  //�������Ļ�������Ҫ�Ƿŵ�10�������� һ���ֽڷ�һλ��
	 /*XBYTE[TEMPADR+1]=0X3;	//�е��˷�
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

	 //��ʼ����������������  ---�ں���Ҫ����EEROM�ж�ȡ
	 XBYTE[TEMP_LIMIT_ADR+0]=0X0;  //�������Ļ�������Ҫ�Ƿŵ�10�������� һ���ֽڷ�һλ��
	 /*XBYTE[TEMP_LIMIT_ADR+1]=0X4;	//�е��˷�
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
	 

	 //���ⲿ�ж�1 ���ڰ����ж�
	 EX1 = 1;
	 IT1 = 1; // 1�½��ش��� 0���ش���
}

/*��Դ������
*/
void speaker(uchar state,uchar type)
{
	speaker_state = state;
	speaker_type = type;
			
}


/*����˵��
dis_mod ��������ѡ����ʾ�¶� ���� ���� ��ѹ
flash_postion ��˸λ�� ȡֵ��ΧΪ0~4
	0 ����˸
	1~4��Ӧλ����˸
*/
void display_flash(uchar dis_mod,uchar flash_postion)
{
	uchar i,j,temp;
	/*    ѡ����ʾ����λ ��  0�¶�λ? 1����λ?  2���ǵ�ѹλ?
	�ⲿRAM��ַ	0~3          4~7       8~11     12~15
	�궨��ĵ�ַDISPADR	   TEMPADR   BRTADR    VCCADR
	���ѡ����dismod����
	*/

	//�����˸λ�� ������ڷ�Χ�� ������ʾ  �����ַ��ָ�������ط���ɶδ���
	if(flash_postion <0 || flash_postion > 4)
		flash_postion = 0;
	
	switch(dis_mod) 
	{
		case temperature:
		/*�����¶�����*/
			XBYTE[DISPADR+0]=chartab[XBYTE[TEMP_LIMIT_ADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[TEMP_LIMIT_ADR+1]];
			XBYTE[DISPADR+2]=digital_dot[XBYTE[TEMP_LIMIT_ADR+2]];
		 	XBYTE[DISPADR+3]=digital[XBYTE[TEMP_LIMIT_ADR+3]];
			if(1 == B20ms && flash_postion)
				XBYTE[DISPADR+flash_postion]=digital[10]; //��
		break;
		case light:
		/*������������*/
			XBYTE[DISPADR+0]=chartab[XBYTE[BRT_LIMIT_ADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[BRT_LIMIT_ADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[BRT_LIMIT_ADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[BRT_LIMIT_ADR+3]];
			if(1 == B20ms && flash_postion)
				XBYTE[DISPADR+flash_postion]=digital[10]; //��
		break;
		default :
		/*�����ѹ����*/
			XBYTE[DISPADR+0]=chartab[XBYTE[VCC_LIMIT_ADR+0]];
			XBYTE[DISPADR+1]=digital_dot[XBYTE[VCC_LIMIT_ADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[VCC_LIMIT_ADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[VCC_LIMIT_ADR+3]];
			if(1 == B20ms && flash_postion)
				XBYTE[DISPADR+flash_postion]=digital[10]; //��
		break;
		
	}

	//�������������뵽 ��ת�� 
	for(i=0; i<Length; i++)
	{
		temp=XBYTE[DISPADR+i];
		for(j=0; j<8; j++)
		{
			CLK=0;CLK=1;
		}
	}
	ST=1;ST=0; //��һ���½����ź� 4����ת��ͬʱ���� ���ת��
	
}



#if 0
/*����˵��
dis_mod ��������ѡ����ʾ�¶� ���� ���� ��ѹ
*/
void display(const uchar dis_mod)
{
	uchar i,j,temp;
	/*    ѡ����ʾ����λ ��  0�¶�λ? 1����λ?  2���ǵ�ѹλ?
	�ⲿRAM��ַ	0~3          4~7       8~11     12~15
	�궨��ĵ�ַDISPADR	   TEMPADR   BRTADR    VCCADR
	���ѡ����dismod����
	*/
	switch(dis_mod) 
	{
		case temperature:
		/*�����¶�����*/
			XBYTE[DISPADR+0]=chartab[XBYTE[TEMPADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[TEMPADR+1]];
			XBYTE[DISPADR+2]=digital_dot[XBYTE[TEMPADR+2]];
		 	XBYTE[DISPADR+3]=digital[XBYTE[TEMPADR+3]];
			break;
		case light:
		/*������������*/
			XBYTE[DISPADR+0]=chartab[XBYTE[BRTADR+0]];
			XBYTE[DISPADR+1]=digital[XBYTE[BRTADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[BRTADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[BRTADR+3]];
			break;
		case voltage :
		/*�����ѹ����*/
			XBYTE[DISPADR+0]=chartab[XBYTE[VCCADR+0]];
			XBYTE[DISPADR+1]=digital_dot[XBYTE[VCCADR+1]];
			XBYTE[DISPADR+2]=digital[XBYTE[VCCADR+2]];
		    XBYTE[DISPADR+3]=digital[XBYTE[VCCADR+3]];
			break;
		default: 
			XBYTE[DISPADR+0]=digital[10]; //��
			XBYTE[DISPADR+1]=digital[10]; //��
			XBYTE[DISPADR+2]=digital[10]; //��
			XBYTE[DISPADR+3]=digital[10]; //��
			break;
		
	}

	//�������������뵽 ��ת�� 
	for(i=0;i<Length;i++)
	{
		temp=XBYTE[DISPADR+i];
		for(j=0;j<8;j++)
		{
			CLK=0;CLK=1;
		}
	}
	ST=1;ST=0; //��һ���½����ź� 4����ת��ͬʱ���� ���ת��
	
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

	//�������������뵽 ��ת�� 
	for(i=0;i<Length;i++)
	{
		temp=XBYTE[DISPADR+i];
		for(j=0;j<8;j++)
		{
			CLK=0;CLK=1;
		}
	}
	ST=1;ST=0; //��һ���½����ź� 4����ת��ͬʱ���� ���ת��
}



//���� �жϼ���ֵ�Ƿ��д��� ���ú��˵�����ֵ
void monitor(void)
{
	//������
	uint temper_current,light_current,voltage_currnt;
	uint temper_limit,light_limit,voltage_limit;
	temper_current = GET_RAM_VLA(TEMPADR);
	light_current = GET_RAM_VLA(BRTADR);
	voltage_currnt = GET_RAM_VLA(VCCADR);

	temper_limit = GET_RAM_VLA(TEMP_LIMIT_ADR);
	light_limit = GET_RAM_VLA(BRT_LIMIT_ADR);
	voltage_limit = GET_RAM_VLA(VCC_LIMIT_ADR);

	//�Ƚ����� ע�� ��������ԭ���Ļ����ϷŴ���10�� ȥ����С�����һλ
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
				( (*temp - temperature_tab[i])/ //����
				(temperature_tab[i]-temperature_tab[i+1])/*��ĸ*/)*5; /*�м�ռ�Ȳ���*/
			//������0.5 �Ŵ���10�� ���ص�ֵҲ�ǷŴ���10��
			//���� ʵ��ֵΪ25.5��  ���Ƿ�����255
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
	//��ad��ֵת��������λ�ο�ֵ ��ȷ��һ�㶼����
	switch(select)
	{
		case temperature:
			//�� R14����
			Vres15 = (5.0/256.0) * temp;
			Ires15 = Vres15 /(10.0*1000.0);
			Res14 = (5.0-Vres15) / Ires15;
			Res14 = Res14/1000.0;
			//���
			changed_val = check_table(&Res14);
			break;
		case light://Ҫ��0~9�����ȼ��� ʵ�ʲ������adֵ193 ��Сadֵ143 ÿ5���任ֵ��ʾһ�����ȼ���
			changed_val = (temp-143)/5;
			break;
		case voltage://���贫����Ϊ���� �ο���ѹΪ5V ad��0~255�任
			changed_val = (500/255) * temp; //�Ŵ���100����Ӧ��ʾС�������λ
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
		
		//������ʾ
		if(no==modify_falg)
		{
			read_adc(temperature);
			monitor();//���ֵ�Ƿ񳬹��仯 
			display(temperature);
			while(0 != time_cnt) //ֻ����һ����ʱ1s����
				if(yes == modify_falg) goto flash;
			time_cnt++;//������һ 
			
			
			read_adc(light);
			monitor();//���ֵ�Ƿ񳬹��仯 
			display(light);
			while(0 != time_cnt)
				if(yes == modify_falg) goto flash;
			time_cnt++;
			

			read_adc(voltage);
			monitor();//���ֵ�Ƿ񳬹��仯 
			display(voltage);
			while(0 != time_cnt)
				if(yes == modify_falg) goto flash;
			time_cnt++;
			
			
			#if 0  // �ѽ��
			if(0 == B1s)
			{	
				B1s = ~B1s;
				delayms(1);//������ʱ ����CPU����
				read_adc(temp_conut);
				monitor();//���ֵ�Ƿ񳬹��仯 
				//û��1sѭ����ʾ���� ������Ƿ񳬹�����
				display(temp_conut);	
				temp_conut++;
				if(temp_conut > 3) temp_conut = 0;
				
			}
			#endif
		}
		else
		{
flash:
			//��˸�޸�ֵ
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
