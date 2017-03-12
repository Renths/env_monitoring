#include"mydefine.h"



//∂Ã‘›—” ± ÃÊªª _nop_();
void my_nop(void)
{
	uchar j;
	for(j=0;j<0x0f;j++)
		;
}


void ad_select_in(uchar select)
{
	switch(select)
	{
		case temperature:
			AD_SET_IN_A = 0;
			AD_SET_IN_B = 0;
			break;
		case light:
			AD_SET_IN_A = 1;
			AD_SET_IN_B = 0;
			break;
		case voltage:
			AD_SET_IN_A = 0;
			AD_SET_IN_B = 1;
			break;
		default : break;
	}
}

uchar adc(uchar select)
{
	uchar i,temp;
	AD_CS = 0;
	my_nop();
	
	ad_select_in(select);
	my_nop();
	
	for(i=0; i<8; i++)
	{
		temp = temp<<1;
		AD_CLK = 0;
		my_nop();
		AD_CLK = 1;
		my_nop();
		temp |= AD_DO;
	}
	AD_CS = 1;
	return temp;
}
