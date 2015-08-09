/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */


//asynchronous timer for time base (1 sec)
ISR(TIMER2_OVF_vect){
		//increase seconds, minutes and hours
		if(s < 59){
			s++;
		}else{
			s = 0;
			if(m < 59){
				m++;
			}else{
				m = 0;
				if(h < 23){
					h++;
				}else{
					h = 0;
				}
			}
			if(menuAct == 1) setlcd = 1; //set LCD if menu active and minute changed
		}
		//show on binary display
		if(binDspl == 1){
			PORTH = h;
			if(binSec == 1) PORTMS = s;
			else PORTMS = m;
		}
		
		//decrease snooze counter
		if(snoozeCnt > 0){
			snoozeCnt--;
		}
		
		//decrease DCF wait counter
		if(dcfWait > 0){
			dcfWait--;
		}
		
		//only set LCD new if menu is not active
		if(menuAct == 0) setlcd = 1;
}