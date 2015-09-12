/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>

#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

#define P_SCK 2
#define P_MISO 1
#define P_MOSI 0
#define P_CSN 3
#define P_CE 4

/* ------------------------------------------------------------------------- */
void nrf24_setupPins()
{
    set_bit(DDRB,P_CE); // CE output
    set_bit(DDRB,P_CSN); // CSN output
    set_bit(DDRB,P_SCK); // SCK output
    set_bit(DDRB,P_MOSI); // MOSI output
    clr_bit(DDRB,P_MISO); // MISO input
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,P_CE);
    }
    else
    {
        clr_bit(PORTB,P_CE);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,P_CSN);
    }
    else
    {
        clr_bit(PORTB,P_CSN);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,P_SCK);
    }
    else
    {
        clr_bit(PORTB,P_SCK);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,P_MOSI);
    }
    else
    {
        clr_bit(PORTB,P_MOSI);
    }
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
    return check_bit(PINB,P_MISO);
}
/* ------------------------------------------------------------------------- */
