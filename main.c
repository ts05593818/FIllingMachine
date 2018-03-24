#include "ASA_Lib.h"
///////////
//io set
#define Hundreds 1<<PA1     //百位數
#define Tens     1<<PA2     //十位數
#define Ones     1<<PA3     //個位數
#define valve_1  1<<PA4     // i/o for controling valve1
#define valve_2  1<<PA5
#define valve_3  1<<PA6
#define valve_4  1<<PA7
#define check_bottom PC1   //mode check and working
//adc channel
int ch[4]={0,1,2,3}; //ADC 0 1  2 3
uint8_t channel=0;
//模式設定
#define setting_mode  1 //setting mass
#define washing_mode  2 //washing
#define filling_mode  3 //filling
int mode_count=0;     //count mode
//adc value
float Vcc=4.7;      //input
volatile int count=0;
volatile int valve_count=0;
volatile int v1[2]={0};
volatile int v2[2]={0};
volatile int v3[2]={0};
volatile int v4[2]={0};
volatile int up=0;               // mode bottom
volatile int total_mass=0;             //mass
volatile int H=0,T=0,O=0;
float valve_avg1=0;
float valve_avg2=0;
float valve_avg3=0;
float valve_avg4=0;


void io_ini (void) {
    DDRA=0xFE; 0b11111110

    printf("io ini done\n");
}

void adc_ini (void) {
    ADMUX=(1<<REFS0);
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    printf("adc ini done\n");
}


void pwm_ini (void) {

    TCCR1A|=(1<<COM1A1)|(1<<COM1A0)|(1<<WGM11)|(1<<WGM10);
    TCCR1B|=(1<<WGM13)|(1<<WGM12)|(0<<CS12)|0<<(CS11)|(1<<CS10); //fast pwm mode N=1 OCR=255 freq=43200hz
    OCR1A=255;
    TIMSK|=(1<<OCIE1A)|(0<<OCIE1B);
    printf("pwm ini done\n");
}

void adc_read(void) {

      //channel=channel&ch[valve_count];
      channel=ch[valve_count];
      ADMUX  =channel|(1<<REFS0);
      ADCSRA |= (1<<ADSC);
      while(!(ADCSRA & (1<<ADIF))) ;
      switch (valve_count) {
          case 0 :
                  v1[count]=ADC;
                  count++;
                  if (count==2) {
                      count=0;
                      valve_avg1=(v1[0]+v1[1])/2;
                      valve_count++;
                  }
                  break;
          case 1 :
                 v2[count]=ADC;
                 count++;
                 if (count==2) {
                     count=0;
                     valve_avg2=(v2[0]+v2[1])/2;
                     valve_count++;
                 }
                 break;
          case 2 :
                  v3[count]=ADC;
                  count++;
                  if (count==2) {
                      count=0;
                      valve_avg3=(v3[0]+v3[1])/2;
                      valve_count++;
                  }
                  break;
          case 3 :
                  v4[count]=ADC;
                  count++;
                  if (count==2) {
                      count=0;
                      valve_avg4=(v4[0]+v4[1])/2;
                      valve_count=0;
                  }
                  break;
           default : break;
      }
      //printf("0:%.2f 1:%.2f 2:%.2f 3:%.2f\n",valve_avg1,valve_avg2,valve_avg3,valve_avg4);
}
void setting (void) {

  if(PINA&0x02 == Hundreds) {
    H=(H+1)%10;
  }
  else if(PINA&0x04 == Tens) {
    T=(T+1)%10;
  }
  else if(PINA&0x08 == Ones) {
    O=(O+1)%10;
  }
  else {
    total_mass=(H*100+T*10+O*1)%1000;
  }

}


ISR(TIMER1_COMPA_vect) {

  if((PINA)&0x01 == 1) { //PA0 as mode bottom
    up=1;
      if((PINA&0x01)==0 && up==1 ) {
        mode_count=(mode_count+1)%3+1;
        up=0;
      }
  }


         switch (mode_count) {
             case setting_mode :
                 setting();
                 printf("1\n");
                 break;
             case filling_mode :
                 filling();
                 printf("2\n");
                 break;
             case washing_mode :
                 washing();
                 printf("3\n");
                 break;
             default : break;
         }


}

int main (void) {

        ASA_M128_set();
        pwm_ini();
        adc_ini();
        io_ini();
        sei();

        while(1) {

            printf("pina=%d\n",PINA);

        }


}
