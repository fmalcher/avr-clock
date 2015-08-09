/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */

void writeOn();
void writeOff();
void setLight(uint8_t val);
uint8_t lightStatus();

void setAlarm(uint8_t val);
uint8_t alarmStatus();

void dcfParse();
void savetoeeprom();
void readfromeeprom();



void writeOn(){
	lcd_setcursor(0,1);
	lcd_string("> On ");
}

void writeOff(){
	lcd_setcursor(0,1);
	lcd_string("> Off");
}


void setLight(uint8_t val){
	if(val == 1){
		PORTB &= ~(1 << PB5);
	}else{
		PORTB |= (1 << PB5);
	}
}

uint8_t lightStatus(){
	if(PORTB & (1 << PB5)){
		return 0;
	}else{
		return 1;
	}
}

void setAlarm(uint8_t val){
	if(val == 1){
		//alarm on
		aLightStatus = lightStatus(); //save light status
		TIMSK |= (1 << OCIE1A);
		
	}else{
		//alarm off
		TIMSK &= ~(1 << OCIE1A);
	
		if(aLightStatus == 1){ //restore light status
			setLight(1);
		}else{
			setLight(0);
		}
	}
}


uint8_t alarmStatus(){
	if(TIMSK & (1 << OCIE1A)){
		return 1;
	}else{
		return 0;
	}
}

void dcfParse(){
	int numOne = 0;
	
	mDcf = 0;
	hDcf = 0;
	
	if(dcfArray[20] != 1) return;
	
	if(dcfArray[21] == 1){
		mDcf += 1;
		numOne++;
	}
	
	if(dcfArray[22] == 1){
		mDcf += 2;
		numOne++;
	}
	
	if(dcfArray[23] == 1){
		mDcf += 4;
		numOne++;
	}
	
	if(dcfArray[24] == 1){
		mDcf += 8;
		numOne++;
	}
	
	if(dcfArray[25] == 1){
		mDcf += 10;
		numOne++;
	}
	
	if(dcfArray[26] == 1){
		mDcf += 20;
		numOne++;
	}
	
	if(dcfArray[27] == 1){
		mDcf += 40;
		numOne++;
	}
	
	if(dcfArray[28] == 1){
		numOne++;
	}
	
	if(numOne % 2){
		return; //minute parity failed
	}
	
	numOne = 0;
	
	
	
	if(dcfArray[29] == 1){
		hDcf += 1;
		numOne++;
	}
	
	if(dcfArray[30] == 1){
		hDcf += 2;
		numOne++;
	}
	
	if(dcfArray[31] == 1){
		hDcf += 4;
		numOne++;
	}
	
	if(dcfArray[32] == 1){
		hDcf += 8;
		numOne++;
	}
	
	if(dcfArray[33] == 1){
		hDcf += 10;
		numOne++;
	}
	
	if(dcfArray[34] == 1){
		hDcf += 20;
		numOne++;
	}
	
	if(dcfArray[35] == 1){
		numOne++;
	}
	
	if(numOne % 2){
		return; //hour parity failed
	}
	
	numOne = 0;
	
	
	if(hDcf < 0 || hDcf > 23 || mDcf < 0 || mDcf > 59){
		return;
	}
	
	
	dcfParsed = 1;
	
	return;
}


void savetoeeprom(){
	uint8_t prefs = 0;
	
	eeprom_update_byte(&ee_ma, mAlarm);
	eeprom_update_byte(&ee_ha, hAlarm);
	
	if(binDspl == 1) prefs |= (1 << 0);
	if(binSec == 1) prefs |= (1 << 1);
	if(dcfAct == 1) prefs |= (1 << 2);
	if(alarmAct == 1) prefs |= (1 << 3);
	if(lightStatus() == 1) prefs |= (1 << 4);

	eeprom_update_byte(&ee_prefs, prefs);
	eeprom_update_byte(&ee_snmin, snoozeMin);
}

void readfromeeprom(){
	uint8_t prefs;
	
	if((mAlarm = eeprom_read_byte(&ee_ma)) > 59) mAlarm = 0;
	if((hAlarm = eeprom_read_byte(&ee_ha)) > 59) hAlarm = 0;
	
	
	if((prefs = eeprom_read_byte(&ee_prefs)) == 0xFF){
		prefs = 0b00010101; //default values
	}
	
	if(prefs & (1 << 0)) binDspl = 1;
	else binDspl = 0;

	if(prefs & (1 << 1)) binSec = 1;
	else binSec = 0;
	
	if(prefs & (1 << 2)) dcfAct = 1;
	else dcfAct = 0;
	
	if(prefs & (1 << 3)){
		alarmAct = 1;
		PORTB |= (1 << PB6); //enable alarm signal lamp
	}else{
		alarmAct = 0;
		PORTB &= ~(1 << PB6); //disable alarm signal lamp
	}
	
	if(prefs & (1 << 4)){
		setLight(1);
	}else setLight(0);
	
	if((snoozeMin = eeprom_read_byte(&ee_snmin)) > 15) snoozeMin = 5;
}