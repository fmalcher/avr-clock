/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */


ISR(TIMER1_COMPA_vect){
	PORTB ^= (1 << PB0);

	if(alarmCnt < 3){
		alarmCnt++;
	}else{
		alarmCnt = 0;
		if(OCR1A < 800){
			OCR1A++;
		}else{
			OCR1A = 600;
		}
	}
	
	if(blinkCnt < 600){
		blinkCnt++;
	}else{
		blinkCnt = 0;
		if(lightStatus() == 1){
			setLight(0);
		}else{
			setLight(1);
		}
	}
}