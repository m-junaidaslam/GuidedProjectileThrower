/* 
 * File:   ADXL335_ADC_45K22.h
 * Author: M.Junaid Aslam
 *
 * Created on April 10, 2016, 9:49 PM
 */

#ifndef ADXL335_ADC_45K22_H
#define	ADXL335_ADC_45K22_H

unsigned int ADC_low;
unsigned int ADC_high;
unsigned int ADC_result;
unsigned char digits[4];
volatile bit adxl_en;
 
void ADXL_Init();
void ADXL_Read();
void ADXL_Display();


void ADXL_Init() {
    TRISE = 1;
    ADCON2 = 0b10101111;    //Right justify, Frc
    ADCON1 = 0b00000101;  //Positive voltage referene at VDD and negative voltage reference at VSS
    ADCON0 = 0b00010101;    //E0 as selected channel, clear GO/Done, turn on adc
    adxl_en = 1;
}

void ADXL_Read() {
    ADCON0bits.GO_DONE = 1;
    while(ADCON0bits.GO_DONE);
    ADC_low = ADRESL;
    ADC_high = ADRESH & 0b00000011;
    ADC_result = (ADC_high << 8) + ADC_low;
}

void ADXL_Display() {
    second_line();
    lcd_delay(100);
    digits[0] = (ADC_result / 1000) + 48;
    digits[1] = ((ADC_result / 100) % 10) + 48;
    digits[2] = ((ADC_result / 10) % 10) + 48;
    digits[3] = (ADC_result % 10) + 48;
    dis_string(digits, 4);
}

#endif	/* ADXL335_ADC_45K22_H */

