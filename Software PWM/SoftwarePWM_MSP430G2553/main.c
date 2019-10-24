#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               //stop watchdog timer

    P1DIR |= BIT0;                          //Set P1.0 as an output

    P1REN |= BIT3;                          // Enable Pull Up on SW2 (P1.3)
    P1IE |=  BIT3;                          // P1.3 interrupt enabled
    P1IES |= BIT3;                          // P1.3 set to high edge
    P1IFG &= ~BIT3;                         //Interrupt generator cleared

    TA0CTL |= TASSEL_2;                     //sets SMCLK as clock source
    TA0CTL |= ID_3;                         //set clk division to 8
    TA0CTL |= MC_1;                         //set to up mode
    TA0CTL &= ~TAIE;                        //disable timer interrupts
    TA0IV = 0x00;                           //resets interrupt vector
    TA0CCR0 = 0x1388;                       //set max timer value; about 5ms debounce time

    TA1CTL |= TASSEL_2 + MC_1;              //Sets SMCLK clock source and upmode
    TA1CCR0 = 999;                          //1kHz pulse
    TA1CCR1 = 499;                          //50% duty cycle
    TA1CCTL0 |= CCIE;                       //enable compare 0 interrupt
    TA1CCTL1 |= CCIE;                       //enable compare 1 interrupt
    TA1CTL |= TAIE;                         //enable timer interrupts

    __bis_SR_register(LPM0_bits + GIE);     //enter low power mode and enable interrupt
}

//interrupt vector for Timer 1, CCR0
void __attribute__((interrupt(TIMER1_A0_VECTOR))) timer1_A0 (void) {
    TA1CTL &= ~TAIFG;                       //clear interrupt flag
    P1OUT |= BIT0;                          //turn on led0
}

//interrupt vector for Timer 1, CCR1
void __attribute__((interrupt(TIMER1_A1_VECTOR))) timer1_A1 (void) {
    if(TA1IV == 0x02)                       //CCR1 compare interrupt
        P1OUT &= ~BIT0;                      //turn off led0
}

//interrupt vector for Timer 0
void __attribute__((interrupt(TIMER0_A1_VECTOR))) Timer_A0 (void) {
    P1IE |=  BIT3;                          // P1.3 interrupt enabled
    TA0CTL &= ~TAIE;                        //disable timer interrupts
}

//interrupt vector for port 1
void __attribute__((interrupt(PORT1_VECTOR))) Port1 (void) {
    P1IFG &= ~BIT3;                         //Clears P1.3 IFG

    TA1CCR0 = TA1CCR0 + 100;                //Increases PWM signal
    if(TA1CCR0 >= 1000) {                   //Checks maximum PWM
        TA1CCR0 = 0;                        //Resets to 0% duty cycle
    }

    P1IE &= ~BIT3;                           //Turns off port interrupts
    TA0CTL |= TACLR;                        //Resets timer
    TA0CTL |= TAIE;                         //enable timer interrupts
}
