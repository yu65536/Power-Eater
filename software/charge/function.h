void wait_10msec_intclk(void);
void wait_100msec_intclk(void);
void wait_500msec_intclk(void);
void wait_50usec(void);
void wait_10msec(void);
void wait_500msec(void);
void clk_setting(void);
void wait_count(unsigned short count);

void timer_setting(void);

void sw_setting(void);
char data_to_chr(int data);



void clk_setting(void)
{
	//ext-clock setting
	prcr = 0b00000001;				//proc0=1 non protect
	cm1 = 0b00101000;				//CM13=1
	cm0 = 0b01011000;				//CM04=1 CM05=0
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	cm0 = 0b00001000;				//CM06=0 分周モードOFF
	//10msecでよいがLCDが40msecなので40msec待つ
	ocd = 0b00000000;				//ocd2=0 cpu clock=xin 
	prcr = 0b00000000;				//proc0=0 protect
}

void clk_setting_old(void)
{
	//ext-clock setting
	prcr = 0b00000001;				//proc0=1 non protect
	cm1 = 0b00101000;				//CM13=1
	cm0 = 0b01001000;				//CM05=0
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	cm0 = 0b00001000;				//CM06=0 分周モードOFF
	//10msecでよいがLCDが40msecなので40msec待つ
	ocd = 0b00000000;				//ocd2=0 cpu clock=xin 
	prcr = 0b00000000;				//proc0=0 protect
}

void wait_count(unsigned short count)
{
	//25+(cnt-1)*10
        asm("_wait:"
            "nop\n\t"
            "sbjnz.w #1,r1,_wait");
//nop 1cycle
//sbjnz 3 or 5cycle
}

void wait_10msec_intclk(void)
{
/* for clock stable */
/* internal clock 125kHz */
/* 8usec * 8分周 * 156.25cycle */

	wait_count(14);//25+13*10 = 155 cnt
	
}


void wait_100msec_intclk(void)
{
/* for clock stable */
/* internal clock 125kHz */
/* 8usec * 1250 */
	wait_10msec_intclk();
	wait_10msec_intclk();
	wait_10msec_intclk();
	wait_10msec_intclk();	
	wait_10msec_intclk();
	wait_10msec_intclk();
	wait_10msec_intclk();
	wait_10msec_intclk();
	wait_10msec_intclk();	
	wait_10msec_intclk();
}
void wait_500msec_intclk(void)
{
/* for clock stable */
/* internal clock 125kHz */
/* 8usec * 1250 */
	wait_100msec_intclk();
	wait_100msec_intclk();
	wait_100msec_intclk();
	wait_100msec_intclk();	
	wait_100msec_intclk();
}
void wait_50usec(void)
{
/* external clock 20MHz */
/* 0.05usec * 1000(985cycle)*/

	wait_count(97);
}

void wait_100usec(void)
{
/* external clock 20MHz */
/* 0.05usec * 1000(985cycle)*/

	wait_count(194);
}

void wait_500usec(void)
{
/* external clock 20MHz */
/* 0.05usec * 1000(985cycle)*/

	wait_count(194);
	wait_count(194);
	wait_count(194);
	wait_count(194);
	wait_count(194);
}

void wait_1msec(void)
{
/* external clock 20MHz */
/* 50usec * 200 */
	int i;
	for(i = 0; i < 20; i++)
	{
		wait_50usec();

	}
}

void wait_10msec(void)
{
/* external clock 20MHz */
/* 50usec * 200 */
	int i;
	for(i = 0; i < 200; i++)
	{
		wait_50usec();

	}
}


void wait_500msec(void)
{
/* for clock stable */
/* internal clock 20MHz */
/* 10msec * 50 */
	int i;
	//OK
	for(i = 0; i < 50; i++)
	{
		wait_10msec();
	}
}



void sw_setting(void)
{
	//出力にセット debug用
	//*
	asm("MOV.B #004H, 000AH");
	asm("MOV.B #040H, 00E2H");
	//*/
	
	pur0 = 0b00000001;//プルアップ設定
}



void timer_setting(void)
{
	tramr = 0b01000000;		//タイマーモード　ソース＝fc32
	//カウントの設定　1024/32.768kHz*32=1  1/8sec周期　128
	trapre = 0;	
	tra = 127;
	tracr = 0b00000001;
}


void ad_to_chr(char *str, unsigned int data)
{
//電圧に変換
//アンプの倍率1/2
//x/(655356-1)*2.048*2

	int i;
	char convert_8to10[8] = {0,1,2,4,5,6,8,9};
	
	str[0] = data / 8000;
	data = data % 8000;
	str[2] = data / 800;
	data = data % 800;
	str[3] = data / 80;
	data = data % 80;
	str[4] = data / 8;
	str[5] = convert_8to10[ data % 8 ];

	for(i=0;i<6;i++)
	{
	  str[i] += 48;
	}
	str[1] = '.';
	str[6] = 'V';
}

char data_to_chr(int data)
{
	if(data>9)
	{
		return 'A'-10+data;
	}
	return '0'+data;
}

void change_led(int onoff)
{
	if(onoff)
	{
		p2 |= 0b00000001;
	}else
	{
		p2 &= 0b11111110;
	}
}