/* ########################################################################

   PICsim - PIC simulator http://sourceforge.net/projects/picsim/

   ########################################################################

   Copyright (c) : 2011  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

/* ----------------------------------------------------------------------- */
/* Template source file generated by piklab */
#include <p18f452.h>


//config words
#pragma romdata CONFIG
rom const unsigned char CONFIG1L = 0x00;	//Dummy byte
rom const unsigned char CONFIG1H = 0x22;	//HS + 4x PLL mode, OSC switch enabled
rom const unsigned char CONFIG2L = 0x0D;	//BOR disabled, PWRTE enabled
rom const unsigned char CONFIG2H = 0x0E;	//WDT disabled
rom const unsigned char CONFIG3L = 0xFF;	//Dummy byte
rom const unsigned char CONFIG3H = 0xFF;	//CCP is on RC1
rom const unsigned char CONFIG4L = 0xFB;	//Debug off, LVP off, stack reset
rom const unsigned char CONFIG4H = 0xFF;	//Dummy byte
rom const unsigned char CONFIG5L = 0xFF;	//Code protection off
rom const unsigned char CONFIG5H = 0xFF;	//EE unprotect, Boot unprotect
rom const unsigned char CONFIG6L = 0xFF;	//Write protection off
rom const unsigned char CONFIG6H = 0xFF;	//EE unprotect, Boot unprotect, Config unprotect
rom const unsigned char CONFIG7L = 0xFF;	//Reads unprotected
rom const unsigned char CONFIG7H = 0xFF;	//Boot read unprotected
#pragma romdata


#include <delays.h>
#include "lcd.h"
#include "display7s.h"
#include <i2c.h>
#include <usart.h>
#include "eeprom.h"
#include "eeprom_ext.h"
#include <adc.h>
#include <stdio.h>
#include "teclado.h"
#include "rtc.h"


unsigned char cnt;
unsigned int t1cont;

void isrh(void);

void isrl(void);

char buffer[40];

char * rom2ram(const rom char *pdata)
{
  char n = 0;                    
  while(buffer[n] = pdata[n++]);
  return &buffer[1];
}


void main() 
{
  unsigned char i;
  unsigned char tmp;
  unsigned int tmpi;
  unsigned int tout;

  char str[6];

  ADCON1=0x06;
  TRISA=0xC3;
  TRISB=0x01;
  TRISC=0x01;
  TRISD=0x00;
  TRISE=0x00;


  INTCON2bits.RBPU=0;


  SSPADD = 0x18;  

  lcd_init();
  OpenI2C(MASTER, SLEW_OFF);
  OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT &USART_CONT_RX & USART_BRGH_HIGH,50);
  OpenADC(ADC_FOSC_8 & ADC_RIGHT_JUST & ADC_5ANA_0REF,ADC_CH0 & ADC_INT_OFF );


//dip
  TRISB=0x03;
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Ligue todos DIP"));
  lcd_cmd(L_L2);
  lcd_str(rom2ram("Press. RB1"));
  while(PORTBbits.RB1);


//testa caracter especial
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("   Teste LCD"));

  lcd_cmd(0x40);//endere�o

  lcd_dat(0x11);
  lcd_dat(0x19);
  lcd_dat(0x15);
  lcd_dat(0x13);
  lcd_dat(0x13);
  lcd_dat(0x15);
  lcd_dat(0x19);
  lcd_dat(0x11);

  lcd_dat(0x0E);
  lcd_dat(0x11);
  lcd_dat(0x0E);
  lcd_dat(0x05);
  lcd_dat(0x0E);
  lcd_dat(0x14);
  lcd_dat(0x0A);
  lcd_dat(0x11);


  lcd_cmd(L_L2);

  for(i=0;i<16;i++)
  {
    lcd_dat(i%2);
    Delay1KTCYx(200);
  }

//teste lcd
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("   Teste LCD"));
  
  for(i=32;i<128;i++)
  {
    if((i%16) == 0)lcd_cmd(L_L2);
    lcd_dat(i);
    Delay1KTCYx(100);
  }

  Delay1KTCYx(200);
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("   Teste LCD"));
  lcd_cmd(L_L2);
  lcd_str(rom2ram("       Ok"));
  Delay10KTCYx(100);




