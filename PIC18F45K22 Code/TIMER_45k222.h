/* 
 * File:   TIMER_45k222.h
 * Author: M.Junaid Aslam
 *
 * Created on April 13, 2016, 8:51 PM
 */

#ifndef TIMER_45K222_H
#define	TIMER_45K222_H
#include "LCD_4bit.h"

#define comp_pin_d TRISDbits.TRISD0
#define comp_pin LATDbits.LATD0

int second;
int minute;
unsigned char time_lcd[5];
volatile bit comp_tmr_en = 0;

void Timer0_Init1();
void Timer0_Init2();
void Compressor_Timer();
void second_inc();
void display_downtime();
void display_uptime();
void Reload_Timer();

void Timer0_Init() {
    first_line();
    dis_string("Compressing Air ", 16);
    INTCONbits.TMR0IF = 0;          //Clear Timer Interrupt Flag bit    
    T0CONbits.T08BIT = 0;           //Configure 16 bit timer
    T0CONbits.T0CS = 0;             //User internal oscillator as clock
    T0CONbits.PSA = 0;              //Use Prescaler
    T0CONbits.T0PS = 0b111;         //Set 1:256 Prescalar
    second = 0;
    minute = 0;
    TMR0H = 0xC2;                   //For 1s on 16 MHz Crystal
    TMR0L = 0xF7;
    INTCONbits.TMR0IE = 1;          //Enable timer0 interrupt
    
    comp_tmr_en = 1;
    T0CONbits.TMR0ON = 1;
}

void Compressor_Timer() {
    
    if(second < 10)             //35 for 35psi, change after proper calculations
        comp_pin = 1;
    else {
        comp_pin = 0;
        T0CONbits.TMR0ON = 0;
        INTCONbits.TMR0IE = 0;
        comp_tmr_en = 0;
        second = 0;
        second_line();
        tx1_byte('d');
        tx1_byte('d');
        tx1_byte('d');
        tx1_byte_n('d');
        dis_string("Done!        ", 13);
        delay();
        clr_scr();
        first_line();
        dis_string("Waiting for CMD ",16);
    }
    
}

void second_inc() {
    second++;
    if(second > 59) {
        minute++;
        second = 0;
    }
    if(minute > 59)
        minute = 0;
    display_downtime();
}

void display_downtime() {
    second_line();
    dis_string("TIME:", 5); 
    time_lcd[2] = ':';
    lcd_right_shift(0);
    time_lcd[0] = ((minute / 10) % 10) + 48;
    time_lcd[1] = (minute % 10) + 48;
    time_lcd[3] = (((10-second) / 10) % 10) + 48;  //10-seconds for compressor countdown
    time_lcd[4] = ((10-second) % 10) + 48;
    dis_string(time_lcd, 5);
}

void display_uptime() {
    second_line();
    dis_string("TIME:", 5); 
    time_lcd[2] = ':';
    lcd_right_shift(0);
    time_lcd[0] = ((minute / 10) % 10) + 48;
    time_lcd[1] = (minute % 10) + 48;
    time_lcd[3] = ((second / 10) % 10) + 48; 
    time_lcd[4] = (second % 10) + 48;
    dis_string(time_lcd, 5);
}

void Reload_Timer() {
    TMR0H = 0xC2;
    TMR0L = 0xF7;
}

#endif	/* TIMER_45K222_H */