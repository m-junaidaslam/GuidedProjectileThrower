/* 
 * File:   Stepper_Drive.h
 * Author: M.Junaid Aslam
 *
 * Created on February 26, 2016, 3:11 AM
 */

#ifndef STEPPER_DRIVE_H
#define	STEPPER_DRIVE_H

#include "LCD_4bit.h"
#include "uart_45k22.h"

#define A4_D TRISAbits.TRISA4		// Configure Port B as output port
#define B5_D TRISAbits.TRISA5       // Configure Port B as output port
#define ABAR6_D TRISAbits.TRISA6		// Configure Port B as output port
#define BBAR7_D TRISAbits.TRISA7		// Configure Port B as output port
#define A4 PORTAbits.RA4
#define B5 PORTAbits.RA5
#define ABAR6 PORTAbits.RA6
#define BBAR7 PORTAbits.RA7

volatile unsigned BYTE state_pos_full = 0;
volatile unsigned BYTE state_neg_full = 0;
volatile unsigned BYTE state_pos_half = 0;
volatile unsigned BYTE state_neg_half = 0;
volatile unsigned BYTE state = 0;
unsigned long last_position = 0;
unsigned int last_status = 0;
volatile bit rstflag = 0;
unsigned char rx_position[4];     //for stepper motor
unsigned long abs_position;
volatile bit rx_position_en = 0;
unsigned int i;
unsigned int j;


void Stepper_Init(void);
void ProcessStepperData(unsigned BYTE data1);
void delay(void);
void FullStepsPos(unsigned int steps);   //For Clockwise Full Step Drive
void FullStepsNeg(unsigned int steps);   //For Anti-Clockwise Full Step Drive
void HalfStepsPos(int steps);   //For Clockwise Half Step Drive
void HalfStepsNeg(int steps);   //For Anti-Clockwise Half Step Drive
void ProcessSteps(long position);

void Stepper_Init(void) {
    A4_D = 0;
    B5_D = 0;
    ABAR6_D = 0;
    BBAR7_D = 0;
    A4 = 0;
    B5 = 0;
    ABAR6 = 0;
    BBAR7 = 0;
}

void ProcessStepperData(unsigned BYTE data1) {
    if(data1 != 'x') {    
        rx_position[i] = data1;
        i++;
    } else if(data1 == 'x') {
        rx_position_en = 0;
        INTCONbits.GIE = 0;
        for(j=i;j<4;j++)
            rx_position[j] = 0;
        clr_scr();
        first_line();
        dis_string("Positioning Ang ", 16);
        second_line();
        dis_string(rx_position, 4);
        abs_position = ((rx_position[0]-48)*1000)+((rx_position[1]-48)*100)+((rx_position[2]-48)*10)+(rx_position[3]-48);
        ProcessSteps(abs_position);
        tx1_byte('d');
        tx1_byte('d');
        tx1_byte('d');
        tx1_byte_n('d');
        dis_char('d');        
        i=0;
        j=0;
        INTCONbits.GIE = 1;
        clr_scr();
        first_line();
        dis_string("Waiting for CMD ",16);
    }
}

void delay(void) {        //For Practical Motor
    int x=0;
    int y=0;
    for(x=0;x<255;x++) 
        for(y=0;y<7;y++)
            NOP();
}

/*void delay(void) {          //For Simulation Motor
    int i=0;
    int j=0;
    int k=0;
    for(i=0;i<255;i++)   
        for(j=0;j<300;j++)
            NOP();
}*/

/*unsigned int x,y,z;

void delay(void) {          //For Theeties
    int x=0;
    int y=0;
    for(x=0;x<200;x++)   
        for(y=0;y<300;y++)
            NOP();
}*/

void ProcessSteps(long position) {
    if(position == last_position)
        NOP();
    else if(position > last_position) {
        //if((position - last_position) > (100-position+last_position))     //For simulation
            //FullStepsNeg(100-position+last_position);     //For simulation
        
        if((position - last_position) > (3600-position+last_position))     //For real
            FullStepsNeg(3600-position+last_position);     //For real
        else
            FullStepsPos(position - last_position);
    } else if(last_position > position) {
        //if((last_position - position) > (100-last_position+position))    //For simulation
            //FullStepsPos(100-last_position+position);                    //For simulation
        
        if((last_position - position) > (3600-last_position+position))    //For real
            FullStepsPos(3600-last_position+position);                    //For real
        else
            FullStepsNeg(last_position - position);
    }
    last_position = position;
}

void GetReset() {
    //dis_string("Resetting",9);
    rstflag = 1;
    state_pos_full = 0;
    last_status = 1;
    
    while(rstflag){
        switch(state_pos_full) {
            case 0:
                BBAR7=1;ABAR6=0;B5=0;A4=0; 
                delay();
                state_pos_full = 1;
                break;
            case 1:
                BBAR7=0;ABAR6=0;B5=0;A4=1; 
                delay();
                state_pos_full = 2;
                break;
            case 2:
                BBAR7=0;ABAR6=0;B5=1;A4=0; 
                delay();
                state_pos_full = 3;
                break;
            case 3:
                BBAR7=0;ABAR6=1;B5=0;A4=0; 
                delay();
                state_pos_full = 0;
                break;
        }
    }
    BBAR7=0;ABAR6=0;B5=0;A4=0;
    INTCONbits.INT0IE = 0;
    i=0;
    j=0;
}

