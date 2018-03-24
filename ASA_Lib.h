/*===============ASA Series Library===============*/

/*****ReleaseDate*****/
//2015/7/28

/*****Feature*****/

/*===Supported Module===*/
//ASA M128
//ASA 7S00
//ASA KB00

/*================================================*/

#ifndef __ASA_H__
#define __ASA_H__

#include <avr\io.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <util\delay.h>

#define M128_AllINT_enable sei()
#define M128_AllINT_disable cli()

/**M128**/
char ASA_M128_set(void);
char M128_DIO_fpt(char LSByte, char Mask, char shift, char Data);
char M128_DIO_fgt(char LSByte, char Mask, char shift, char *Data_p);
char M128_Timer0ISR_set(void);
char M128_Timer0ISR_reg(void (*function)(void));

/**7S00**/
char ASA_7S00_set(char ASA_ID, char LSByte, char Mask, char shift, char Data);
char ASA_7S00_put(char ASA_ID, char LSByte, char Bytes, void *Data_p);

/**KB00**/
char ASA_KB00_set(char ASA_ID, char LSByte, char Mask, char shift, char Data);
char ASA_KB00_get(char ASA_ID, char LSByte, char Bytes, void *Data_p);
 
#endif