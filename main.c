//ccs6 written by David Whitfield
//The program decodes an IR signal with the TSOP32438

#include "msp430.h"

#define LEDRed BIT3 //red LED
#define LEDGreen BIT4 //green LED
#define TXpin BIT5 //relay1 for light 1
#define IRrec BIT1 //connected to IR receiver for interrupt on rising edge of pulse
#define IRrec1 BIT0 //connected to IR receiver for interrupt on falling edge of pulse
#define Switch1 BIT3

int resetcount =990;
int loopcount=0;
int loopcountMAX =0;
int sendTx =0;
int count =0;
int transmitcount =0;
int transmit =0;
int one=0;
int two =0;
int three =0;
int four =0;
int five =0;
int six =0;
int seven =0;
int eight =0;
int bit =0;
int odd =0;

unsigned long realdata =0; //complete data
unsigned long rxdata =0; //uncomplete data



void main(void){

  WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer

  P1DIR |= (TXpin);     //output direction                       // Set all pins but RXD to output
  P1OUT =0;
  P2DIR |= (LEDGreen + LEDRed);     //output direction                       // Set all pins but RXD to output
  P2OUT =0;
  P1IE |= IRrec; // P1.3 interrupt enabled
  P1IES &= ~IRrec; // P1.3 interrupt enabled
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IE |= IRrec1; // P1.3 interrupt enabled
  P1IES |= IRrec1; // P1.3 interrupt enabled
  P1IFG &= ~IRrec1; // P1.3 IFG cleared
  P1REN |= Switch1;
  P1OUT |= Switch1;

  TA0CCTL0 |= CCIE;                             // CCR0 interrupt enabled
  TA0CTL = TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
  TA0CCR0 =  20;                            //count up to 20



    __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts



  while(1)
  {
    _delay_cycles(50);

    if(resetcount == 200)
    {
      loopcountMAX=loopcount;
      loopcount =0;
      realdata = rxdata >> 1;

    }
    if(realdata != 0) //signal present
    {
      realdata = 0;
      transmit =0;
    }
    if(transmit == 1)
    {
    	transmitcount++;
    	if(transmitcount > 1000)
    	{
    		transmitcount =0;
    	}
    	if(transmitcount == 5)
    	{
    		  P2OUT |=LEDGreen;
    		        P2OUT &=~LEDRed;
    	}
    	if (transmitcount == 505)
    	{
  		  P2OUT &=~LEDGreen;
  		  P2OUT |=LEDRed;
    	}
    	if(transmitcount %4 == 1)
    	    	{
    	    	if(sendTx == 1)
    	    	{
    	    		sendTx =0;
    	    		 P1OUT |= TXpin;
    	    	}
    	    	else
    	    	{
    	    		sendTx =1;
    	    		P1OUT &= ~TXpin;
    	    	}
    	    	}


    }
    else
    {
    	P1OUT |= TXpin;
    	P2OUT &= ~(LEDGreen + LEDRed);
    }

    if((P1IN & Switch1) != Switch1)
    {
    	transmit =1;
    }

    resetcount++;
    if(resetcount >1000)
    {
      resetcount =999;
    }
    if(count >1000)
    {
      count =999;
    }

  }
}




#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  TA0CTL |= TACLR;

  if(loopcount == 0)
  {
    rxdata =0;
  }
  if (loopcount == 50)
  {
    one = count;
  }
  else if (loopcount ==51)
  {
    two = count;
  }
  else if(loopcount ==52)
  {
    three = count;
  }
  else if(loopcount ==53)
  {
    four = count;
  }
  else if(loopcount == 54)
  {
    five = count;

  }
  else if(loopcount == 55)
  {
    six = count;

  }
  else if(loopcount == 56)
  {
    seven = count;

  }
  else if(loopcount == 57)
  {
    eight = count;

  }

  odd = (loopcount & 0x1);
  if(odd == 0)
  {
    if(count >13 && count < 24)
    {
      bit = 1;
    }
    else
    {
      bit = 0;
    }

    rxdata += bit;
    rxdata<<=1;
  }

  loopcount++;
  count=0;
  resetcount=0;
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IFG &= ~IRrec1; // P1.3 IFG cleared
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  count++;
}
