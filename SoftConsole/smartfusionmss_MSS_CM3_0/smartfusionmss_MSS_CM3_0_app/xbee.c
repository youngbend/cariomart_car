#include "xbee.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void initialize_controller_reciever(void) {
	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    //MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
}

/*
void uart1_rx_handler( mss_uart_instance_t * this_uart ) {
	char rx_buff[20];
	int rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );
	if (rx_size > 0) {
		strcpy(recieved_data, rx_buff);
		printf("%s", recieved_data);
	}
}
*/
