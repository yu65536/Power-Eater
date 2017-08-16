void init(void);
void display_menu(int status);
char check_sw(void);
void read_data(unsigned char *write_adr, unsigned int read_adr, int size);
void write_data(unsigned int bank, unsigned int write_index, unsigned char *data_pointer, unsigned int size);
void display_setting(char num, char point, unsigned char hi, unsigned char lo, char *unit);
void change_setting(char num, char point, unsigned char *hi, unsigned char *lo, int max, int min, char *unit);


void status_change(void);
void setting_change(void);

void edit_mode_on(void);
void edit_mode_off(void);

void data_check(char *data);

void charge(void);
void cvcharge(void);
void discharge(void);
void sequence(void);


char convert_4to10[4] = {0,3,5,8};

void current_to_current_char(void);
void current_char_to_current(void);

int ad_read_internal(int ch);
void bat_test(void);
void data_erase(void);



unsigned char data[1024];
unsigned char data_seq[256];
unsigned char data_setting[16];
unsigned char setting_mode;
unsigned char setting_place;

char seq_mode;

char current_char[3];
unsigned int current;
unsigned int stop_current;
unsigned int current_monitor;
unsigned int voltage;
unsigned int voltage_monitor;
unsigned int voltage_max;

unsigned char shibori;

unsigned long capacity;
unsigned long capacity_monitor;
unsigned int delta_peak;
unsigned int peak_delay;
unsigned char mode;
unsigned char mode_shibori;

unsigned char cv_trickle_mode;
unsigned char dec_speed;

unsigned long time_cnt;


unsigned char pc;

unsigned char end_flag;
unsigned char seq_cnt[4];

char cap_mode;
char buf[16];



void init(void)
{
	clk_setting();
	serial1_setting();
	timer_setting();
	sw_setting();
	lcd_setting();
	ad_setting();
	da_setting();
	p2 = 0b00000000;
//	p2drr = 0b10000000;
//	trdoer1 = 0b11111111;
//	pd2 = 0b11100000;	
	pd2 = 0b00000001;
	p2drr = 0b00000001;
	adcon1 = 0b00101000;
	cap_mode=0;
}

