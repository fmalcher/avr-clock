/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */

//interrupt for DCF receiver
ISR(INT0_vect){
	//enable timer0 after 5 ticks
	if(tmpCnt < 5) tmpCnt++;
	else{
		TIMSK |= (1 << TOIE0);
	}
	
	if(dcfRun == 0){
		TCNT0 = 0;
		if(ovfCnt > 80){
			if(dcfParsed == 1){
				
				//set new time and sync timer
				s = 0;
				m = mDcf;
				h = hDcf;
				TCNT2 = 0;
				
				//disable interrupt
				GICR &= ~(1 << INT0);
				
				//disable timer
				TIMSK &= ~(1 << TOIE0);
				
				//disable DCF Power
				PORTB &= ~(1 << PB1);
				
				//set time until next dcf sync
				dcfWait = 3600;
				
				dcfParsed = 0;
				
				return;
			}
			
			
			dcfRun = 1;
			dcfPos = -1;
		}
		ovfCnt = 0;
	}
	
	if(dcfRun == 1){
		if(dcfSig == 0){
			TCNT0 = 0;
			ovfCnt = 0;
			dcfSig = 1;
		}else{
			if(ovfCnt > 4 && ovfCnt < 8){
				dcfPos++;
				dcfArray[dcfPos] = 0;
			}else if(ovfCnt > 8 && ovfCnt < 15){
				dcfPos++;
				dcfArray[dcfPos] = 1;
				
			}
			setlcd = 1;
			dcfSig = 0;
		}
		
		if(dcfPos == 35){
			dcfRun = 0;
			dcfParse();
		}
		
	}
}