//testa display 7s

  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("   Teste 7 Seg"));


   ADCON1=0x06;
    for(i=0;i<4;i++)
    {
      switch(i)
      {
         case 0: 
           PORTA=0x20;
           break;
         case 1: 
           PORTA=0x10;
           break;
         case 2: 
           PORTA=0x08;
           break;
         case 3: 
           PORTA=0x04;
           break;
       }

      for(tmp=0;tmp<16;tmp++)
      {
        PORTD=display7s(tmp);	 
        Delay10KTCYx(40);	
      }
    }

  ADCON1=0x02;
  PORTD=0;


//dip
  TRISB=0x03;
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Desligue RTC DIP"));
  lcd_cmd(L_L2);
  lcd_str(rom2ram("Press. RB1"));
  while(PORTBbits.RB1);

//testa LEDs
  TRISB=0x00;
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1+3);
  lcd_str(rom2ram("Teste LEDs"));

  for(tmp=0;tmp<3;tmp++)
    {
      for(i=1;i > 0;i=i*2)
      { 
        PORTB=i;
        PORTD=i; 
        Delay10KTCYx(40);  
      }
    }
  PORTB=0;
  PORTD=0;
  for(i=0;i<4;i++)
  {
    PORTB^=0xFF;
    PORTD^=0xFF;
    Delay10KTCYx(40);
  }  

//testa chaves


  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("  Teste Chaves"));

  tmp=0;
  ADCON1=0x06;
  TRISA|=0x20;

  while((tmp & 0x7F) != 0x7F)
  {
    TRISB=0x3F;

    if(PORTBbits.RB0 == 0)
    {
      tmp|=0x01;
    }
    if(PORTBbits.RB1 == 0)
    {
      tmp|=0x02;
    }
    if(PORTBbits.RB2 == 0)
    {
      tmp|=0x04;
    }
    if(PORTBbits.RB3 == 0)
    {
      tmp|=0x08;
    }
    if(PORTBbits.RB4 == 0)
    {
      tmp|=0x10;
    }
    if(PORTBbits.RB5 == 0)
    {
      tmp|=0x20;
    }
    if(PORTAbits.RA5 == 0)
    {
      tmp|=0x40;
    }

    
    TRISB=0x00;
    PORTB=tmp;
    Delay1KTCYx(20);
  }

    Delay10KTCYx(100);

  PORTB=0; 
 

  TRISCbits.TRISC7=1; //RX
  TRISCbits.TRISC6=0; //TX

//teste serial
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Teste Serial TX"));
  lcd_cmd(L_L2+4);
  lcd_str(rom2ram("9600 8N1"));

   putsUSART(rom2ram("\r\n Picsimlab\r\n Teste Serial TX\r\n"));


  for(i=0;i<4;i++)
  {
    putcUSART(i+0x30);
    putsUSART(rom2ram(" PicsimLab\r\n"));
  }
  Delay10KTCYx(200);

  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Teste Serial RX"));
  putsUSART(rom2ram(" Digite!\r\n"));
  for(i=0;i<32;i++)
  {
    if(!(i%16))
    {
       lcd_cmd(L_L2);
       putsUSART(rom2ram("\r\n"));
    }
  
    tout=0;
    while((!DataRdyUSART())&&(tout < 2000))
    {
      tout++;
      Delay1KTCYx(2);
    }
    
    if(DataRdyUSART())
    { 
      tmp=getcUSART();
    }
    else
    {
      tmp='-';
    } 
    lcd_dat(tmp);
    putcUSART(tmp);
  }
  Delay1KTCYx(200);


 TRISA=0x07;
 ADCON1=0x02;


//teste ADC
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram(" Teste ADC (P1)"));

  for(i=0; i< 200; i++)
  {

    SetChanADC(ADC_CH0); 
    ConvertADC();
    while(BusyADC());
    tmp=((((unsigned int)ReadADC())/64)*10)/204;
    lcd_cmd(L_L2+6);
    sprintf(str,"%05i",tmp);
    lcd_dat(str[3]);
    lcd_dat(',');
    lcd_dat(str[4]);
    lcd_dat('V');
    Delay1KTCYx(20);
  }

  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram(" Teste ADC (P2)"));

  for(i=0; i< 200; i++)
  {

    SetChanADC(ADC_CH1); 
    ConvertADC();
    while(BusyADC());
    tmp=((((unsigned int)ReadADC())/64)*10)/204;
    lcd_cmd(L_L2+6);
    sprintf(str,"%05i",tmp);
    lcd_dat(str[3]);
    lcd_dat(',');
    lcd_dat(str[4]);
    lcd_dat('V');
    Delay1KTCYx(20);
  }


