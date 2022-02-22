/* 
 * File:   uart_45k22.h
 * Author: M.Junaid Aslam
 *
 * Created on December 10, 2015, 4:43 AM
 */

#ifndef UART_45K22_H
#define	UART_45K22_H

#define tx1PIN TRISCbits.RC6 
#define rx1PIN TRISCbits.RC7
#define tx2PIN TRISDbits.RD6
#define rx2PIN TRISDbits.RD7

void Open_UART1_Resetting();
void Open_UART1();
void Open_UART2();
void Close_UART1();
void Close_UART2();
void tx1_byte(unsigned char data1);
void tx2_byte(unsigned char data2);
void tx1_byte_n(unsigned char data1n);
unsigned char rx1_byte(void);
unsigned char rx2_byte(void);
void tx1_string(char wrptr[],char len);
void tx2_string(char wrptr[],char len);

void Open_UART1_Resetting() {
    tx1PIN=0;
    rx1PIN=1;
    SPBRG1=25;                                // Fill the SPBRG register to set the Baud Rate (9600 Baud Rate using 16MHz Oscillator)
    TXSTA1bits.TXEN=1;                                     // To enable transmission
    RCSTA1bits.SPEN=1;                                     // To activate Serial port (TX and RX pins)
    RCSTA1bits.CREN=1;                                     // To enable continuous reception
}

void Open_UART1() {
    tx1PIN=0;
    rx1PIN=1;
    SPBRG1=25;                                // Fill the SPBRG register to set the Baud Rate (9600 Baud Rate using 16MHz Oscillator)
    TXSTA1bits.TXEN=1;                                     // To enable transmission
    RCSTA1bits.SPEN=1;                                     // To activate Serial port (TX and RX pins)
    RCSTA1bits.CREN=1;                                     // To enable continuous reception
    RCONbits.IPEN = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RC1IE = 1;
}

void Close_UART1() { 
    TXSTA1bits.TXEN=0;                                     // To disable transmission
    RCSTA1bits.SPEN=0;                                     // To deactivate Serial port (TX and RX pins)
    RCSTA1bits.CREN=0;                                     // To disable continuous reception
    PIE1bits.RC1IE = 0;
}


void Open_UART2() {
    tx2PIN=0;
    rx2PIN=1;
    SPBRG2=25;                                // Fill the SPBRG register to set the Baud Rate (9600 Baud Rate using 16MHz Oscillator)
    RCSTA2bits.SPEN=1;                                     // To activate Serial port (TX and RX pins)
    TXSTA2bits.TXEN=1;                                     // To enable transmission
    //RCSTA2bits.CREN=1;                                     // To enable continuous reception
    RCONbits.IPEN = 0;
    PIE3bits.RC2IE = 1;
    IPR3bits.RC2IP = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    RCONbits.IPEN = 1;
    
}

void Close_UART2() { 
    TXSTA2bits.TXEN=0;                                     // To disable transmission
    RCSTA2bits.SPEN=0;                                     // To deactivate Serial port (TX and RX pins)
    //TXSTA2bits.SYNC=0;
    RCSTA2bits.CREN=0;                                     // To disable continuous reception
    PIE3bits.RC2IE = 0;
 }

void tx1_byte(unsigned char data1)
{
    TXREG1=data1;                                     // Store data in Transmit register
    while(PIR1bits.TX1IF==0);                             // Wait until TXIF gets low
}

void tx1_byte_n(unsigned char data1n) {
    tx1_byte(data1n);
    tx1_byte('\r');
    tx1_byte('\n');
}

void tx2_byte(unsigned char data2)
{
    TXREG2=data2;                                     // Store data in Transmit register
    while(PIR3bits.TX2IF==0);                             // Wait until TXIF gets low
}


unsigned char rx1_byte(void)
{
    while(PIR1bits.RC1IF==0);                        // Wait until RCIF gets low
    return RCREG1;                                   // Retrieve data from reception register
}

unsigned char rx2_byte(void)
{
    while(PIR3bits.RC2IF==0);                            // Wait until RCIF gets low
    return RCREG2;                                   // Retrieve data from reception register
}


void tx1_string(char wrptr[],char len){   
    int count=0;
    int i;
    for (i=0;i<len;i++) {                           // test for string null character
        tx1_byte(wrptr[count]);
        count++;
    }
}

void tx2_string(char wrptr[],char len){   
    int count=0;
    int i;
    for (i=0;i<len;i++) {                           // test for string null character
        tx2_byte(wrptr[count]);
        count++;
    }
}

#endif	/* UART_45K22_H */
