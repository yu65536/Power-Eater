void wait_10msec_intclk(void);
void wait_100msec_intclk(void);
void wait_500msec_intclk(void);
void wait_50usec(void);
void wait_10msec(void);
void wait_500msec(void);
void clk_setting(void);
//void i2c_setting(void);
//void i2c_int(void);
void i2c_send(char *data, int n);
int i2c_check(void);
void wait_count(unsigned short count);



char *i2c_data;
char i2c_data_num;
int i2c_flg = 0;

void clk_setting(void)
{
	//ext-clock setting
	prcr |= 0b00000001;				//proc0=1 non protect
	cm1 |= 0b00001000;				//CM13=1
	cm0 &= 0b11011111;				//CM05=0
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	wait_10msec_intclk();	//wait　10ms
	//10msecでよいがLCDが40msecなので40msec待つ
	ocd2 &= 0b11111011;				//ocd2=0 cpu clock=xin 
	prcr &= 0b11111110;				//proc0=0 protect
}

void wait_count(unsigned short count)
{
	//25+(cnt-1)*10
        asm("_wait:"
			"nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "sbjnz.w #1,r1,_wait");
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
/* for clock stable */
/* internal clock 20MHz */
/* 0.05usec * 1000 */

	wait_count(98);
}

void wait_10msec(void)
{
/* for clock stable */
/* internal clock 20MHz */
/* 50usec * 200 */
	int i;
	for(i=0;i<200;i++)
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
	for(i=0;i<50;i++)
	{
		wait_10msec();
	}
}

/*void i2c_setting(void)
{
	//I2C setting

	char data = 0;
	
	//send
	//(1)
	pmr |=0b10000000;				//	PMR IICSEL = 1
	sar &= 0b11111110;				//SAR FS =0
	icsr &= 0b11110111;				//icsr stop = 0
	iccr1 = 0b10001000;				//ice = 1

	//(2)
	while(iccr2 & 0b10000000 != 0)
	{
		//busy
		//停止条件入れる
	}
	
	//not busy
	iccr1 = 0b10111000;		//mastertensoumode trs, mst  CKS = 1000 20MHz/56=357kHz 
	asm("MOV.B #0bfH, 00B9H");  //bbsy=1, scp=0   0b10110000

	//(3)
	while(icsr & 0b10000000 == 0)	//tdre = 0
	{
		//before send wait
		//停止条件入れる
	}
	icdrt = data;				//send data (1byteme) set

	//(4)
	while(icsr & 0b01000000 == 0 ) 		//tend = 0
	{
		//send end wait
		//停止条件入れる
	}
	while(icier & 0b00000010 == 1) //ACKBR = 1
	{
		//slave ack wait
		//停止条件入れる mov bbsy=0, scp=0
	}
	icdrt = data;				//send data (2byteme) set
	
	//(5)
	while(1)//exist send data
	{
		while(icsr & 0b10000000 == 0)	//tdre = 0
		{
			//before send wait
			//停止条件入れる
		}
		icdrt = data;				//send data set
	}
	
	//(6)
	//all data send
	while(icsr & 0b11000000 != 0b11000000)	//tdre = 0 or tend=0
	{
		//before send wait
		//停止条件入れる
	}
	//tdre = 1 tend = 1
	//stop
	asm("MOV.B #030H, 00B9H"); //bbsy = 0 scp = 0  0b00110000
	
	//(7)
	while(icsr & 0b00001000 == 0)	//icsr stop = 0
	{
		//wait stop
		//停止条件入れる
	}
	//icsr stop = 1
	//slave receive mode
	iccr1 = 10001000;

	
}*/


#pragma interrupt int15
void int15(void)
{
			//OK
//						p0 = 0b01000000;
	
	//i2c割り込み発生時の処理
	if(icier & 0b10000000 && (icsr & 0b10000000))
	{
		//TIE = 1 and TDRE = 1
		//送信データエンプティ
		
		//(3)
		if(i2c_data_num > 0)
		{
			icdrt = *i2c_data;				//send data (1byteme) set
			++i2c_data;
			i2c_data_num --;
					//OK
		}else{
			//OK
		}
		//OK
	}
	else if(icier & 0b01000000 && (icsr & 0b01000000))
	{
		//TEIE = 1 and TEND = 1
		//送信終了
		asm("MOV.B #030H, 00B9H"); //bbsy = 0 scp = 0  0b00110000
		//NG
					p0 = 0b01000000;
	}
	else if(icier & 0b00100000 && (icsr & 0b00100000))
	{
		//RIE = 1 and RDRF = 1
		//受信データフル
		
	}
	else if(icier & 0b00001000 && (icsr & 0b00001000))
	{
		//STIE = 1 and STOP = 1
		//停止条件検出
		
		//slave receive mode
		iccr1 = 10001000;
		i2c_flg = 0;	//i2c終了

	icier = 0b00000000;		//i2c各要素の割り込み設定
		//NG

	}
	else if(icier & 0b00010000 && (icsr & 0b00000100))
	{
		//NAKIE = 1 and AL = 1
		//アービトレーションロスト/オーバーランエラー
	}
	else if(icier & 0b00010000 && (icsr & 0b00010000))
	{
		//NAKIE = 1 and NACKF = 1
		//NACK検出
					//NG
		asm("MOV.B #030H, 00B9H"); //bbsy = 0 scp = 0  0b00110000
	}
	else
	{
		
	}
}

void i2c_send(char *data, int n)
{
	//I2C setting
	icier = 0b00000000;		//i2c各要素の割り込み設定

	i2c_data = data;
	i2c_data_num = n;
	i2c_flg = 1;	//i2c使用中
		
	//send
	//(1)
	pmr |=0b10000000;				//	PMR IICSEL = 1
	sar &= 0b11111110;				//SAR FS =0
	icsr &= 0b11110111;				//icsr stop = 0
	iccr1 = 0b10001000;				//ice = 1
	icmr = 0b00001000;

	icier = 0b11011000;		//i2c各要素の割り込み設定

	//(2)
	while((iccr2 & 0b10000000) != 0)
	{
		//busy
		//停止条件入れる
	}
					//Ok
//
	//not busy
	iccr1 = 0b10111000;		//mastertensoumode trs, mst  CKS = 1000 20MHz/56=357kHz 
//NG
	asm("MOV.B #0B8H, 00B9H");  //bbsy=1, scp=0   0b10110000
//NG	

/*	int i;
	
	for(i=0;i<n;i++)
	{
		//send data
		++data;
	}
*/	

	
}

int i2c_check()//return 1で使用不可　0で使用可
{
//NG
	return i2c_flg;
	
}