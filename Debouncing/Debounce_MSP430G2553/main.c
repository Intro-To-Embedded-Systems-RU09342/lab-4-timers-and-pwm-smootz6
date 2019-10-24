#include <msp430.h>

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;               //stop watchdog timer

    P1DIR |= BIT0;                          //set P1.0 to output
    P1OUT &= ~BIT0;                         //turn off LED

    P1DIR &= ~BIT3;                         //set P1.3 input
    P1REN |= BIT3;                          //enable pull-up resistor
    P1IE |= BIT3;                           //enable the interrupt on P1.1
    P1IES |= BIT3;                         //set to flag on high to low
    P1IFG &= ~BIT3;                         //clear interrupt flag

    TA0CCR0 = 100;                           //set desired debouncing time
    TA0CCTL0 |= CCIE;                       //enable interrupts
    TA0CTL = TASSEL_1 + MC_0;               //timer0 set up

    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE);          //enable interrupts in LPM0
}
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {             //button interrupt
    P1OUT ^= BIT0;                          //toggle LED
    P1IFG &= ~BIT3;                         //P1.3 IFG cleared
    P1IE &= ~BIT3;                          //disable the interrupt on P1.1
    TA0CTL = TASSEL_1 + MC_1;               //start timer
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void) {     // Timer 0 interrupt{
    TA0CTL = TASSEL_1 + MC_0;               //stop timer
    TA0R = 0;                               //reset timer
    P1IE |= BIT3;                           //enable the interrupt on P1.1
}
