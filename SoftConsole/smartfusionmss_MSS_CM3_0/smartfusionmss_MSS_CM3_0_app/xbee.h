#ifndef XBEE_H
#define XBEE_H

#include <inttypes.h>
#include "drivers/mss_uart/mss_uart.h"

int turn_direction;
uint8_t gas_button;

// Initialize uart interface and function handler for controller data reception
void initialize_controller_reciever(void);

// Function handler for recieving of controller data
void controller_data_rx_handler( mss_uart_instance_t * this_uart );

#endif