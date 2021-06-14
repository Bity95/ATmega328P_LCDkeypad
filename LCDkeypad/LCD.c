/*
 * LCD.c
 *
 * Created: 12.03.2021 9:08:51
 *  Author: jakub
 */ 

#include "LCD.h"
#include "timer.h"

#define RS (1<<DDB0)
#define ENABLE (1<<DDB1)
#define DB7 (1<<DDD7)
#define DB6 (1<<DDD6)
#define DB5 (1<<DDD5)
#define DB4 (1<<DDD4)

void writeLCD(bool select,uint8_t data)
{
	PORTD &= 0x0f;
	PORTB |= ENABLE;
	if(select)
		PORTB |= RS;
	else
		PORTB &= ~RS;
	PORTD |= data & 0xf0;
	busyDelay(10);
	PORTB &= ~ENABLE;
	busyDelay(10);
	PORTD &= 0x0f;
	busyDelay(5);
	PORTB |= ENABLE;
	PORTD |= (data<<4) & 0xf0;
	busyDelay(10);
	PORTB &= ~ENABLE;
	busyDelay(40);//doba potrebna pro data write
}

void clearLCD(void)
{
	writeLCD(false, 1);
	busyDelay(1640);//cekani na vykonani instrukce 1,64ms
}

void shiftLCD(bool l)
{
	if(l)
		writeLCD(false, 0b00011000);//cekani 40us soucasti zapisu
	else
		writeLCD(false, 0b00011100);
}

void returnHome(void)
{
	writeLCD(false, 0b00000010);
	busyDelay(1640);//cekani na vykonani instrukce 1,64ms
}

void onoffLCD(bool on, bool curs)
{
	if(on)
		if(curs)
			writeLCD(false, 0b00001111);//cekani 40us soucasti zapisu
		else
			writeLCD(false, 0b00001100);
	else
		writeLCD(false, 0b00001010);
}

void entryMode(bool increase, bool shift)
{
	if(increase && shift)
		writeLCD(false, 0b00000111);//cekani 40us soucasti zapisu
	else if(increase && !shift)
		writeLCD(false, 0b00000110);
	else if(!increase && shift)
		writeLCD(false, 0b00000101);
	else
		writeLCD(false, 0b00000100);
}

void setAddress(bool radek, uint8_t pozice)//pozice cislovane od nuly
{
	if(pozice > 40)
		pozice = 40;
	pozice += 0b10000000;
	if(radek)
		writeLCD(false, pozice);//cekani 40us soucasti zapisu
	else
	{
		pozice += 0x40;
		writeLCD(false, pozice);
	}
}

void cursorShift(bool l)
{
	if(l)
		writeLCD(false, 0b00010000);//cekani 40us soucasti zapisu
	else
		writeLCD(false, 0b00010100);
}

void lightOn(bool on)
{
	if(on)
		PORTB |= 0b00000100;
	else
		PORTB &= 0b11111011;
}

void initLCD(void)
{
	DDRB |= RS|ENABLE;//nastaveni na vystup
	DDRB |= (1<<DDB2);//pro vypnuti podsviceni displeje
	DDRD |= DB4|DB5|DB6|DB7;//nastaveni na vystup D4 - D7
	
	//inicializace displeje pomoci instrukci
	busyDelay(40000);//cekani na power up displeje 40ms
	//step 1
	PORTB |= ENABLE;
	PORTB &= ~RS;
	PORTD = DB5|DB4;
	busyDelay(10);
	PORTB &= ~ENABLE;//1.odeslani
	busyDelay(10);
	PORTB |= ENABLE;
	
	busyDelay(5000);
	PORTB &= ~ENABLE;//2.odeslani
	busyDelay(10);
	PORTB |= ENABLE;
	
	busyDelay(150);
	PORTB &= ~ENABLE;//3. odeslani
	busyDelay(10);
	//inicializovano
	PORTB |= ENABLE;
	PORTB &= ~RS;
	PORTD = DB5;
	busyDelay(10);
	PORTB &= ~ENABLE;
	busyDelay(10);
	
	//nastaveni 4 bitove komunikace a 2 radkove zobrazovani
	writeLCD(false, 0b00101000);//40us cekani soucasti zapisu
	
	//display on
	onoffLCD(true, false);
	
	//entry mode
	entryMode(true, false);
	
	clearLCD();
	lightOn(true);
}