//teste RELE

  TRISCbits.TRISC0=0;
  TRISEbits.TRISE0=0;

  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Teste RELE 1"));

  
  for(i=0;i<5;i++)
  {
    PORTCbits.RC0^=1;
    Delay10KTCYx(100); 
  }
  PORTCbits.RC0=0; 

  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Teste RELE 2"));

  for(i=0;i<5;i++)
  {
    PORTEbits.RE0^=1;
    Delay10KTCYx(100); 
  }
  PORTEbits.RE0=0;
//dip
  TRISB=0x03;
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Desl. REL1 DIP"));
  lcd_cmd(L_L2);
  lcd_str(rom2ram("Press. RB1"));
  while(PORTBbits.RB1);


//teste TEMP
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("   Teste TEMP"));

  for(i=0; i< 100; i++)
  {
    SetChanADC(ADC_CH2); 
    ConvertADC();
    while(BusyADC());   
    tmpi=( (((unsigned int)ReadADC())/64)*10)/2;
    lcd_cmd(L_L2+5);
    sprintf(str,"%05i",tmpi);
    lcd_dat(str[2]);
    lcd_dat(str[3]);
    lcd_dat(',');
    lcd_dat(str[4]);
    lcd_dat('C');
    Delay1KTCYx(40);
  }


//teste Aquecimento
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("   Teste AQUEC"));
  PORTCbits.RC5=1;
  for(i=0; i< 100; i++)
  {
    SetChanADC(ADC_CH2); 
    ConvertADC();
    while(BusyADC());
    tmpi=( (((unsigned int)ReadADC())/64)*10)/2;
    lcd_cmd(L_L2+5);
    sprintf(str,"%05i",tmpi);
    lcd_dat(str[2]);
    lcd_dat(str[3]);
    lcd_dat(',');
    lcd_dat(str[4]);
    lcd_dat('C');
    Delay1KTCYx(100);
  }
  PORTCbits.RC5=0;


//teste Resfriamento

  TRISCbits.TRISC0=1;
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("   Teste RESFR"));

//timer0 temporizador

 T0CONbits.T0CS=0;
 T0CONbits.PSA=0;
 T0CONbits.T08BIT=1;
 T0CONbits.T0PS0=0; // divide por 32
 T0CONbits.T0PS1=0;
 T0CONbits.T0PS2=1;
 T0CONbits.TMR0ON=1;

 INTCONbits.T0IE=1;

//T = 32x250x125 = 1segundo;

//timer1 contador
 T1CONbits.TMR1CS=1;
 T1CONbits.T1CKPS1=0;
 T1CONbits.T1CKPS0=0;


 INTCONbits.T0IF=0;
 TMR0H=0;
 TMR0L=6; //250
 cnt=125; 
 INTCONbits.GIE=1;

 TMR1H=0;
 TMR1L=0;
 T1CONbits.TMR1ON=1;

  PORTCbits.RC2=1;
  for(i=0; i< 150; i++)
  {
    SetChanADC(ADC_CH2); 
    ConvertADC();
    while(BusyADC());  
    tmpi=( (((unsigned int)ReadADC())/64)*10)/2;
    lcd_cmd(L_L2+2);
    sprintf(str,"%05i",tmpi);
    lcd_dat(str[2]);
    lcd_dat(str[3]);
    lcd_dat(',');
    lcd_dat(str[4]);
    lcd_dat('C');

    lcd_cmd(L_L2+8);
    sprintf(str,"%05i",t1cont);
    lcd_dat(str[1]);
    lcd_dat(str[2]);
    lcd_dat(str[3]);
    lcd_dat(str[4]);
    lcd_dat('R');
    lcd_dat('P');
    lcd_dat('S');

    Delay1KTCYx(20);
  }

  INTCONbits.GIE=0;
  PORTCbits.RC2=0;


