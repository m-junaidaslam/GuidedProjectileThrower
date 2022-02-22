/*
xtal = 20M
//#pragma config FOSC = INTOSC_EC
#pragma config FOSC = HS
#pragma config PLLDIV=5
#pragma config CPUDIV=OSC4_PLL6,OSC3_PLL4
*/
//This library will work only with these configurations


#ifndef _LCD_H_
#define	_LCD_H_
//Prototypes
void lcd_ini();
void lcd_cmd(unsigned char);
void dis_char(unsigned char);
void LCD_delay(int times);
void lcd_right_shift(unsigned int n);
void dis_string( char wrptr[],char len);
void dis_string_cod(unsigned char line,unsigned char col,char wrptr[]);
void dis_char_cod(unsigned char line,unsigned char col,char wrptr);
void clr_scr();
void first_line();
void second_line();
void lcd_waiting_display();
void back_space();
void lcdcmd_nibble(unsigned char);
void lcddata_nibble(unsigned char);

//LCD Control pins
#define rs LATBbits.LB2
//#define rw LATCbits.LC1	//Always Write 
#define en LATBbits.LB3
#define en_D    TRISBbits.RB3=0;			// Configure Port B as output port
#define rs_D	TRISBbits.RB2=0;			// Configure Port B as output port
#define D4_D	TRISBbits.RB4=0;			// Configure Port B as output port
#define D5_D	TRISBbits.RB5=0;			// Configure Port B as output port
#define D6_D	TRISBbits.RB6=0;			// Configure Port B as output port
#define D7_D	TRISBbits.RB7=0;			// Configure Port B as output port
#define D4 LATBbits.LATB4
#define D5 LATBbits.LATB5
#define D6 LATBbits.LATB6
#define D7 LATBbits.LATB7
int init_count=0;
void back_space(){
    lcd_cmd(0x10);
    dis_char(' ');
    lcd_cmd(0x10);
}
void first_line(){
    lcd_cmd(0x80);
}
void second_line(){
    lcd_cmd(0xc0);
}

void clr_scr(){
    lcd_cmd ( 0x0C );   //display on , cursor off
lcd_cmd ( 0x06 );   //increment cursor 
lcd_cmd ( 0x80 );   //force cursor to the begining of 1st line
lcd_cmd ( 0x01 );   //clear display

}

void dis_string(char wrptr[],char len){   
    unsigned int count = 0;
    int i;
    for(i = 0; i< len; i++) {
        dis_char(wrptr[count]);
        count++;
    }
}

void dis_string_cod(unsigned char line,unsigned char col,char wrptr[]){
    if(init_count==0){
        lcd_ini();
    }  
 col-=1;
 
    unsigned char x=0;
    if(line==1)
        x=0x80;
    if(line==2)
        x=0xc0;
    x+=col;
    lcd_cmd(x);
    dis_string(wrptr,sizeof wrptr);

}
void dis_char_cod(unsigned char line,unsigned char col,char wrptr){
    if(init_count==0){
        lcd_ini();
    }  
 col-=1;
    unsigned char x=0;
    if(line==1)
        x=0x80;
    if(line==2)
        x=0xc0;
    x+=col;
    lcd_cmd(x);
    dis_char(wrptr);

}

void lcd_delay(long temp){
    long i,j;
    for(j=0;j<temp;j++)
    for( i=0;i<150;i++)
        ;
}
void lcd_ini()	                    
{init_count=1;
int i;
    lcd_delay(10);
en_D;
rs_D;
D4_D;D5_D;D6_D;D7_D;
    en=1;
    rs=1;
    D4=1;D5=1;D6=1;D7=1;    

	lcd_cmd(0x02);		// To initialize LCD in 4-bit mode.
	lcd_cmd(0x28);		// To initialize LCD in 2 lines, 5x7 dots and 4bit mode.
	lcd_cmd(0x0C);
	lcd_cmd(0x06);
	lcd_cmd(0x80);
}
void lcd_waiting_display(){
    int i,j;
    lcd_delay(40);
    for( i=0;i<16;i++){
    lcd_cmd(0x18);
    lcd_delay(20);
    }
    while(1){
    
    for( i=0;i<31;i++)	
    lcd_cmd(0x1C);
       
    for( i=0;i<31;i++){
    lcd_cmd(0x18);
    
    lcd_delay(20);
    }
     
    }}
void lcd_cmd(unsigned char cmd_value)
{
	unsigned char cmd_value1;
	cmd_value1 = (cmd_value & 0xF0);	// Mask lower nibble because RB4-RB7 pins are being used
	lcdcmd_nibble(cmd_value1);			// Send to LCD
	cmd_value1 = ((cmd_value<<4) & 0xF0);	// Shift 4-bit and mask
	lcdcmd_nibble(cmd_value1);			// Send to LCD
}

void lcd_right_shift(unsigned int n) {
    unsigned int p;
    for(p=0;p<n;p++)
        lcd_cmd(0b00010100);
}

void dis_char(unsigned char data_value) {
	unsigned char data_value1;
	data_value1=(data_value&0xF0);
	lcddata_nibble(data_value1);
	data_value1=((data_value<<4)&0xF0);
	lcddata_nibble(data_value1);
}


void lcdcmd_nibble(unsigned char cmdout)
{
//	lcdport=cmdout;		//Send command to lcdport=PORTB
            D4=((cmdout&0x10)? 1:0);
            D5=((cmdout&0x20)? 1:0);
            D6=((cmdout&0x40)? 1:0);
            D7=((cmdout&0x80)? 1:0);
	rs=0;						
	//rw=0;
	en=1;rs=0;rs=0;rs=0;
	en=0;
    lcd_delay(20);
}

void lcddata_nibble(unsigned char dataout)
{
//	lcdport=dataout;	//Send data to lcdport=PORTB
            D4=((dataout&0x10)? 1:0);
            D5=((dataout&0x20)? 1:0);
            D6=((dataout&0x40)? 1:0);
            D7=((dataout&0x80)? 1:0);
	
	rs=1;
	//rw=0;
	en=1;
	rs=1;rs=1;rs=1;
	en=0;
    
    lcd_delay(10);
}
#endif


/*
 Example
void main(void)
{
	unsigned char data0[]="Ahsan Mehmood";
	unsigned char data1[]="LCD Lib test";
    unsigned char data2[]="check";
    
	unsigned int i=0;
	lcd_ini();			// LCD initialization
    
    //clr_scr();
    //first_line();
    //dis_string(data0);
    //second_line();
    //dis_string(data1);
    
    dis_string_cod(data0,1,1);
    dis_string_cod(data1,2,1);
    dis_string_cod(data2,1,4);
    
    while(1);
}
*/