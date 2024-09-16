#include <avr/io.h>
#define F_CPU 1000000ul
#include "MYMICRO.h"
#include "SSEG.h"
#include "OStime.h"
#include "UART_Drive.h"
#include "LED.h"
#include "common.h"
#include "lcd_lib.h"
#include "EEPROM.h"
#include <avr/interrupt.h>
#include <avr/delay.h>
void print(char x,char y);
void PrintStr(const char * sent);
void STOPTIMER (void);
 char pr;
 unsigned char  read;
 char n_raed;
 char n_LED;
int main(void)
{
	///////////// Local Declaration //////////////
	uint8 receivedByte;

	//////////// init seq ////////////
	/////207
	USART_Init(25);
	LED_init;
	init_SSEG();
	LCDinit();
	LCDclr();
	//n_raed= read;
	
	OStime_start();
    EEPROM_init();
	//EEROM_write(0,read);
	//_delay_ms(500);
    read=EEPROM_read(0);
    SSEG_Print99(read);
	LCDclr();
	LCDGotoXY(0,0);
	LCDstring("No Restart:",11);
	LCDGotoXY(14,0);
	LCDsendChar(read%10+48);
	LCDGotoXY(12,0);
	LCDsendChar((read/10)+48);
	_delay_ms(3000);
    read++;
   EEROM_write(0,read);
   _delay_ms(500);
   TCCR0 &=~ (1 << CS00);
   TCCR0 &=~ (1 << CS01);
   TCCR0 &=~ (1 << CS02);
   
   LCDclr();
   _delay_ms(500);
   LCDGotoXY(2,0);
   LCDstring("Hello World",11);
   _delay_ms(1000);
	//////////  app seq /////////////
	while(1)
	{
		//STOPTIMER();
        
		receivedByte = USART_Receive();
		switch(receivedByte)
		{
			case 0x61:
			LED1_ON;
			TIMSK|=(1<<TOIE1);  //ENABLE OVERFLOW INTERRUPT
			TCCR1B|=(1<<CS11);   //start timer at CPU/8
			 pr='a';
			 n_LED='1';
			 sei();    /// enable global interrupt
			 //goto xx;
			break;
			case 'b':
		//	LED_OFF;
		    LED2_ON;
			TIMSK|=(1<<TOIE1);  //ENABLE OVERFLOW INTERRUPT
			TCCR1B|=(1<<CS11);   //start timer at CPU/8
			 pr='b';
			 n_LED='2';
			 sei();    /// enable global interrupt
			 //goto xx;
			break;
			case 'c':
			LED3_ON;
			TIMSK|=(1<<TOIE1);  //ENABLE OVERFLOW INTERRUPT
			TCCR1B|=(1<<CS11);   //start timer at CPU/8
			pr='c';
			n_LED='3';
			sei();    /// enable global interrupt
			//goto xx;
			break;
			case 'd':
			LED4_ON;
			TIMSK|=(1<<TOIE1);  //ENABLE OVERFLOW INTERRUPT
			TCCR1B|=(1<<CS11);   //start timer at CPU/8
			pr='d';
			n_LED='4';
			sei();    /// enable global interrupt
			//goto xx;
			break;
			case 'f':
			OStime_start();
			SSEG_Print99(33);
			_delay_ms(1000);
			TCCR0 &=~ (1 << CS00);
			TCCR0 &=~ (1 << CS01);
			TCCR0 &=~ (1 << CS02);
			break;
			default:
				USART_Transmit('E');
			//PrintStr("E");
		}
	}
	return 0;
}
void PrintStr(const char * sent)
{
	// USART_Transmit(*(sent+2))
	while(*(sent) != 0)
	{
		USART_Transmit(*sent++);
	}

}
ISR(TIMER1_OVF_vect) ////interrupt service routine
{
	print(pr,n_LED);
	//STOPTIMER();
}
void print(char x,char y)
{
	//cli();
	LCDclr();
	LCDGotoXY(3,0);
	LCDstring("YOU SEND:",9);
	LCDGotoXY(13,0);
	LCDsendChar(x);
	LCDGotoXY(3,1);
	LCDstring("LED",3);
	LCDGotoXY(6,1);
	LCDsendChar(y);
	LCDGotoXY(8,1);
	LCDstring(":ON",3);
	_delay_ms(5000);
	STOPTIMER();
	 LCDclr();
//	 LCDGotoXY(2,1);
//	 LCDstring("Hello World",11);
	//sei();
	
}
void STOPTIMER (void)
{
	TCCR1B&=~(1<<CS01);
	TCCR1B&=~(1<<CS11);
	TCCR1B&=~(1<<CS12);
}