void FullStepsPos(unsigned int steps) {
    if(last_status == 0)
        state_pos_full = state;
    else if(last_status == 1)
        state_pos_full = state_pos_full;
    else if(last_status == 2) {
        switch(state_neg_full) {
            case 0:
                state_pos_full = 0;
                break;
            case 1:
                state_pos_full = 3 ;
                break;
            case 2:
                state_pos_full = 2;
                break;
            case 3:
                state_pos_full = 1;
                break;
            
        }
    }
    
    last_status = 1;
    //tx1_byte('p');
    
    do {
        switch(state_pos_full) {
            case 0:
                BBAR7=1;ABAR6=0;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state_pos_full = 1;
                break;
            case 1:
                BBAR7=0;ABAR6=0;B5=0;A4=1; 
                steps = steps-1;
                delay();
                state_pos_full = 2;
                break;
            case 2:
                BBAR7=0;ABAR6=0;B5=1;A4=0; 
                steps = steps-1;
                delay();
                state_pos_full = 3;
                break;
            case 3:
                BBAR7=0;ABAR6=1;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state_pos_full = 0;
                break;
        }
    }while(steps>0);
    BBAR7=0;ABAR6=0;B5=0;A4=0;
}

void FullStepsNeg(unsigned int steps) {
    if(last_status == 0)
        state_neg_full = 0;
    else if(last_status == 2)
        state_neg_full = state_neg_full;
    else if(last_status == 1) {
        switch(state_pos_full) {
            case 0:
                state_neg_full = 0;
                break;
            case 1:
                state_neg_full = 3 ;
                break;
            case 2:
                state_neg_full = 2;
                break;
            case 3:
                state_neg_full = 1;
                break;
            
        }
    }
    
    last_status = 2;
    //tx1_byte('n');
    
    do {
        switch(state_neg_full) {
            case 0:
                BBAR7=0;ABAR6=0;B5=1;A4=0; 
                steps = steps-1;
                delay();
                state_neg_full = 1;
                break;
            case 1:
                BBAR7=0;ABAR6=0;B5=0;A4=1; 
                steps = steps-1;
                delay();
                state_neg_full = 2;
                break;
            case 2:
                BBAR7=1;ABAR6=0;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state_neg_full = 3;
                break;
            case 3:
                BBAR7=0;ABAR6=1;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state_neg_full = 0;
                break;
        }
    }while(steps>0);
    BBAR7=0;ABAR6=0;B5=0;A4=0;
}

/*
void HalfStepsPos(int steps) {
    state = 0;
    do {
        switch(state) {
            case 0:
                BBAR7=1;ABAR6=1;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state = 1;
                break;
            case 1:
                BBAR7=1;ABAR6=0;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state = 2;
                break;
            case 2:
                BBAR7=1;ABAR6=0;B5=0;A4=1; 
                steps = steps-1;
                delay();
                state = 3;
                break;
            case 3:
                BBAR7=0;ABAR6=0;B5=0;A4=1; 
                steps = steps-1;
                delay();
                state = 4;
                break;
            case 4:
                BBAR7=0;ABAR6=0;B5=1;A4=1; 
                steps = steps-1;
                delay();
                state = 5;
                break;
            case 5:
                BBAR7=0;ABAR6=0;B5=1;A4=0; 
                steps = steps-1;
                delay();
                state = 6;
                break;
            case 6:
                BBAR7=0;ABAR6=1;B5=1;A4=0; 
                steps = steps-1;
                delay();
                state = 7;
                break;
            case 7:
                BBAR7=0;ABAR6=1;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state = 0;
                break;
        }
    }while(steps>0);
    BBAR7=0;ABAR6=0;B5=0;A4=0;
}

void HalfStepsNeg(int steps) {
    state = 0;
    do {
        switch(state) {
            case 0:
                BBAR7=0;ABAR6=1;B5=1;A4=0; 
                steps = steps-1;
                delay();
                state = 1;
                break;
            case 1:
                BBAR7=0;ABAR6=0;B5=1;A4=0; 
                steps = steps-1;
                delay();
                state = 2;
                break;
            case 2:
                BBAR7=0;ABAR6=0;B5=1;A4=1; 
                steps = steps-1;
                delay();
                state = 3;
                break;
            case 3:
                BBAR7=0;ABAR6=0;B5=0;A4=1; 
                steps = steps-1;
                delay();
                state = 4;
                break;
            case 4:
                BBAR7=1;ABAR6=0;B5=0;A4=1; 
                steps = steps-1;
                delay();
                state = 5;
                break;
            case 5:
                BBAR7=1;ABAR6=0;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state = 6;
                break;
            case 6:
                BBAR7=1;ABAR6=1;B5=0;A4=0; 
                steps = steps-1;
                delay();
                state = 7;
                break;
            case 7:
                BBAR7=0;ABAR6=1;B5=0;A4=0;
                steps = steps-1;
                delay();
                state = 0;
                break;
        }
    }while(steps>0);
    BBAR7=0;ABAR6=0;B5=0;A4=0;
}
*/
#endif	/* STEPPER_DRIVE_H */