void status_change(void)
{
		if(status < 0x10)//MAIN MENU
		{
			if((sw_data & SW_LEFT) != 0)
			{
			}else if((sw_data & SW_UP) != 0)
			{
				if(status != MENU_CHG)
				{
					status --;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if(status != MENU_CFG)
				{
					status ++;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
				if(status != MENU_CFG)
				{
					status = status + 0x10;
					data_num = 0;
				}else{
					status = MENU_CFG_BAT;
				}
			}
		}else if(status == MENU_CHG_LOAD)//charge load menu
		{
			if((sw_data & SW_LEFT) != 0)
			{
				status = MENU_CHG;
			}else if((sw_data & SW_UP) != 0)
			{
				if(data_num != 0)
				{
					data_num --;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if(data_num != 15)
				{
					data_num ++;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
				read_data(data_setting, BANK_A+data_num*16,16);
				data_check(data_setting);
				status = MENU_CHG_START;
			}
		}else if(status == MENU_DIS_LOAD)//discharge load menu
		{
			if((sw_data & SW_LEFT) != 0)
			{
				status = MENU_DIS;
			}else if((sw_data & SW_UP) != 0)
			{
				if(data_num != 0)
				{
					data_num --;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if(data_num != 15)
				{
					data_num ++;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
				read_data(data_setting, BANK_A+data_num*16+512,16);
				data_check(data_setting);
				status = MENU_DIS_START;
			}
		}else if(status == MENU_SEQ_LOAD)//sequence load menu
		{
			if((sw_data & SW_LEFT) != 0)
			{
				status = MENU_SEQ;
			}else if((sw_data & SW_UP) != 0)
			{
				if(data_num != 0)
				{
					data_num --;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if(data_num != 3)
				{
					data_num ++;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
				pc = 0;
				read_data(data_seq, BANK_B+data_num*256,256);
				status = MENU_SEQ_START;
			}

		}else if(status <0x30)//charge MENU
		{
			if((sw_data & SW_LEFT) != 0)
			{
				status = MENU_CHG_LOAD;
				edit_mode_off();
			}else if((sw_data & SW_UP) != 0)
			{
				if((status != MENU_CHG_START) && (setting_mode == NOT_EDIT_MODE))
				{
					status --;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if((status != MENU_CHG_SAVE) && (setting_mode == NOT_EDIT_MODE))
				{
					status ++;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
//				status = MENU_CHG;

			}
		}else if(status <0x40)//discharge MENU
		{
			if((sw_data & SW_LEFT) != 0)
			{
				status = MENU_DIS_LOAD;
				edit_mode_off();
			}else if((sw_data & SW_UP) != 0)
			{
				if((status != MENU_DIS_START) && (setting_mode == NOT_EDIT_MODE))
				{
					status --;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if((status != MENU_DIS_SAVE) && (setting_mode == NOT_EDIT_MODE))
				{
					status ++;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
//				status = MENU_CHG;
			}
		}else if(status <0x50)//seq MENU
		{
			if((sw_data & SW_LEFT) != 0)
			{
				status = MENU_SEQ_LOAD;
				edit_mode_off();
			}else if((sw_data & SW_UP) != 0)
			{
				if((status == MENU_SEQ_SETTING) && (pc > 0) && (setting_mode == NOT_EDIT_MODE))
				{
					pc--;
				}else if((status != MENU_SEQ_START) && (pc == 0) && (setting_mode == NOT_EDIT_MODE))
				{
					status--;
				}else if(status == MENU_SEQ_START)
				{
					status = MENU_SEQ_SETTING;
					pc = 255;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if((status != MENU_SEQ_SETTING) && (setting_mode == NOT_EDIT_MODE))
				{
					status ++;
					pc = 0;
				}else if((status == MENU_SEQ_SETTING) && (pc < 255) && (setting_mode == NOT_EDIT_MODE))
				{
					pc++;
				}else if((status == MENU_SEQ_SETTING) && (pc == 255) && (setting_mode == NOT_EDIT_MODE))
				{
					status = MENU_SEQ_START;
					pc = 0;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
//				status = MENU_CHG;
			}

		}else if(status <0x60)//config MENU
		{
			if((sw_data & SW_LEFT) != 0)
			{
				status = MENU_CFG;
				edit_mode_off();
			}else if((sw_data & SW_UP) != 0)
			{
				if((status != MENU_CFG_BAT) && (setting_mode == NOT_EDIT_MODE))
				{
					status --;
				}
			}else if((sw_data & SW_DOWN) != 0)
			{
				if((status != MENU_CFG_RST) && (setting_mode == NOT_EDIT_MODE))
				{
					status ++;
				}
			}else if((sw_data & SW_RIGHT) != 0)
			{
//				status = MENU_CHG;
			}
		}
}

void display_menu(int status)
{
	int i;
	switch(status)
	{
		case MENU_CHG:
			lcd_display(0x80, "CCCV V CHARGE\0", 16);
			buf[0] = 0x15;
			lcd_display(0x84, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;

		case MENU_DIS:
			lcd_display(0x80, "DISCHARGE\0", 16);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_SEQ:
			lcd_display(0x80, "SEQUENCE\0", 16);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_CFG:
			lcd_display(0x80, "CONFIG\0", 16);
			lcd_display(0xC0, "\0", 16);
			break;

		case MENU_CHG_LOAD:
			lcd_display(0x80, "CHARGE No. \0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x8A, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_DIS_LOAD:
			lcd_display(0x80, "DISCHARGE No. \0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x8D, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_SEQ_LOAD:
			lcd_display(0x80, "SEQUENCE No. \0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x8C, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_CHG_START:
			lcd_display(0x80, "CHG  START\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_CHG_CURRENT:
			lcd_display(0x80, "CHG  CURRENT\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(3, 1, data_setting[0], data_setting[1], "A\0");
			break;
		case MENU_CHG_DELTAPEAK:
			lcd_display(0x80, "CHG  DELTA PEAK\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(2, 0, 0, data_setting[2], "mV\0");
			break;
		case MENU_CHG_PEAKDELAY:
			lcd_display(0x80, "CHG  PEAK DELAY\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(3, 0, data_setting[3], data_setting[4], "sec\0");
			break;
		case MENU_CHG_VOLTAGE:
			lcd_display(0x80, "CHG  STOP VOLT\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(4, 1, data_setting[5], data_setting[6], "V\0");
			break;
		case MENU_CHG_CVTRICKLE://修正
			lcd_display(0x80, "CHG  CVMODE\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			if(data_setting[7] == 0)
			{
				lcd_display(0xC0, "CC + OVP\0", 16);
			}else if(data_setting[7] == 1){
				lcd_display(0xC0, "CV -> STOP\0", 16);
			}else{
				lcd_display(0xC0, "CV -> TRICKLE\0", 16);
			}
			break;
		case MENU_CHG_STOPCUR:
			lcd_display(0x80, "CHG  STOP CUR\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(3, 1, data_setting[8], data_setting[9], "A\0");
			break;
		case MENU_CHG_DECSPEED:
			lcd_display(0x80, "CHG  DEC SPEED\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(2, 0, 0, data_setting[10], "\0");
			break;
		case MENU_CHG_CAPACITY:
			lcd_display(0x80, "CHG  STOP CAP\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(4, 0, data_setting[11], data_setting[12], "mAh\0");
			break;

		case MENU_CHG_SAVE:
			lcd_display(0x80, "CHG  SAVE\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;



		case MENU_DIS_START:
			lcd_display(0x80, "DIS  START\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_DIS_CURRENT:
			lcd_display(0x80, "DIS  CURRENT\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(3, 1, data_setting[0], data_setting[1], "A\0");
			break;
		case MENU_DIS_VOLTAGE:
			lcd_display(0x80, "DIS  STOP VOLT\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(4, 1, data_setting[2], data_setting[3], "V\0");
			break;
		case MENU_DIS_SHIBORI:
			lcd_display(0x80, "DIS  SHIBORI\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			if(data_setting[4] == 0)
			{
				lcd_display(0xC0, "OFF\0", 16);
			}else{
				lcd_display(0xC0, "ON\0", 16);
			}
			break;
		case MENU_DIS_DECSPEED:
			lcd_display(0x80, "DIS  DEC SPEED\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(2, 0, 0, data_setting[5], "\0");
			break;
		case MENU_DIS_STOPCUR:
			lcd_display(0x80, "DIS  STOP CUR\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(3, 1, data_setting[6], data_setting[7], "A\0");
			break;
		case MENU_DIS_CAPACITY:
			lcd_display(0x80, "DIS  STOP CAP\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			display_setting(4, 0, data_setting[8], data_setting[9], "mAh\0");
			break;
		case MENU_DIS_SAVE:
			lcd_display(0x80, "DIS  SAVE\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_SEQ_START:
			lcd_display(0x80, "SEQ  START\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_SEQ_SAVE:
			lcd_display(0x80, "SEQ  SAVE\0", 16);
			buf[0] = data_to_chr(data_num);
			lcd_display(0x83, buf, 1);
			lcd_display(0xC0, "\0", 16);
			break;			
		case MENU_SEQ_SETTING:
			lcd_display(0x80, "SEQ     SETTING\0", 16);
			buf[0] = data_to_chr(data_num);
			buf[1] = '-';
			buf[2] = data_to_chr(pc / 16);
			buf[3] = data_to_chr(pc % 16);
			lcd_display(0x83, buf, 4);
			buf[0] = data_to_chr(data_seq[pc] / 16);
			buf[1] = data_to_chr(data_seq[pc] % 16);
			buf[2] = '\0';
			lcd_display(0xC0, buf, 16);
			break;

		case MENU_CFG_BAT:
			lcd_display(0x80, "BATTERY CHECK\0", 16);
			lcd_display(0xC0, "\0", 16);
			break;
		case MENU_CFG_RST:
			lcd_display(0x80, "DATA RESET\0", 16);
			lcd_display(0xC0, "\0", 16);
			break;

		default:
			lcd_display(0x80, "ERR\0", 16);
			lcd_display(0xC0, "\0", 16);
			break;
	}
}

void edit_mode_on(void)
{
	setting_mode = EDIT_MODE;
	lcd_cursor(CURSOR_ON);
	setting_place = 0;
	lcd_set_address(0xC0);
}

void edit_mode_off(void)
{
	setting_mode = NOT_EDIT_MODE;
	lcd_cursor(CURSOR_OFF);
	
}

void setting_change(void)
{
//	char *tmp_adr;
	int tmp_int;
	char dummy_data;
	switch(status)
	{
		case MENU_CHG_START:
			if((sw_data & SW_RIGHT) != 0)
			{
				charge();
			}
			break;
			
		case MENU_CHG_CURRENT:
			change_setting(3, 1, &data_setting[0], &data_setting[1], 700, 0, "A\0");
			break;
		case MENU_CHG_DELTAPEAK:
			dummy_data=0;
			change_setting(2, 0, &dummy_data, &data_setting[2], 99, 0, "mV\0");
			break;
		case MENU_CHG_PEAKDELAY:
			change_setting(3, 0, &data_setting[3], &data_setting[4], 999, 0, "sec\0");
			break;
		case MENU_CHG_VOLTAGE:
			change_setting(4, 1, &data_setting[5], &data_setting[6], 7000, 0, "V\0");
			break;
		case MENU_CHG_CVTRICKLE:
		 	if(setting_mode == EDIT_MODE)
			{
				if((sw_data & SW_UP) != 0)
				{
					if(data_setting[7] < 2)
					{
						data_setting[7]++;
						if(data_setting[7] == 0)
						{
							lcd_display(0xC0, "CC + OVP\0", 16);
						}else if(data_setting[7] == 1){
							lcd_display(0xC0, "CV -> STOP\0", 16);
						}else{
							lcd_display(0xC0, "CV -> TRICKLE\0", 16);
						}
						lcd_set_address(0xC0);	
					}
				}else if((sw_data & SW_DOWN) != 0)
				{
					if(data_setting[7] > 0)
					{
						data_setting[7]--;
						if(data_setting[7] == 0)
						{
							lcd_display(0xC0, "CC + OVP\0", 16);
						}else if(data_setting[7] == 1){
							lcd_display(0xC0, "CV -> STOP\0", 16);
						}else{
							lcd_display(0xC0, "CV -> TRICKLE\0", 16);
						}
						lcd_set_address(0xC0);
					}
				}else if((sw_data & SW_RIGHT) != 0)
				{
					edit_mode_off();
				}
			}else
			{
				if((sw_data & SW_RIGHT) != 0)
				{
					edit_mode_on();
					lcd_set_address(0xC0);
				}
			}
			break;

		case MENU_CHG_STOPCUR:
			change_setting(3, 1, &data_setting[8], &data_setting[9], 700, 0, "A\0");
			break;
		case MENU_CHG_DECSPEED:
			dummy_data=0;
			change_setting(2, 0, &dummy_data, &data_setting[10], 99, 0, "\0");
			break;
		case MENU_CHG_CAPACITY:
			change_setting(4, 0, &data_setting[11], &data_setting[12], 9999, 0, "mAh\0");
			break;
		case MENU_CHG_SAVE:
			if((sw_data & SW_RIGHT) != 0)
			{
				write_data(BANK_A, data_num*16, data_setting, 16);
			}
			break;







		case MENU_DIS_START:
			if((sw_data & SW_RIGHT) != 0)
			{
				discharge();
			}
			break;
		case MENU_DIS_CURRENT:
			change_setting(3, 1, &data_setting[0], &data_setting[1], 700, 0, "A\0");
			break;

		case MENU_DIS_VOLTAGE:
			change_setting(4, 1, &data_setting[2], &data_setting[3], 7000, 0, "V\0");
			break;
		case MENU_DIS_SHIBORI:
		 	if(setting_mode == EDIT_MODE)
			{
				if((sw_data & SW_UP) != 0)
				{
					if(data_setting[4] < 1)
					{
						data_setting[4]++;
						lcd_display(0xC0, "ON\0", 16);
						lcd_set_address(0xC0);
					}
				}else if((sw_data & SW_DOWN) != 0)
				{
					if(data_setting[4] > 0)
					{
						data_setting[4]--;
						lcd_display(0xC0, "OFF\0", 16);
						lcd_set_address(0xC0);
					}

				}else if((sw_data & SW_RIGHT) != 0)
				{
					edit_mode_off();
				}
			}else
			{
				if((sw_data & SW_RIGHT) != 0)
				{
					edit_mode_on();
					lcd_set_address(0xC0);
				}
			}
			break;
		case MENU_DIS_DECSPEED:
			dummy_data=0;
			change_setting(2, 0, &dummy_data, &data_setting[5], 99, 0, "\0");
			break;			
		case MENU_DIS_STOPCUR:
			change_setting(3, 1, &data_setting[6], &data_setting[7], 700, 0, "A\0");
			break;
		case MENU_DIS_CAPACITY:
			change_setting(4, 0, &data_setting[8], &data_setting[9], 9999, 0, "mAh\0");
			break;
		case MENU_DIS_SAVE:
			if((sw_data & SW_RIGHT) != 0)
			{
				write_data(BANK_A, 512+data_num*16, data_setting, 16);
			}
			break;
		case MENU_SEQ_START:
			if((sw_data & SW_RIGHT) != 0)
			{
				sequence();
			}
			break;
		case MENU_SEQ_SAVE:
			if((sw_data & SW_RIGHT) != 0)
			{
				write_data(BANK_B, data_num*256, data_seq, 256);
			}
			break;
	
		case MENU_SEQ_SETTING:
		 	if(setting_mode == EDIT_MODE)
			{
				if((sw_data & SW_UP) != 0)
				{
					if(setting_place == 0)
					{
						data_seq[pc] += 16;
						buf[0] = data_to_chr(data_seq[pc] / 16);
						buf[1] = data_to_chr(data_seq[pc] % 16);
						lcd_display(0xC0, buf, 2);
						lcd_set_address(0xC0);
					}else if(setting_place == 1)
					{
						data_seq[pc] += 1;
						buf[0] = data_to_chr(data_seq[pc] / 16);
						buf[1] = data_to_chr(data_seq[pc] % 16);
						lcd_display(0xC0, buf, 2);
						lcd_set_address(0xC1);
					}
				}else if((sw_data & SW_DOWN) != 0)
				{
					if(setting_place == 0)
					{
						data_seq[pc] -= 16;
						buf[0] = data_to_chr(data_seq[pc] / 16);
						buf[1] = data_to_chr(data_seq[pc] % 16);
						lcd_display(0xC0, buf, 2);
						lcd_set_address(0xC0);
					}else if(setting_place == 1)
					{
						data_seq[pc] -= 1;
						buf[0] = data_to_chr(data_seq[pc] / 16);
						buf[1] = data_to_chr(data_seq[pc] % 16);
						lcd_display(0xC0, buf, 2);
						lcd_set_address(0xC1);
					}

				}else if((sw_data & SW_RIGHT) != 0)
				{
					if(setting_place == 1)
					{
						edit_mode_off();
					}else
					{
						setting_place++;
						lcd_set_address(0xC0+setting_place);
					}
				}
			}else
			{
				if((sw_data & SW_RIGHT) != 0)
				{
					edit_mode_on();
				}
			}
			break;


		case MENU_CFG_BAT:
			if((sw_data & SW_RIGHT) != 0)
			{
				bat_test();
			}
			break;
		case MENU_CFG_RST:
			if((sw_data & SW_RIGHT) != 0)
			{
				data_erase();
			}
			break;
		default:
			break;
	}

}

char check_sw(void)
{
	//3回読んで同じ状態ならON or OFF
	//off->onを押したとする
	static char sw_data_old;
	char sw_data[3];
	char sw_data_old_tmp;
	
	sw_data_old_tmp = sw_data_old;
	wait_10msec();
	sw_data[0] = p0;
	wait_10msec();
	sw_data[1] = p0;
	wait_10msec();
	sw_data[2] = p0;
	sw_data_old = (sw_data_old & (sw_data[0] | sw_data[1] | sw_data[2])) | (sw_data[0] & sw_data[1] & sw_data[2]);
	return (~sw_data_old & sw_data_old_tmp);
}

//データフラッシュ関係

void read_data(unsigned char *write_adr, unsigned int read_adr, int size)
{
	int i;
//	lcd_display(0xC0, "READ DATA\0", 16);
	for(i=0; i<size; i++)
	{
		*write_adr = *(signed char *)read_adr;
		++write_adr;
		++read_adr;
	}
//	lcd_display(0xC0, "READ END\0", 16);
}

void write_data(unsigned int bank, unsigned int write_index, unsigned char *data_pointer, unsigned int size)
//bank: 書き込むバンク
//write_index: 書き込みたいindex 1024個の配列のインデックス
//data_pointer:書き込むデータのポインタ
//size: 書き込むサイズ
{
	int i;
	unsigned int adr;
	
	read_data(data, bank, 1024);
	for(i=write_index;i<write_index+size;i++)
	{
		data[i] = *data_pointer;
		++data_pointer;
	}
	
	lcd_display(0xC0, "WRITE DATA\0", 16);
	fmr0 = 0b00000001;
	asm(" ");
	
	fmr0 = 0b00000011;//プロテクト解除
	fmr1 = 0b10000000;//EW1モード
	fmr1 = 0b10000010;//EW1モード
	asm(" ");
	adr = bank;
	*(signed char *)adr = 0x20;
	*(signed char *)adr = 0xd0;
	while((fmr0 & 0b00000001) == 0)//イレーズ待ち
	{
	}
	if((fmr0 & 0b10000000) != 0)
	{
		//エラー
	}

	*(signed char *)adr = 0x50;
	
	for(i=0; i<1024; i++)
	{
		*(signed char *)adr = 0x40;
		*(signed char *)adr = data[i];
		++adr;
		while((fmr0 & 0b00000001) == 0)//ライト待ち
		{
		}
		if((fmr0 & 0b10000000) != 0)
		{
			//エラー
		}
		*(signed char *)adr = 0x50;
	}
	*(signed char *)adr = 0xFF;


	fmr0 = 0b00000001;//プロテクト
	lcd_display(0xC0, "WRITE END\0", 16);
}


void charge(void)
{
	int i;
	
	mode = MODE_CHG;
	
	change_led(LED_ON);
	
	serial1_send("\r\n");
	serial1_send("charge start\r\n");
	serial1_send("/////setting/////\r\n");

//データ読み込み	
	
	serial1_send("current = \0");
	current = data_setting[0]*256+data_setting[1];
	current_char[0] = current/100+'0';
	current_char[1] = (current/10)%10+'0';
	current_char[2] = current%10+'0';
	buf[0]=current_char[0];
	buf[1]='.';
	buf[2]=current_char[1];
	buf[3]=current_char[2];
	serial1_send_n(buf, 4);
	serial1_send("A\r\n");
	if(current>700)
	{
		serial1_send("ERR\r\n");
		return;
	}
	current=(long)current*128/3/10; // 1/8Ω*I(A)/1.2V*4096 = I(10mA)*1280/3/10
	
	serial1_send("delta_peak = \0");
	delta_peak = data_setting[2];
	buf[0]=delta_peak/10+'0';
	buf[1]=delta_peak%10+'0';
	serial1_send_n(buf, 2);
	serial1_send("mV\r\n");
	delta_peak = delta_peak*32768/2048/4;
	
	serial1_send("peak_delay = \0");
	peak_delay = data_setting[3]*256+data_setting[4];
	buf[0]=peak_delay/100+'0';
	buf[1]=(peak_delay/10)%10+'0';
	buf[2]=peak_delay%10+'0';
	serial1_send_n(buf, 3);
	serial1_send("sec\r\n");
	peak_delay = peak_delay*8;
	
	serial1_send("stop_voltage = \0");	
	voltage = data_setting[5]*256+data_setting[6];
	buf[0]=voltage/1000+'0';
	buf[1]='.';
	buf[2]=(voltage/100)%10+'0';
	buf[3]=(voltage/10)%10+'0';
	buf[4]=voltage%10+'0';
	serial1_send_n(buf, 5);
	serial1_send("V\r\n");
	voltage = voltage*32768/2048/4;
	
	serial1_send("cv_trickle_mode = \0");	
	cv_trickle_mode = data_setting[7];
	switch(cv_trickle_mode)
	{
		case 0:
			serial1_send("CC + OVP\r\n");
			break;
		case 1:
			serial1_send("CV -> STOP\r\n");
			break;
		case 2:
			serial1_send("CV -> TRICKLE\r\n");
			break;
		default:
			break;
	}


	serial1_send("stop current = \0");
	stop_current = data_setting[8]*256+data_setting[9];
	buf[0] = stop_current/100+'0';
	buf[1] = '.';
	buf[2] = (stop_current/10)%10+'0';
	buf[3] = stop_current%10+'0';
	serial1_send_n(buf, 4);
	serial1_send("A\r\n");
	stop_current=(long)stop_current*128/3/10; // 1/8Ω*I(A)/1.2V*4096 = I(10mA)*1280/3/10

	serial1_send("current decrease speed = \0");
	dec_speed = data_setting[10];
	buf[0] = dec_speed/10+'0';
	buf[1] = dec_speed%10+'0';
	serial1_send_n(buf, 2);
	serial1_send("\r\n");
		
	
	serial1_send("capacity = \0");	
	capacity = data_setting[11]*256+data_setting[12];
	buf[0] = capacity/1000+'0';
	buf[1] = (capacity/100)%10+'0';
	buf[2] = (capacity/10)%10+'0';
	buf[3] = capacity%10+'0';
	serial1_send_n(buf, 4);
	serial1_send("mAh\r\n");
	capacity = capacity*128/3/100 *3600*8; // 1/8Ω*I(A)/1.2V*4096 = I(1mA)*1280/3/10



	serial1_send("/////////////////\r\n\r\n");
	
	serial1_send("[sec], [A], [V], [mAh], [A]\r\n\r\n");
	serial1_send("time, current, voltage, capacity, current_monitor\r\n\r\n");



	da_out(0,current);
	da_out(1,0);
	if(cap_mode == 0)
	{
		capacity_monitor=0;
	}
	voltage_max=0;
	time_cnt = 0;
	//p2 = 0b11100000;

//	lcd_display(0x80, "0.000V  0000mAh ", 16);
//	lcd_display(0xC0, "0.00A\0", 16);

	
	traic = 0b00000101;		//i2c割り込みレベル5
	asm("FSET I");			//割り込み許可

	while(mode!=MODE_MENU){}
	asm("FCLR I");			//割り込み禁止
	if(seq_mode == 0)
	{
		display_menu(status);
	}
	
	change_led(LED_OFF);
}

void discharge(void)
{
	int i;

	mode = MODE_DIS;
	change_led(LED_ON);
	serial1_send("\r\n");
	serial1_send("discharge start\r\n");
	serial1_send("/////setting/////\r\n");

//データ読み込み	
	serial1_send("current = \0");
	current = data_setting[0]*256+data_setting[1];
	current_char[0] = current/100+'0';
	current_char[1] = (current/10)%10+'0';
	current_char[2] = current%10+'0';
	buf[0]=current_char[0];
	buf[1]='.';
	buf[2]=current_char[1];
	buf[3]=current_char[2];
	serial1_send_n(buf, 4);
	serial1_send("A\r\n");
	if(current>700)
	{
		serial1_send("ERR\r\n");
		return;
	}
	current=(long)current*128/3/10; // 1/8Ω*I(A)/1.2V*4096 = I(10mA)*1280/3/10

	serial1_send("stop_voltage = \0");
	voltage = data_setting[2]*256+data_setting[3];
	buf[0]=voltage/1000+'0';
	buf[1]='.';
	buf[2]=(voltage/100)%10+'0';
	buf[3]=(voltage/10)%10+'0';
	buf[4]=voltage%10+'0';
	serial1_send_n(buf, 5);
	serial1_send("V\r\n");
	voltage = voltage*32768/2048/4;



	shibori = data_setting[4];
	if(shibori == 0)
	{
		serial1_send("shibori = OFF\r\n");
	}else
	{
		serial1_send("shibori = ON\r\n");
	}
	
	serial1_send("current decrease speed = \0");
	dec_speed = data_setting[5];
	buf[0] = dec_speed/10+'0';
	buf[1] = dec_speed%10+'0';
	serial1_send_n(buf, 2);
	serial1_send("\r\n");

	serial1_send("stop current = \0");
	stop_current = data_setting[6]*256+data_setting[7];
	buf[0] = stop_current/100+'0';
	buf[1] = '.';
	buf[2] = (stop_current/10)%10+'0';
	buf[3] = stop_current%10+'0';
	serial1_send_n(buf, 4);
	serial1_send("A\r\n");
	stop_current=(long)stop_current*128/3/10; // 1/8Ω*I(A)/1.2V*4096 = I(10mA)*1280/3/10
	
	serial1_send("capacity = \0");	
	capacity = data_setting[8]*256+data_setting[9];
	buf[0] = capacity/1000+'0';
	buf[1] = (capacity/100)%10+'0';
	buf[2] = (capacity/10)%10+'0';
	buf[3] = capacity%10+'0';
	serial1_send_n(buf, 4);
	serial1_send("mAh\r\n");
	capacity = capacity*128/3/100 *3600*8; // 1/8Ω*I(A)/1.2V*4096 = I(1mA)*1280/3/10
	


	serial1_send("/////////////////\r\n");
	
	serial1_send("[sec], [A], [V], [mAh], [A]\r\n");
	serial1_send("time, current, voltage, capacity, current_monitor\r\n");
	
	
	da_out(0,0);
	da_out(1,current);
	if(cap_mode == 0)
	{
		capacity_monitor=0;
	}
	mode_shibori = 0;
	time_cnt = 0;

	//p2 = 0b11100000;
	
//	lcd_display(0x80, "0.000V  0000mAh ", 16);
//	wait_50usec();

	//電流表示
/*mp[0]=current_char[0];
	tmp[1]='.';
	tmp[2]=current_char[1];
	tmp[3]=current_char[2];
	tmp[4]='A';
	tmp[0]='0';
	tmp[1]='.';
	tmp[2]='0';
	tmp[3]='0';
	tmp[4]='A';
	lcd_display(0xC0, tmp, 5);
	wait_1msec();
	asm("NOP");
	*/
	traic = 0b00000101;		//i2c割り込みレベル5
	
	asm("FSET I");			//割り込み許可
	
	while(mode!=MODE_MENU){}
	asm("FCLR I");			//割り込み禁止
	if(seq_mode == 0)
	{
		display_menu(status);
	}
	change_led(LED_OFF);
}

void sequence(void)
{

	unsigned int tmp_data;
	unsigned int i;
	unsigned char data_num_tmp;

	pc = 0;
	seq_mode = 1;
	data_num_tmp = data_num;

	serial1_send("\r\n");
	serial1_send("msg: seq start\r\n");
	
	while(1)
	{
		serial1_send("\r\n");
		serial1_send("msg: seq No.\0");
		buf[0] = data_num_tmp + '0';
		buf[1] = '-';
		buf[2] = data_to_chr(pc / 16);
		buf[3] = data_to_chr(pc % 16);
		buf[4] = '[';
		buf[5] = data_to_chr(data_seq[pc] / 16);
		buf[6] = data_to_chr(data_seq[pc] % 16);
		buf[7] = ']';
		buf[8] = '\r';
		buf[9] = '\n';
		serial1_send(buf);
	
	
		if(data_seq[pc] < 16)//CC
		{
			data_num = data_seq[pc] % 16;
			read_data(data_setting, BANK_A+data_num*16,16);
			serial1_send("msg: cc charge start\r\n");
			charge();
			serial1_send("\r\n");
			serial1_send("msg: cc charge end\r\n");
			pc++;

		}else if(data_seq[pc] < 48)//DIS
		{
			data_num = data_seq[pc] % 16;
			read_data(data_setting, BANK_A+data_num*16+512,16);
			serial1_send("msg: discharge start\r\n");
			discharge();
			serial1_send("\r\n");
			serial1_send("msg: discharge end\r\n");
			pc++;
		}else if((data_seq[pc] / 16) >= 3 && (data_seq[pc] / 16) < 7)//カウンタ命令
		{
			if((data_seq[pc] % 16) == 0)
			{
				seq_cnt[data_seq[pc] / 16-3] = 0;
				serial1_send("msg: cnt = 0\r\n");
				pc++;
				
			}else if((data_seq[pc] % 16) == 1)
			{
				seq_cnt[data_seq[pc] / 16-3] = data_seq[pc+1];
				serial1_send("msg: cnt = ");
				buf[0] = data_to_chr(data_seq[pc+1] / 16);
				buf[1] = data_to_chr(data_seq[pc+1] % 16);
				buf[2] = '\r';
				buf[3] = '\n';
				serial1_send(buf);
				pc += 2;
			}else if((data_seq[pc] % 16) == 4)
			{
				seq_cnt[data_seq[pc] / 16-3] += data_seq[pc+1];
				serial1_send("msg: cnt = cnt + ");
				buf[0] = data_to_chr(data_seq[pc+1] / 16);
				buf[1] = data_to_chr(data_seq[pc+1] % 16);
				buf[2] = '\r';
				buf[3] = '\n';
				serial1_send(buf);
				pc += 2;
			}else if((data_seq[pc] % 16) == 5)
			{
				seq_cnt[data_seq[pc] / 16-3] -= data_seq[pc+1];
				serial1_send("msg: cnt = cnt - ");
				buf[0] = data_to_chr(data_seq[pc+1] / 16);
				buf[1] = data_to_chr(data_seq[pc+1] % 16);
				buf[2] = '\r';
				buf[3] = '\n';
				serial1_send(buf);
				pc += 2;
			}else if((data_seq[pc] % 16) == 6)
			{
				serial1_send("msg: if(cnt == ");
				buf[0] = data_to_chr(data_seq[pc+1] / 16);
				buf[1] = data_to_chr(data_seq[pc+1] % 16);
				buf[2] = '\0';
				serial1_send(buf);
				serial1_send(") goto ");
				buf[0] = data_to_chr(data_seq[pc+2] / 16);
				buf[1] = data_to_chr(data_seq[pc+2] % 16);
				buf[2] = '\r';
				buf[3] = '\n';
				serial1_send(buf);
				if(seq_cnt[data_seq[pc] / 16-3] == data_seq[pc+1])
				{
					serial1_send("msg: TRUE\r\n");
					pc = data_seq[pc+2];
				}else{
					serial1_send("msg: FALSE\r\n");
					pc += 3;
				}
			}else if((data_seq[pc] % 16) == 9)
			{
				serial1_send("msg: if(cnt > ");
				buf[0] = data_to_chr(data_seq[pc+1] / 16);
				buf[1] = data_to_chr(data_seq[pc+1] % 16);
				buf[2] = '\0';
				serial1_send(buf);
				serial1_send(") goto ");
				buf[0] = data_to_chr(data_seq[pc+2] / 16);
				buf[1] = data_to_chr(data_seq[pc+2] % 16);
				buf[2] = '\r';
				buf[3] = '\n';
				serial1_send(buf);
				if(seq_cnt[data_seq[pc] / 16-3] > data_seq[pc+1])
				{
					serial1_send("msg: TRUE\r\n");
					pc = data_seq[pc+2];
				}else{
					serial1_send("msg: FALSE\r\n");
					pc += 3;
				}
			}
		}else if(data_seq[pc] == 0x70)//終了条件分岐
		{
			serial1_send("msg: if((end_flag and ");
			buf[0] = data_to_chr(data_seq[pc+1] / 16);
			buf[1] = data_to_chr(data_seq[pc+1] % 16);
			buf[2] = '\0';
			serial1_send(buf);
			serial1_send(") != 0) goto ");
			buf[0] = data_to_chr(data_seq[pc+2] / 16);
			buf[1] = data_to_chr(data_seq[pc+2] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);
			if((end_flag & data_seq[pc+1]) != 0)
			{
				serial1_send("msg: TRUE\r\n");
				pc = data_seq[pc+2];
			}else{
				serial1_send("msg: FALSE\r\n");
				pc += 3;
			}
		}else if(data_seq[pc] == 0x80)//電圧値分岐
		{
			serial1_send("msg: if(end_voltage == ");
			//比較値表示
			tmp_data = data_seq[pc+1] * 256 + data_seq[pc+2];
			buf[0] = tmp_data/1000;
			buf[2] = (tmp_data/100)%10;
			buf[3] = (tmp_data/10)%10;
			buf[4] = tmp_data%10;
			for(i=0;i<5;i++)
			{
			  buf[i] += 48;
			}
			buf[1] = '.';
			buf[5] = '\0';
			serial1_send(buf);
			serial1_send(") goto ");

			//飛び先表示
			buf[0] = data_to_chr(data_seq[pc+3] / 16);
			buf[1] = data_to_chr(data_seq[pc+3] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);

			//比較値表示2
			serial1_send("msg: end_voltage = ");
			buf[0] = voltage_monitor/4000;
			buf[2] = (voltage_monitor/400)%10;
			buf[3] = (voltage_monitor/40)%10;
			buf[4] = (voltage_monitor/4)%10;
			tmp_data = buf[0]*1000 + buf[2]*100 + buf[3]*10 + buf[4];
			for(i=0;i<5;i++)
			{
			  buf[i] += 48;
			}
			buf[1] = '.';
			buf[5] = '\r';
			buf[6] = '\n';
			serial1_send(buf);
			
			//分岐
			if(tmp_data == data_seq[pc+1]*256+data_seq[pc+2])
			{
				serial1_send("msg: TRUE\r\n");
				pc = data_seq[pc+3];
			}else{
				serial1_send("msg: FALSE\r\n");
				pc += 4;
			}
		}else if(data_seq[pc] == 0x83)//電圧値分岐
		{
			serial1_send("msg: if(end_voltage > ");
			//比較値表示
			tmp_data = data_seq[pc+1] * 256 + data_seq[pc+2];
			buf[0] = tmp_data/1000;
			buf[2] = (tmp_data/100)%10;
			buf[3] = (tmp_data/10)%10;
			buf[4] = tmp_data%10;
			for(i=0;i<5;i++)
			{
			  buf[i] += 48;
			}
			buf[1] = '.';
			buf[5] = '\0';
			serial1_send(buf);
			serial1_send(") goto ");

			//飛び先表示
			buf[0] = data_to_chr(data_seq[pc+3] / 16);
			buf[1] = data_to_chr(data_seq[pc+3] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);

			//比較値表示2
			serial1_send("msg: end_voltage = ");
			buf[0] = voltage_monitor/4000;
			buf[2] = (voltage_monitor/400)%10;
			buf[3] = (voltage_monitor/40)%10;
			buf[4] = (voltage_monitor/4)%10;
			tmp_data = buf[0]*1000 + buf[2]*100 + buf[3]*10 + buf[4];
			for(i=0;i<5;i++)
			{
			  buf[i] += 48;
			}
			buf[1] = '.';
			buf[5] = '\r';
			buf[6] = '\n';
			serial1_send(buf);
			
			//分岐
			if(tmp_data > data_seq[pc+1]*256+data_seq[pc+2])
			{
				serial1_send("msg: TRUE\r\n");
				pc = data_seq[pc+3];
			}else{
				serial1_send("msg: FALSE\r\n");
				pc += 4;
			}
		}else if(data_seq[pc] == 0x86)//電流値分岐
		{
			serial1_send("msg: if(end_current == ");
			//比較値表示
			tmp_data = data_seq[pc+1] * 256 + data_seq[pc+2];
			buf[0] = (tmp_data/100)%10;
			buf[2] = (tmp_data/10)%10;
			buf[3] = tmp_data%10;
			for(i=0;i<4;i++)
			{
			  buf[i] += 48;
			}
			buf[1] = '.';
			buf[4] = '\0';
			serial1_send(buf);
			serial1_send(") goto ");

			//飛び先表示
			buf[0] = data_to_chr(data_seq[pc+3] / 16);
			buf[1] = data_to_chr(data_seq[pc+3] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);

			//比較値表示2
			serial1_send("msg: end_current = ");
			buf[0]=current_char[0];
			buf[2]=current_char[1];
			buf[3]=current_char[2];
			tmp_data = (buf[0]-'0')*100 + (buf[2]-'0')*10 + buf[3]-'0';
			buf[1] = '.';
			buf[4] = '\r';
			buf[5] = '\n';
			serial1_send(buf);
			
			//分岐
			if(tmp_data == data_seq[pc+1]*256+data_seq[pc+2])
			{
				serial1_send("msg: TRUE\r\n");
				pc = data_seq[pc+3];
			}else{
				serial1_send("msg: FALSE\r\n");
				pc += 4;
			}
		}else if(data_seq[pc] == 0x89)//電流値分岐
		{
			serial1_send("msg: if(end_current > ");
			//比較値表示
			tmp_data = data_seq[pc+1] * 256 + data_seq[pc+2];
			buf[0] = (tmp_data/100)%10;
			buf[2] = (tmp_data/10)%10;
			buf[3] = tmp_data%10;
			for(i=0;i<4;i++)
			{
			  buf[i] += 48;
			}
			buf[1] = '.';
			buf[4] = '\0';
			serial1_send(buf);
			serial1_send(") goto ");

			//飛び先表示
			buf[0] = data_to_chr(data_seq[pc+3] / 16);
			buf[1] = data_to_chr(data_seq[pc+3] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);

			//比較値表示2
			serial1_send("msg: end_current = ");
			buf[0]=current_char[0];
			buf[2]=current_char[1];
			buf[3]=current_char[2];
			tmp_data = (buf[0]-'0')*100 + (buf[2]-'0')*10 + buf[3]-'0';

			buf[1] = '.';
			buf[4] = '\r';
			buf[5] = '\n';
			serial1_send(buf);
			
			//分岐
			if(tmp_data > data_seq[pc+1]*256+data_seq[pc+2])
			{
				serial1_send("msg: TRUE\r\n");
				pc = data_seq[pc+3];
			}else{
				serial1_send("msg: FALSE\r\n");
				pc += 4;
			}
		}else if(data_seq[pc] == 0x8C)//容量分岐
		{
			serial1_send("msg: if(end_capacity == ");
			//比較値表示
			tmp_data = data_seq[pc+1] * 256 + data_seq[pc+2];
			buf[0] = (tmp_data/1000)%10;
			buf[1] = (tmp_data/100)%10;
			buf[2] = (tmp_data/10)%10;
			buf[3] = tmp_data%10;

			buf[4] = '\0';
			serial1_send(buf);
			serial1_send(") goto ");

			//飛び先表示
			buf[0] = data_to_chr(data_seq[pc+3] / 16);
			buf[1] = data_to_chr(data_seq[pc+3] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);

			//比較値表示2
			serial1_send("msg: end_capacity = ");
			tmp_data = capacity_monitor / 3 / 4096;
			buf[0] = (tmp_data / 1000) % 10+'0';
			buf[1] = (tmp_data / 100) % 10+'0';
			buf[2] = (tmp_data / 10) % 10+'0';
			buf[3] = tmp_data % 10+'0';
			for(i=0;i<4;i++)
			{
			  buf[i] += 48;
			}
			buf[4] = '\r';
			buf[5] = '\n';
			serial1_send(buf);
			
			//分岐
			if(tmp_data == data_seq[pc+1]*256+data_seq[pc+2])
			{
				serial1_send("msg: TRUE\r\n");
				pc = data_seq[pc+3];
			}else{
				serial1_send("msg: FALSE\r\n");
				pc += 4;
			}
		}
		else if(data_seq[pc] == 0x8F)//容量分岐
		{
			serial1_send("msg: if(end_capacity > ");
			//比較値表示
			tmp_data = data_seq[pc+1] * 256 + data_seq[pc+2];
			buf[0] = (tmp_data/1000)%10;
			buf[1] = (tmp_data/100)%10;
			buf[2] = (tmp_data/10)%10;
			buf[3] = tmp_data%10;
			for(i=0;i<4;i++)
			{
			  buf[i] += 48;
			}
			buf[4] = '\0';
			serial1_send(buf);
			serial1_send(") goto ");

			//飛び先表示
			buf[0] = data_to_chr(data_seq[pc+3] / 16);
			buf[1] = data_to_chr(data_seq[pc+3] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);

			//比較値表示2
			serial1_send("msg: end_capacity = ");
			tmp_data = capacity_monitor / 3 / 4096;
			buf[0] = (tmp_data / 1000) % 10+'0';
			buf[1] = (tmp_data / 100) % 10+'0';
			buf[2] = (tmp_data / 10) % 10+'0';
			buf[3] = tmp_data % 10+'0';


			buf[4] = '\r';
			buf[5] = '\n';
			serial1_send(buf);
			
			//分岐
			if(tmp_data > data_seq[pc+1]*256+data_seq[pc+2])
			{
				serial1_send("msg: TRUE\r\n");
				pc = data_seq[pc+3];
			}else{
				serial1_send("msg: FALSE\r\n");
				pc += 4;
			}
		}else if(data_seq[pc] == 0xA0)//wait_sec
		{
			serial1_send("msg: wait_sec ");
			buf[0] = data_to_chr(data_seq[pc+1] / 16);
			buf[1] = data_to_chr(data_seq[pc+1] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);
			mode = MODE_WAIT;
			traic = 0b00000101;		//i2c割り込みレベル5
			time_cnt = 0;
			asm("FSET I");			//割り込み禁止
			while((time_cnt/8) < (data_seq[pc+1])){
			}
			asm("FCLR I");			//割り込み禁止
			mode = MODE_MENU;
			pc +=2;
		}else if(data_seq[pc] == 0xA1)//wait_min
		{
			serial1_send("msg: wait_min ");
			buf[0] = data_to_chr(data_seq[pc+1] / 16);
			buf[1] = data_to_chr(data_seq[pc+1] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);
			mode = MODE_WAIT;
			traic = 0b00000101;		//i2c割り込みレベル5
			time_cnt = 0;
			asm("FSET I");			//割り込み禁止
			while((time_cnt/8/60) < (data_seq[pc+1])){
			}
			asm("FCLR I");			//割り込み禁止
			mode = MODE_MENU;
			pc +=2;
		}else if(data_seq[pc] == 0xA2)//cap accumulate mode on
		{
			serial1_send("msg: cap accumulate mode on\r\n");
			cap_mode = 1;
			pc +=1;
		}else if(data_seq[pc] == 0xA3)//cap accumulate mode off
		{
			serial1_send("msg: cap accumulate mode off\r\n");
			cap_mode = 1;
			pc +=1;
		}else if(data_seq[pc] == 0xA4)//cap reset
		{
			serial1_send("msg: cap reset\r\n");
			capacity_monitor = 0;
			pc +=1;
			
		}else if(data_seq[pc] == 0xB0)//goto
		{
			serial1_send("msg: goto ");

			//飛び先表示
			buf[0] = data_to_chr(data_seq[pc+1] / 16);
			buf[1] = data_to_chr(data_seq[pc+1] % 16);
			buf[2] = '\r';
			buf[3] = '\n';
			serial1_send(buf);

			pc = data_seq[pc+1];
		}else if((data_seq[pc] == 0xC0) || (data_seq[pc] == 0xFF))//end
		{
			serial1_send("msg: end\r\n");
			break;
		}else{
			serial1_send("msg: nop\r\n");
			pc++;
		}
		wait_50usec();
		
		buf[0] = '\0';
		lcd_display(0xC0, buf, 16);
		lcd_display(0xC8, buf, 16);
	}
	pc = 0;
	seq_mode = 0;
	display_menu(status);
	serial1_send("\r\n");
	serial1_send("msg: seq end");
	
}


void bat_test(void)
{
	unsigned long tmp1;
	//ad bat読み込み
	tmp1 = ad_read_internal(0);
	tmp1 = tmp1 * 375 / 256;
	
	//電流モニタ
	buf[0] = tmp1 / 100 + '0';
	buf[1] = '.';
	buf[2] = (tmp1 / 10) % 10 + '0';
	buf[3] = tmp1 % 10 + '0';
	buf[4] = 'V';
	buf[5] = '\0';
		
	lcd_display(0xC8, buf, 0);
	
}

void data_erase(void)
{
	int i;
	unsigned int adr;
	
	
	lcd_display(0xC0, "ERASE DATA\0", 16);
	fmr0 = 0b00000001;
	asm(" ");
	
	fmr0 = 0b00000011;//プロテクト解除
	fmr1 = 0b10000000;//EW1モード
	fmr1 = 0b10000010;//EW1モード
	asm(" ");
	adr = BANK_A;
	*(signed char *)adr = 0x20;
	*(signed char *)adr = 0xd0;
	while((fmr0 & 0b00000001) == 0)//イレーズ待ち
	{
	}
	if((fmr0 & 0b10000000) != 0)
	{
		//エラー
	}
	*(signed char *)adr = 0x50;
	
	adr = BANK_B;
	*(signed char *)adr = 0x20;
	*(signed char *)adr = 0xd0;
	while((fmr0 & 0b00000001) == 0)//イレーズ待ち
	{
	}
	if((fmr0 & 0b10000000) != 0)
	{
		//エラー
	}

	*(signed char *)adr = 0x50;
	
	*(signed char *)adr = 0xFF;


	fmr0 = 0b00000001;//プロテクト
	lcd_display(0xC0, "ERASE END\0", 16);
}


void current_to_current_char(void)
{
	unsigned int tmp;
	current_char[0] = current*3*10/128 / 100 + '0';
	tmp = current * 3 * 10 / 128 % 100;
	current_char[1] = tmp / 10 + '0';
	current_char[2] = tmp % 10 + '0';
	

}

void current_char_to_current(void)
{
	current = (current_char[0]-'0')*100+(current_char[1]-'0')*10+(current_char[2]-'0');
	
	current=(long)current*128/3/10; // 1/8Ω*I(A)/1.2V*4096 = I(10mA)*1280/3/10
	
}


int ad_read_internal(int ch)
{
	while((adcon0 & 0b01000000) != 0)//adst==1
	{
	}
	switch(ch)
	{
		case 0:
			adcon0 = 0b10000000;
			break;
		case 1:
			adcon0 = 0b10000001;
			break;
		case 2:
			adcon0 = 0b10000010;
			break;
		case 3:
			adcon0 = 0b10000011;
			break;
		default:
		  break;
	}
	adcon1 = 0b00101000;
	adcon2 = 0b00000001;
	
	
	adcon0 |= 0b01000000;

	while((adcon0 & 0b01000000) != 0)//adst==1
	{
	}
	return ad;
}

void display_setting(char num, char point, unsigned char hi, unsigned char lo, char *unit)
{
	//num 桁数
	//point　小数点位置　0で小数点なし
	//hi　上位バイト
	//lo　下位バイト
	//unit 単位の文字列　\0を入れる

	int i;
	int j;//表示位置
	int tmp;
	
	j = 0;
	tmp = hi * 256 + lo;
	for( i = 0; i < num; i++ )
	{
		buf[j] = tmp / (((num-i)==1)?1:((num-i)==2)?10:((num-i)==3)?100:1000);
		tmp = tmp % (((num-i)==1)?1:((num-i)==2)?10:((num-i)==3)?100:1000);
		buf[j] += '0';
		j++;
		if(point == j)
		{
			buf[j] = '.';
			j++;
		}
	}
	i = 0;
	while(unit[i] != '\0')
	{
		buf[j+i] = unit[i];
		i++;
	}
	buf[j+i] = '\0';
	lcd_display(0xC0, buf, 16);
	
	if(point != 0 && setting_place >= point)
	{
		lcd_set_address(0xC0+setting_place+1);
	}else
	{
		lcd_set_address(0xC0+setting_place);
	}
	
}

void data_check(char *data)
{
	int i;
	for( i=0; i<16;i++)
	{
		if(*data == 0xFF)
		{
			*data = 0;
		}
		data++;
	}
}

void change_setting(char num, char point, unsigned char *hi, unsigned char *lo, int max, int min, char *unit)
{
	//num 桁数
	//point　小数点位置　0で小数点なし
	//hi　上位バイト
	//lo　下位バイト
	//min max 制限
	int data;
	int data_old;

	
	if(setting_mode == EDIT_MODE)
	{
		data = *hi*256+*lo;
		if(data>max || data<min)
		{
			data=0;
		}
		data_old = data;
		if((sw_data & SW_UP) != 0)
		{
			data += (((num-setting_place)==1)?1:(((num-setting_place)==2)?10:(((num-setting_place)==3)?100:1000)));
			if(data > max || data_old > data)
			{
				data = max;
			}
			*hi = data/256;
			*lo = data%256;

			display_setting(num, point, *hi, *lo, unit);

		}else if((sw_data & SW_DOWN) != 0)
		{
			data -= (((num-setting_place)==1)?1:(((num-setting_place)==2)?10:(((num-setting_place)==3)?100:1000)));
			if(data < min || data_old < data)
			{
				data = min;
			}
			*hi = data/256;
			*lo = data%256;

			display_setting(num, point, *hi, *lo, unit);
		}else if((sw_data & SW_RIGHT) != 0)
		{
			if(setting_place == num-1)
			{
				edit_mode_off();
			}else
			{
				setting_place++;
				if(point != 0 && setting_place >= point)
				{
					lcd_set_address(0xC0+setting_place+1);
				}
				else
				{
					lcd_set_address(0xC0+setting_place);
				}
			}
		}
	}else
	{
		if((sw_data & SW_RIGHT) != 0)
		{
			edit_mode_on();
		}
	}
}
