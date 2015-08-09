/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */

//interrupt for menu button
ISR(INT1_vect){
	cli();
	
		//start menu		
		if(menuAct == 0){
			menuAct = 1;
			PORTB |= (1 << PB7); //enable menu signal lamp
			
			menuPos = 0;
		}else{
			//next menu point
			if(menuPos < menuLen){
				menuPos++;
			}else{
				menuPos = 0;
			}		
		}
				
		setlcd = 1;

	_delay_ms(200);
	sei();	
		
}