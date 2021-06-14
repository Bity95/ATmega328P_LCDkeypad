/*
 * LCD.h
 *
 * Created: 12.03.2021 9:09:15
 *  Author: jakub
 */ 

#pragma once

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>

void initLCD(void);

void clearLCD(void);	/**vymaza obsah pameti displeje*/

void shiftLCD(bool l);	/**pri zapsani hodnoty true probiha shift doleva
						pri zapisu false probiha shift doprava
						vzdy shift o jeden znak*/

void writeLCD(bool select,uint8_t data);/**zapisuje data do displeje,
										select urèuje zda je o pøíkaz - false
										nebo o zápis znaku - true*/

void returnHome(void);/**vrati kurzor na pozi 0x00*/

void onoffLCD(bool on, bool curs);/**pri zapisu true zapina displej, zapis false displej vypne
									zapis curs true zobrazuje kurzor na displeji*/

void entryMode(bool increase, bool shift);	/**increase urèuje smìr zápusi adres
											true odecitani adres, false pricitani adres
											shift zapina/vypina posun kurzoru pri posunu displeje*/

void setAddress(bool radek, uint8_t pozice);/**pokud je radek true zapisuje se na prvni radek,
											   pokud false zapisuje se na druhy radek
											   pozice se pocitaji od nuly
											   maximalni pozice je 40*/

void lightOn(bool on);/**hodnota true zapíná podvsícení, false vypíná podsvícení*/

void cursorShift(bool l);/** hodnota true posune kurzor o jeden znak doleva*/

static int lcd_putchar(char c, FILE *stream)
{
	writeLCD(true, c);
	return 0;
}

static FILE display = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
