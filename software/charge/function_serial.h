void serial1_setting(void);
void serial1_send_char(char *data);
void serial1_send(char *data);
void serial1_send_n(char *data, int n);

void serial1_setting(void)
{
	//asm u1brg = 0x081;//9600bps@20MHz
	asm("MOV.B #081H, 00A9H");
	u1mr = 0b00000101;
	u1sr = 0x0f;
	u1c0 = 0b00001000;
	pmr = 0b00010000;
	u1c1 = 0b00000001;
}

void serial1_send_char(char *data)
{
	while((u1c1 & 0b00000010)==0){
	}
	//asm u1tb = 0x0030;
//	asm("MOV.B #030H, 00AAH");
//	asm("MOV.B _data, 00AAH");
	u1tb=*data;
}

void serial1_send(char *data)
{
	while((*data != '\n') && (*data != '\0'))
	{
		serial1_send_char(data);
		++data;
	}
	if(*data == '\n')
	{
		serial1_send_char(data);
	}

}

void serial1_send_n(char *data, int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		serial1_send_char(data);
		++data;
	}
//	serial1_send_char(data);

}