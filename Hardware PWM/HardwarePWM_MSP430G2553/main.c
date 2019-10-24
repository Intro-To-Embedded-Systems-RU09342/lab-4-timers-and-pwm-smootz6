#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;               //holds watchdog timer

    P1DIR |= BIT6;                          //set P1.6 to output
    P1SEL |= BIT6;                          //P1.6 set to pwm
    P1OUT =  BIT3;                          //enables pull up resistor
    P1REN |= BIT3;                          //enables pull up resistor
    P1IE |= BIT3;                           //enables interrupt
    P1IES |= BIT3;                          //enable interrupt
    P1IFG &= ~BIT3;                         //flag set off at pin1.3

    TA0CCR0 = 999;                          //100% duty cycle
    TA0CCTL1 = OUTMOD_7;                    //set output mode
    TA0CCR1 = 499;                          //50% duty cycle
    TA0CTL = TASSEL_2 + MC_1;               //set up timer

    __bis_SR_register(LPM0_bits + GIE);     //enters lowpowermode0 w/ interrupt
}

#pragma vector=PORT1_VECTOR                 //interrupt triggers on button press
__interrupt void Button(void) {
    if (TA0CCR1 >= 1000) {                  //if at 100% duty cycle
        TA0CCR1 = 0;                        //sets duty cycle back to 0
    } else {
        TA0CCR1 += 100;                     // increment duty cycle by 10%
    }
    P1IFG = ~BIT3;                          // interrupt flag off
}
