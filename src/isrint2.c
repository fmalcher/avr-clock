/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */
 
//interrupt for set buton
ISR(INT2_vect){
	cli();
		
		//snooze action
		if(alarmStatus() == 1 && menuAct == 0){
			setAlarm(0);
			snoozeCnt = snoozeMin * 60; //set snooze counter
			
			return;
		}
		
		
		//menu actions
		if(menuAct == 1){
			
			//LIGHT
			if(menuPos == 0){
				if(lightStatus() == 1){
					setLight(0);
				}else{
					setLight(1);
				}
				
			
			//SET HOUR
			}else if(menuPos == 1){
				if(h < 23){
					h++;
				}else{
					h = 0;
				}
				if(binDspl == 1){
					PORTH = h;
				}

			//SET MIN
			}else if(menuPos == 2){
				if(m < 58){
					m++;
				}else{
					m = 0;
				}
				s = 0;
				if(binDspl == 1){
					if(binSec == 1) PORTMS = s;
					else PORTMS = m;
				}
			
			
			//ENABLE ALARM
			}else if(menuPos == 3){
				if(alarmAct == 0){
					alarmAct = 1;
					PORTB |= (1 << PB6); //enable alarm signal lamp
				}else{
					if(alarmStatus() == 1){
						setAlarm(0); //switch off alarm
					}
					alarmAct = 0;
					snoozeCnt = 0;
					PORTB &= ~(1 << PB6); //disable alarm signal lamp
				}
			
			//SET ALARM HOUR
			}else if(menuPos == 4){
				if(hAlarm < 23){
					hAlarm++;
				}else{
					hAlarm = 0;
				}

			//SET ALARM MIN
			}else if(menuPos == 5){
				if(mAlarm < 58){
					mAlarm++;
				}else{
					mAlarm = 0;
				}
			
			//SNOOZE TIME (minutes)
			}else if(menuPos == 6){
				if(snoozeMin < 15){
					snoozeMin++;
				}else{
					snoozeMin = 1;
				}

			//ENABLE DCF SYNC
			}else if(menuPos == 7){
				if(dcfAct == 0){
					dcfAct = 1;
					dcfWait = 0;
				}else{
					dcfAct = 0;
					PORTB &= ~(1 << PB1);
				}
		
			//BINARY DISPLAY
			}else if(menuPos == 8){
				if(binDspl == 1){
					binDspl = 0;
					PORTH = 0;
					PORTMS = 0;
				}else{
					binDspl = 1;
					PORTH = h;
					if(binSec == 1) PORTMS = s;
					else PORTMS = m;
				}
			
			//BINARY MIN/SEC
			}else if(menuPos == 9){
				if(binSec == 1){
					binSec = 0;
					if(binDspl == 1) PORTMS = m;
				}else{
					binSec = 1;
					if(binDspl == 1) PORTMS = s;
				}

	
			//EXIT
			}else if(menuPos == menuLen){
				menuAct = 0;
				PORTB &= ~(1 << PB7); //disable menu signal lamp
			
				returnFromMenu = 1;
			}
		}
		setlcd = 1;
		
		_delay_ms(200);
	sei();	
}
