/* 
 * File:   GPS_DUALUART_Parsing.h
 * Author: M.Junaid Aslam
 *
 * Created on February 26, 2016, 2:20 AM
 */

#ifndef GPS_DUALUART_PARSING_H
#define	GPS_DUALUART_PARSING_H

#include "uart_45k22.h"
#include "LCD_4bit.h"
#include "Stepper_Drive.h"
#include "TIMER_45k222.h"
#include "ADXL335_ADC_45K22.h"


void ProcessNMEA();                 //Tokenize the stream
void Shift(unsigned char data);     //Circular Buffer

void RxI1();                        //Receive from UART 1 on Interrupt
void RxI2();                        //Receive from UART 2 on Interrupt

volatile unsigned BYTE byte1;       //Contains data coming from UART 1
volatile unsigned BYTE byte2;       //Contains data coming from UART 2
volatile bit int1flag = 0;          //flag for UART 1 interrupt
volatile bit int2flag = 0;          //flag for UART 2 interrupt
volatile bit eol = 0;               //Represents end of line in tokenizing
volatile unsigned BYTE strbuf[28];  //Circular Buffer
volatile unsigned int count = 6;    //counts commas

void RxI1() {
    switch(byte1) {
        case 'a':                   //Start Receiving data from GPS and direct to Android
            RCSTA2bits.CREN = 1;
            second_line();
            dis_string("Start GPS       ", 16);
            break;
        case 'b':                   //Stop Receiving data from GPS stop directing to Android;
            RCSTA2bits.CREN = 0;
            second_line();
            dis_string("Stop GPS        ", 16);
            break;
        case 's':                   //Start Receiving from Android for Stepper Motor
            if(RCSTA2bits.CREN == 0)
                rx_position_en = 0b1;
            break;
        case 't':                   //Start Air Compression and timer
            Timer0_Init();
            break;
        case 'd':
            ADXL_Init();
            break;
        default:                    //Process data other than above cases
            if(rx_position_en) {
                ProcessStepperData(byte1);
            }
            break;
            
    }
    while(PIR1bits.RC1IF==1);
    int1flag = 0;                   //Tasks required by android command are initiated
}

void ProcessNMEA() {
    if(byte2 == '$') {
        count = 0;
        eol = 0;
    }
    else if(byte2 == ',')
        count = count+1; 
    if(count>1 && count<6 && byte2 != ',')
        Shift(byte2); 
    if((count == 3 || count == 4 || count == 5) && (byte2 == ','))
        Shift(','); 
    if(count == 6 && eol == 0) {
        eol = 1;
        Shift('\r');
        Shift('\n');
    }
    while(PIR3bits.RC2IF==1);
}

void Shift(char data) {
    TXREG1 = strbuf[27];
    unsigned int i=0;
    for(i=0 ; i<27 ; i++) {
        strbuf[27-i] = strbuf[27-i-1];
    }
    strbuf[0] = data;
}


void RxI2() {
    ProcessNMEA();
    int2flag = 0;
}

#endif	/* GPS_DUALUART_PARSING_H */