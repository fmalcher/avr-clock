/* Author: Ferdinand Malcher
 * Project: Clock project, HfTL Computer Engineering 2012
 */

//timer for DCF signal length
ISR(TIMER0_OVF_vect){
	ovfCnt++;
}