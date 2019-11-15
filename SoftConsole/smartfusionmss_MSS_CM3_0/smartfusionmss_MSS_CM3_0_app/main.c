#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "CarControl.h"
#include "drivers/mss_uart/mss_uart.h"
//#include "xbee.h"

void uart1_rx_handler( mss_uart_instance_t * this_uart ) {
	char rx_buff[8];
	int rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );
	if (rx_size > 0) {
		puts(rx_buff);
	}
}

int main() {
	//initialize_controller_reciever();
	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	//MSS_UART_enable_irq( &g_mss_uart1, MSS_UART_RBF_IRQ);
	MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);


	setLeftSideForward();
	setRightSideForward();

	setLeftSideSpeed(64);
	setRightSideSpeed(64);

	setLeftSideReverse();
	setRightSideReverse();

	setLeftSideSpeed(128);
	setRightSideSpeed(128);

	setLeftSideSpeed(0);
	setRightSideSpeed(0);

	while(1){

	}

}
