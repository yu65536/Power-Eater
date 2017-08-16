
void i2c_send(char *data, int n, int mode);

void lcd_display(char adr, char str[17], int num);
void lcd_setting(void);
unsigned int ad_read(void);


void ad_setting(void);

//void lcd_blink(char on_off);
//void lcd_set_address(char adr);

void lcd_cursor(char on_off);






void i2c_send(char *data, int n, int mode)
{
	//I2C setting

	char tmp;
	int n_tmp;
	if(mode==0)
	{
		n_tmp=n;
	}else{
		n_tmp=1;
	}
//ok
	//send
	//(1)
	pmr |=0b10000000;				//	PMR IICSEL = 1
	sar &= 0b11111110;				//SAR FS =0
	icsr &= 0b11110111;				//icsr stop = 0
	iccr1 = 0b10001000;				//ice = 1
	icmr = 0b00001000;


	//(2)
	while((iccr2 & 0b10000000) != 0)
	{
		//busy
	}
	//not busy
	iccr1 = 0b10111000;		//mastertensoumode trs, mst  CKS = 1000 20MHz/56=357kHz 
	asm("MOV.B #0B0H, 00B9H"); //bbsy = 1 scp = 0  0b00110000

	//(3)
	//送信データセット
	if(n_tmp > 0)
	{
		//(4)
		icdrt = *data;				//send data (1byteme) set
		++data;
		n_tmp --;
	}
//ok
	//(5)
	while((icsr & 0b01000000) == 0)//tend==0
	{
	}
	//(6)
	wait_50usec();
	if((icier & 0b00000010) != 0)//ackbr==1
	{
		//停止条件
		goto lbl_stop;
	}
	if(mode == 0)
	{
		//NG
		//if 送信モード
		while(n_tmp>1)
		{
			if(n_tmp > 0)
			{
				//(4)
				icdrt = *data;				//send data (1byteme) set
				++data;
				n_tmp --;
				//NG
			}
			while((icsr & 0b10000000)==0)//転送待ち
			{
			}
		}
	
		icdrt = *data;				//send data (1byteme) set

		while((icsr & 0b01000000)==0)//TEND==0
		{
		}
	
	
lbl_stop:

		icsr &= 0b1010111; //tend=0  stop =0
		
		asm("MOV.B #030H, 00B9H"); //bbsy = 0 scp = 0  0b00110000
		while((icsr & 0b00001000)==0)//stop==0
		{
		}
		//slave receive mode
		iccr1 = 10001000;

		icsr &= 0b01111111; //tdre=0
	}else{
		//if 受信モード
		//(1)
		icsr &= 0b10111111;				//icsr tend = 0
		iccr1 = 0b10101000;				//trs = 0
		icsr &= 0b01111111;				//icsr tdre = 0
		icier &= 0b11111110;			//ACKBT = 0

		//(2)
		tmp = icdrr;					//icdrr ダミーリード
		
		while(n>2)
		{
			while((icsr & 0b00100000) == 0) 	//rdrf = 0 then loop
			{
			}
			//OK
			*data = icdrr;
			++data;
			n--;
		}
		while((icsr & 0b00100000) == 0) 	//rdrf = 0 then loop
		{
		}
		icier |= 0b00000001;//ackbt=1
		iccr1 |= 0b01000000;//rcvd=1
		*data = icdrr;
		++data;
		n--;

		//ok
		while((icsr & 0b00100000) == 0) 	//rdrf = 0 then loop
		{
		}
//ok
		icsr &= 0b1110111; //tend=0  stop =0
		
		asm("MOV.B #030H, 00B9H"); //bbsy = 0 scp = 0  0b00110000
		while((icsr & 0b00001000)==0)//stop==0
		{
		}
		*data = icdrr;
				
		
		//slave receive mode
		iccr1 = 10001000;
		//NG


	}
}



void lcd_setting()
{
	char i2c_data[35];
	int i2c_num;

//OK
//LCDセットアップ	
					

	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b00111000;
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
//OK
	wait_10msec();
	
	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b00111001;
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();
	
	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b00010100;
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ; //osc
	wait_10msec();


	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b01110011;	//contrast
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();
//OK

	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b01010010;	//power/icon/contrast control
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();

//OK

	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b01101100;	//follower control
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
//OK
	wait_500msec();
//OK
	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b00111000;
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();

	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b00000001;	//clear display
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();
		
	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b00001100;	//display on/off control
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();
	
	//LCDイニシャライズ終了
	//NG


	i2c_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b00000000;
	i2c_data[2] = 0b00000000;	
	i2c_num = 3;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();	
//	wait_1msec();	

		
}

void lcd_display(char adr, char str[17], int num)
{
	char send_data[35];
	int send_num;
	int i;
	int flg=0;
	
	send_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	send_data[1] = 0b00000000;
	send_data[2] = adr;
	send_num = 3;//35;
	i2c_send(send_data, send_num,0) ;
	wait_10msec();
	
	send_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)

	if(str[num-1]=='\0'){
		num = 16;
	}
	for( i = 0; i < num; i++ )
	{
		if(i == num - 1)
		{
			send_data[2*i+1]=0b01000000;
		}else{
			send_data[2*i+1]=0b11000000;
		}
		
		if((flg == 0) && (str[i] != '\0'))
		{
			send_data[2*i+2] = str[i];
		}else{
			send_data[2*i+2] = ' ';
			flg = 1;
		}
	}
	send_num = num*2+1;
	i2c_send(send_data, send_num,0) ;

}

void lcd_cursor(char on_off)
{
	char send_data[35];
	int send_num;
	
	send_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	send_data[1] = 0b00000000;
	if(on_off == CURSOR_OFF)
	{
		send_data[2] = 0b00001100;
	}else{
		send_data[2] = 0b00001110;
	}
	
	send_num = 3;
	i2c_send(send_data, send_num,0) ;
}

void lcd_set_address(char adr)
{
	char send_data[35];
	int send_num;
	int i;
	int flg=0;
	
	send_data[0] = 0b01111100;	//１バイト目はアドレス+R(1)/W(0)
	send_data[1] = 0b00000000;
	send_data[2] = adr;
	send_num = 3;
	i2c_send(send_data, send_num,0) ;
	

}

void ad_setting(void)
{
	
	char i2c_data[35];
	int i2c_num;
	i2c_data[0] = 0b11010000;	//１バイト目はアドレス+R(1)/W(0)
	i2c_data[1] = 0b10011000;
	i2c_num = 2;
	i2c_send(i2c_data,i2c_num,0) ;
	wait_10msec();
}

unsigned int ad_read()
{
	
	char i2c_data[35];
	int i2c_num;
	i2c_data[0] = 0b11010001;	//１バイト目はアドレス+R(1)/W(0)
	i2c_send(i2c_data, 3,1);
	wait_10msec();
	return i2c_data[1]*256+i2c_data[2];
}
