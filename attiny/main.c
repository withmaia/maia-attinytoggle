/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>
#include <stdint.h>
#include "./lib/nrf24-noce.h"

/* ------------------------------------------------------------------------- */
/* Software UART routine. */
/* 2400 bps. Transmit only */
/* Transmit pin is: B4 */
/* ------------------------------------------------------------------------- */
/* Hardware specific section ... */
/* ------------------------------------------------------------------------- */
#include <util/delay.h>
#define uart_pin_output()    DDRB |= (1<<4)
#define uart_set_pin()        PORTB |= (1<<4)
#define uart_clr_pin()        PORTB &= ~(1<<4)
#define uart_bit_dly()        _delay_us(395)
/* ------------------------------------------------------------------------- */
/* Printing functions */
/* ------------------------------------------------------------------------- */
#include "./util/xprintf.h"
/* ------------------------------------------------------------------------- */
void uart_init()
{
    return;
    uart_pin_output();
}
/* ------------------------------------------------------------------------- */
void uart_put_char(uint8_t tx)
{
    return;
    uint8_t i;

    /* Start condition */
    uart_clr_pin();
    uart_bit_dly();

    for(i=0;i<8;i++)
    {
        if(tx & (1<<i))
        {
            uart_set_pin();
        }
        else
        {
            uart_clr_pin();
        }

        uart_bit_dly();
    }

    /* Stop condition */
    uart_set_pin();
    uart_bit_dly();
    uart_bit_dly();
}
/* ------------------------------------------------------------------------- */
uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
/* ------------------------------------------------------------------------- */
void send_char(uint8_t c) {

    /* Fill the data buffer */
    data_array[0] = 0x00;
    data_array[1] = 0xAA;
    data_array[2] = 0x55;
    data_array[3] = c;                                    

    /* Automatically goes to TX mode */
    nrf24_send(data_array);        
    
    /* Wait for transmission to end */
    while(nrf24_isSending());

    /* Make analysis on last tranmission attempt */
    temp = nrf24_lastMessageStatus();

    if(temp == NRF24_TRANSMISSON_OK) {                    
        xprintf("> Tranmission went OK\r\n");
    }
    else if(temp == NRF24_MESSAGE_LOST) {                    
        xprintf("> Message is lost ...\r\n");    
    }
    
    /* Retranmission count indicates the tranmission quality */
    temp = nrf24_retransmissionCount();
    xprintf("> Retranmission count: %d\r\n",temp);

    /* Optionally, go back to RX mode ... */
    //nrf24_powerUpRx();

    /* Or you might want to power down after TX */
    // nrf24_powerDown();            
}

int main()
{
    /* init the software uart */
    uart_init();

    /* init the xprintf library */
    xdev_out(uart_put_char);

    /* simple greeting message */
    xprintf("\r\n> TX device ready\r\n");
    
    /* init hardware pins */
    nrf24_init();
    
    /* Channel #2 , payload length: 4 */
    nrf24_config(2,4);

    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);    

    DDRB &= ~(1 << 4); // reading reed
    PORTB |= (1 << 4); // pull up

    int last_reading = -1;

    while(1) {                
        int reading = (PINB & (1 << 4));
        if (reading != last_reading) {
            if (reading)
                send_char('o');
            else
                send_char('c');
            last_reading = reading;
            _delay_ms(250);
        }

        /* Wait a little ... */
        _delay_ms(50);
    }
}

/* ------------------------------------------------------------------------- */
