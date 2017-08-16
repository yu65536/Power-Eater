#define DA_CHG 0
#define DA_DIS 1

void da_out(int ch, int data);
void da_setting(void);
void da_out_bit(int data);


void da_out(int ch, int data)
{
/*
VOUTA:充電用
VOUTB:放電用

p6_4:LDAC
p6_3:CS
p6_2:SCK
p6_1:SDI

0:A/B 0/1
1:BUF 1:Buffered 0:unbufferd
2:GA 1:1x 0:2x
3:SHDN 0:Hi-Z 1:
4-:D11 to D0
*/

	int i;
	
	p6 = 0b00010000; //cs = 0, ldac = 1
	da_out_bit(ch);
	//BUF
	da_out_bit(0);
	//GA
	da_out_bit(1);
	//shdn
	da_out_bit(1);
	//data
	for(i=0;i<12;i++)
	{
		if((data & 0x0800) == 0) //最上位ビット = 0
		{
			da_out_bit(0);
		}else{
			da_out_bit(1);
		}
		data = data << 1;
	}
	
	p6 = 0b00011000; //CS = 1
	p6 = 0b00001000; //LDAC = 0
	asm("NOP");
	asm("NOP");
	p6 = 0b00011000; //LDAC = 1
	
}

void da_out_bit(int data)
{
	if(data == 1){
		p6 = 0b00010010; //SDI = 1
		p6 = 0b00010110; //SCK = HI
		p6 = 0b00010010; //SCK = LO
	}else{
		p6 = 0b00010000; //SDI = 0
		p6 = 0b00010100; //SCK = HI
		p6 = 0b00010000; //SCK = LO
	}
}

void da_setting(void)
{
	pd6 = 0b00011110;
	p6 = 0b00011000; //CS = 1
	da_out(DA_CHG, 0x0000);
	da_out(DA_DIS, 0x0000);
}
