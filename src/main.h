/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */

#ifndef F_CPU
#define F_CPU 1000000
#endif


#define PORTH PORTA
#define DDRH DDRA

#define PORTMS PORTC
#define DDRMS DDRC



//time variables
volatile uint8_t h, m, s, hAlarm, mAlarm;

//system status
volatile uint8_t bounce = 0, setlcd = 1;
volatile uint8_t p1, p2;


//DCF variables
volatile uint8_t dcfSig = 0, dcfRun = 0, dcfParsed = 0; //flags
volatile uint16_t dcfWait = 0;
volatile uint8_t tmpCnt = 0, dcfPos = 0, ovfCnt = 0, dcfArray[36], hDcf, mDcf;

//general preferences
volatile uint8_t binDspl, binSec, dcfAct, alarmAct; //flags
volatile uint8_t snoozeMin;
volatile uint16_t snoozeCnt = 0;

//alarm
volatile uint8_t aLightStatus; //flags
volatile uint8_t alarmCnt;
volatile uint16_t blinkCnt;

//char buffer for display
char buf[3];

//eeprom variables
uint8_t ee_ma EEMEM; //mAlarm
uint8_t ee_ha EEMEM; //hAlarm
uint8_t ee_prefs EEMEM; //prefs
uint8_t ee_snmin EEMEM; //snoozemin


//menu system status
volatile uint8_t menuAct = 0, menuPos = 0, returnFromMenu = 0;
uint8_t menuLen = 10; //number of menu elements (w/o exit)

char menuTxt[11][16] = {
	"Display light",   //0
	"Set hour",        //1
	"Set minute",      //2
	"Enable alarm",    //3
	"Set alarm hour",  //4
	"Set alarm min",   //5
	"Snooze time (m)", //6
	"Enable DCF sync", //7
	"Binary display",  //8
	"Binary Min/Sec",  //9
	"Exit menu"
};