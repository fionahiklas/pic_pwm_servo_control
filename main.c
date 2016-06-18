/*
 * File:   main.c
 *
 * Original code copied from here: 
 *   http://saeedsolutions.blogspot.co.uk/2013/09/pic16f628a-pwm-code-proteus-simulation.html?m=1
 */


#include <xc.h>
#include <pic16f628a.h>

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

// Don't really need this for this program, in fact it should be off 
// otherwise it'll keep restarting your code!
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)

// Make damn sure that this is OFF otherwise your PIC will go mental if you 
// even wave your hand near it or the circuit!  Do NOT waste the hours I did 
// on trying to 'debug' weird glitches and pauses
#pragma config LVP = OFF         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


void main(void) {
    initPWM();
    setPWMDutyCycle(125);
    
    enablePWM();
    while(1);
}


/**
 * According to the datasheet the period is given by the following
 *   Period = (PR2 + 1) . 4. Tosc . T2 Prescaler
 * 
 * Since the oscillator is 4MHz internal, Tosc = 250ns
 * So period = 4.096ms, frequency = 244.14Hz
 */
void initPWM() {
    TRISB3=0;        // B3 is output of CCP1
    CCP1CON = 0x0C;  // CCP1 module is set to PWM
    
    PR2 = 0xFF;
    T2CON = 0x03; // Set prescaler to 16
}

/**
 * The high time is given by the following formula
 * 
 *   High period = (CCPR1L:CCP1CON<5:4>) . Tosc . T2 Prescaler
 * 
 * The duty cycle value is 10bits in total.
 * 
 * For a value of, say, 10, the time would be 40uS.
 * For 100 it would be 400uS, 1ms would need a value of 250.
 * 
 * @param dutyCycleValue
 */
void setPWMDutyCycle(unsigned int dutyCycleValue) {
    CCPR1L = dutyCycleValue >> 2;
    CCP1CON &= 0xCF;
    CCP1CON |= (0x30 & (dutyCycleValue << 4));
}


void enablePWM() {
    T2CON |= 0x04;
}

