/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "main.h"
#include "lcd.h"

#include "functions.c"


#include "isrint0.c"
#include "isrint1.c"
#include "isrint2.c"
#include "isrtimer0.c"
#include "isrtimer1.c"
#include "isrtimer2.c"





int main(void){
	//disable JTAG (must be done twice)
	MCUCSR |= (1 << JTD);
	MCUCSR |= (1 << JTD);

	//Pin and port setting
	DDRH  = 0b11111111;
	PORTH = 0b00000000;
	
	DDRMS  = 0b11111111;
	PORTMS = 0b00000000;

	DDRB = 0b11111011;
	PORTB = 0b00111100;
	
	DDRD = 0b11110011;
	PORTD |= 0b00001100;
	
	lcd_init();
	lcd_clear();
	
	readfromeeprom();
	
	//INT0 (DCF) (is being enabled in while1-loop)
	MCUCR |= (1 << ISC00);
	MCUCR &= ~(1 << ISC01);
	
	
	//INT1 (Menu button)
	MCUCR |= (1 << ISC11);
	MCUCR &= ~(1 << ISC10);
	GICR |= (1 << INT1);
	
	//INT2 (Set button)
	MCUCR &= ~(1 << ISC2);
	GICR |= (1 << INT2);
	
	//Timer0
	TCCR0 |= (1 << CS01) | (1 << CS00);
	TCCR0 &= ~(1 << CS02);
	
	//Timer 1 (buzzer)
	TCCR1B &= ~(1 << WGM13); //WGM13 = 0
	TCCR1B |= (1 << WGM12);  //WGM12 = 1
	TCCR1A &= ~(1 << WGM11) & ~(1 << WGM10); // WGM11 = 0; WGM10 = 0
    
	TCCR1B |= (1 << CS10); //prescaler
    OCR1A = 600; //overflow compare
	
	
	
	
	//Timer2 (1 sec)
	TIMSK |= (1 << TOIE2);
	ASSR |= (1 << AS2);
	TCCR2 |= (1 << CS22) | (1 << CS20);
	
	
	
	
	
	sei();
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	
	
	//show time (time is shown when returning from menu)
	returnFromMenu = 1;

	while(1){
	
		//debounce
		_delay_ms(20);
    	if(p1){
        	p1--;
   		}
    	if(p2){
        	p2--;
    	}
		
	
		//enable DCF interrupts if there is no more waiting time
		if(dcfWait == 0 && dcfAct == 1){
			PORTB |= (1 << PB1); //DCF power enable
			GICR |= (1 << INT0);
		}
		
		/* switch on alarm when:
		 * alarm is enabled AND not already running AND
		 * - alarm time is reached AND no snooze counter running OR
		 * - snooze counter is finished
		 */
		if(alarmAct == 1 && alarmStatus() == 0 && (((hAlarm == h && mAlarm == m) && snoozeCnt == 0) || (snoozeCnt == 1))){
			setAlarm(1);
		}
		//switch off alarm if disabled but still running
		if(alarmStatus() == 1 && alarmAct == 0){
			setAlarm(0);
		}


		//write stuff to LCD		
		if(setlcd == 1){
			if(menuAct == 1){ //show menu
				lcd_clear();
				lcd_string(menuTxt[menuPos]);
				
				if(menuPos == 1 || menuPos == 2 || menuPos == 4 || menuPos == 5 || menuPos == 6){
					lcd_setcursor(0,1);
					lcd_string("> ");
				}
				
				
				//DISPLAY LIGHT
				if(menuPos == 0){
					if(lightStatus() == 1){
						writeOn();
					}else{
						writeOff();
					}
				
				//HOUR
				}else if(menuPos == 1){
					itoa(h, buf, 10);
					if(h < 10) lcd_string("0");
					lcd_string(buf);

				//MIN
				}else if(menuPos == 2){
					itoa(m, buf, 10);
					if(m < 10) lcd_string("0");
					lcd_string(buf);
					
				//ENABLE ALARM
				}else if(menuPos == 3){
					if(alarmAct == 1){
						writeOn();
					}else{
						writeOff();
					}					
				
				//ALARM HOUR
				}else if(menuPos == 4){
					itoa(hAlarm, buf, 10);
					if(hAlarm < 10) lcd_string("0");
					lcd_string(buf);

				//ALARM MIN
				}else if(menuPos == 5){
					itoa(mAlarm, buf, 10);
					if(mAlarm < 10) lcd_string("0");
					lcd_string(buf);
				
				//SNOOZE TIME
				}else if(menuPos == 6){
					itoa(snoozeMin, buf, 10);
					lcd_string(buf);
					if(snoozeMin < 10) lcd_string(" ");
				
				//DCF SYNC
				}else if(menuPos == 7){
					if(dcfAct == 1){
						writeOn();
					}else{
						writeOff();
					}
					
				//BINARY DISPLAY
				}else if(menuPos == 8){
					if(binDspl == 1){
						writeOn();
					}else{
						writeOff();
					}
					
				//BINARY MIN/SEC
				}else if(menuPos == 9){
					lcd_setcursor(0,1);
					if(binSec == 1){
						lcd_string("> Sec");
					}else{
						lcd_string("> Min");
					}
					
				}
				
			}else{ //show normal time
				if(returnFromMenu == 1){
					returnFromMenu = 0;
					
					savetoeeprom();					
					
					lcd_clear();
					lcd_setcursor(6,0);
					lcd_string("TIME");
				}
				
				
				//write time to display
				lcd_setcursor(4,1);
				
				if(h < 10) lcd_string("0");
				itoa(h, buf, 10);
				lcd_string(buf);
			
				lcd_string(":");
	
				if(m < 10) lcd_string("0");
        		itoa(m, buf, 10);
	        	lcd_string(buf);			
	
				lcd_string(":");

				if(s < 10) lcd_string("0");
	        	itoa(s, buf, 10);
    	    	lcd_string(buf);
    	    }
    	    
    	    setlcd = 0;
    	}
	sleep_mode();
	}

	return 0;
}