ADCON1=0x06;
TRISC=0x18;  


//teste RTC
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Teste RTC"));
  
//ajuste  rtc_w();

//  rtc_w();
 
  rtc_r();
  lcd_cmd(L_L2);
  lcd_str(date);
   Delay10KTCYx(400);
  for(i=0;i<20;i++)
  {
    rtc_r();
    lcd_cmd(L_L2);;
    lcd_str(time);
    Delay10KTCYx(100); 
  }


//dip
  TRISB=0x03;
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Des. DIS* e LED1"));
  lcd_cmd(L_L2);
  lcd_str(rom2ram("Press. RB1"));
  while(PORTBbits.RB1);



  TRISB=0xF8;
//teste Teclado
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1+2);
  lcd_str(rom2ram("Teste Teclado"));

  lcd_cmd(L_L2+1);

  i=0;
  while(i<14)
  {
    TRISD=0x0F;
    tmp=tc_tecla(1500)+0x30;
    TRISD=0x00;
    lcd_dat(tmp);
    i++;
  }



  TRISB=0x03;
//teste EEPROM INT
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Teste EEPROM INT"));
// testar ? 
  lcd_cmd(L_L2);
  lcd_str(rom2ram(" (s=RB0 n=RB1) ?"));


  while(PORTBbits.RB0 && PORTBbits.RB1);

  if(PORTBbits.RB0 == 0)
  {
    tmp=e2prom_r(10);
    lcd_dat(tmp);

    e2prom_w(10,0xA5);
    e2prom_w(10,0x5A);
    i=e2prom_r(10);

    e2prom_w(10,tmp);

    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str(rom2ram("Teste EEPROM INT"));
    lcd_cmd(L_L2);
    if(i == 0x5A) 
      lcd_str(rom2ram("       OK"));
    else
      lcd_str(rom2ram("      ERRO"));

    Delay10KTCYx(200);
  }
  else
  {
    while(PORTBbits.RB1 == 0);
  }


 
//teste EEPROM EXT
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("Teste EEPROM EXT"));
// testar ? 
  lcd_cmd(L_L2);
  lcd_str(rom2ram(" (s=RB0 n=RB1) ?"));


  while(PORTBbits.RB0 && PORTBbits.RB1);

  if(PORTBbits.RB0 == 0)
  {
    tmp=e2pext_r(10);
    lcd_dat(tmp);

    e2pext_w(10,0xA5);
    e2pext_w(10,0x5A);
    i=e2pext_r(10);

    e2pext_w(10,tmp);

    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str(rom2ram("Teste EEPROM EXT"));
    lcd_cmd(L_L2);
    if(i == 0x5A) 
      lcd_str(rom2ram("       OK"));
    else
      lcd_str(rom2ram("      ERRO"));

    Delay10KTCYx(200);
  }
  else
  {
    while(PORTBbits.RB1 == 0);
  }

  TRISB=0x00;
  PORTB=0;


//fim teste 
  lcd_cmd(L_CLR);
  lcd_cmd(L_L1);
  lcd_str(rom2ram("      Fim"));
  lcd_cmd(L_L2);
  lcd_str(rom2ram(" Pressione RST"));

  putsUSART(rom2ram("\r\n FIM!\r\n"));


   while(1);

}


//----------------------------------------------------------------------------
// High priority interrupt vector

#pragma code InterruptVectorHigh = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto isrh //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// Low priority interrupt vector

#pragma code InterruptVectorLow = 0x18
void
InterruptVectorLow (void)
{
  _asm
    goto isrl //jump to interrupt routine
  _endasm
}


//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt isrh

void isrh() 
{
  cnt--; 
  if(!cnt)
  {
//executada a cada 1 segundo
     T1CONbits.TMR1ON=0;
     t1cont=(((unsigned int)TMR1H<<8)|(TMR1L))/7; //ventilador com 7 p�s
     cnt=125; 
     TMR1H=0;
     TMR1L=0;
     T1CONbits.TMR1ON=1;
  }
  INTCONbits.T0IF=0;
  TMR0H=0;
  TMR0L=6; //250
}

//----------------------------------------------------------------------------

#pragma code
#pragma interrupt isrl

void isrl()
{
_asm
	NOP
_endasm;
}
