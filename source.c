#include <p18f452.h>
#pragma config WDT = OFF

#define RS_PIN	LATCbits.LATC0
#define RW_PIN	LATCbits.LATC1
#define EN_PIN	LATCbits.LATC2
#define HIGH	1
#define LOW 	0


enum button
{
	FIRST_PRESS,
	SECOND_PRESS
}buttonState = FIRST_PRESS;


unsigned char state = 0;
const rom unsigned char str[] = "WELCOME TO LCD";

void delay(const unsigned char, const unsigned char, const unsigned char);
void lcdInitialization(void);
void command(unsigned char, unsigned char);
void data(unsigned char);
void busyFlag(void);

#pragma interrupt function
void function(void)
{
	if(INTCONbits.INT0IF)
	{
		INTCONbits.INT0IF = 0;	
		if(buttonState == FIRST_PRESS)
		{
			buttonState = SECOND_PRESS;
			state = 10;
		}
		else
		{
			buttonState = FIRST_PRESS;
			state = 20;
		}
	}
}

#pragma code vectorMap = 0x00008
void vectorMap(void)
{
	_asm
		GOTO function
	_endasm
}
#pragma code

#pragma code main = 0x000F0
void main(void)
{	
	unsigned char i = 0;
	TRISD = 0x00;
	TRISC = 0xF8;
	TRISBbits.TRISB0 = 1;
	INTCONbits.GIE = 1;
	INTCONbits.INT0IE = 1;
	INTCONbits.INT0IF = 0;
	INTCON2bits.INTEDG0 = 0;
	lcdInitialization();
	while(1)
	{
		if(state == 10)		
		{
			i = 0;
			while(str[i] != '\0')
				data(str[i++]);
			state = 0;
		}
		else if(state == 20)
		{
			i = 0;
			command(0x80, 1);
			while(i < 16)
			{
				data(0x20);
				++i;
			}
			command(0x80, 1);
			state = 0;
		}
	}		
}
#pragma code

void lcdInitialization(void)
{
	delay(0x01, 0x0B, 0xDC);
	command(0x30, 0);
	delay(0x01, 0x0B, 0xDC);
	command(0x0F, 0);
	delay(0x01, 0x0B, 0xDC);
	command(0x01, 0);
	delay(0x01, 0x0B, 0xDC);
}

void delay(const unsigned char DATA, const unsigned char HIGH_BYTE, const unsigned char LOW_BYTE)
{
	T0CON = DATA;
	TMR0H = HIGH_BYTE;	
	TMR0L = LOW_BYTE;
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 1;
	while( INTCONbits.TMR0IF == 0 );
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 0;
}
void command(unsigned char data, unsigned char flag)
{
	LATD = data;
	RS_PIN = LOW;
	RW_PIN = LOW;
	EN_PIN = 1;
	EN_PIN = 0;
	if(flag)
		busyFlag();
}
void data(unsigned char data)
{
	LATD = data;
	RS_PIN = HIGH;
	RW_PIN = LOW;
	EN_PIN = 1;
	EN_PIN = 0;
	busyFlag();
}
void busyFlag(void)
{
	RS_PIN = 0;
	RW_PIN = 1;
	TRISDbits.TRISD7 = 1;
	while(1)
	{
		EN_PIN = 1;
		if(PORTDbits.RD7 == 0)
		{
			EN_PIN = 0;
			break;
		}
		EN_PIN = 0;
	}
	TRISDbits.TRISD7 = 0;
}