/*
 * timer.h
 *
 * Created: 3.3.2021 10:21:31
 *  Author: Jakub
 */ 

#pragma once

void timerInit(void);
uint32_t getTime(void);

void busyDelay(uint32_t us);
void delay(uint32_t us);