#include "xbee.h"
#include "CarControl.h"
#include <stdio.h>
#include <string.h>

int curr_button_state=0;

void XBEE_init(void) {
	MSS_UART_init(&g_mss_uart1, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
}

void XBEE_send(char* string) {
	MSS_UART_polled_tx_string(&g_mss_uart1, (uint8_t*)string);
}

void uart1_rx_handler( mss_uart_instance_t * this_uart ) {
	char rx_buff[1];
	char received_data[50];
	int accel=0;
	int i = 0;
	int no_rcv = 0;
	int rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );

	if (*rx_buff == '\r' || *rx_buff == '\n') rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );
	if (*rx_buff == '\r' || *rx_buff == '\n') rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );

	while (!(*rx_buff == '\r' ||  *rx_buff == '\n') && i < 50 && no_rcv < 500) {
		if (rx_size > 0 ) {
			received_data[i++] = *rx_buff;
			no_rcv = 0;
		}
		else no_rcv++;
		rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );
	}
	received_data[i] = '\0';

	// Process the packet string received data, i is the length of the received data

	// Process accelerometer data and translate it to motor output
	if(received_data[0]=='2' && strlen(received_data) == 7 && game_status == 1){
		int new_button_state;

		//Packet structure  = packet_header("2"),button_state,accel_data
		new_button_state = received_data[2] - '0';

		sscanf(received_data+4, "%d", &accel);

		// Calculate individual side speeds
		int left_speed = max_speed - (ACCEL_CENTER - accel) - OFFSET;
		int right_speed = max_speed + (ACCEL_CENTER - accel) + OFFSET;
		if (left_speed > 255) left_speed = 255;
		else if (left_speed < 0) left_speed = 0;
		if (right_speed > 255) right_speed = 255;
		else if (right_speed < 0) right_speed = 0;

		if(new_button_state == 1 && curr_button_state != 1){
			setLeftSideForward();
			setRightSideForward();
			setLeftSideSpeed(left_speed);
			setRightSideSpeed(right_speed);
		}
		else if(new_button_state == 1){
			setLeftSideSpeed(left_speed);
			setRightSideSpeed(right_speed);
		}
		else if(new_button_state == 2 && curr_button_state != 2) {
			setLeftSideReverse();
			setRightSideReverse();
			setLeftSideSpeed(left_speed);
			setRightSideSpeed(right_speed);
		}
		else if(new_button_state == 2){
			setLeftSideSpeed(left_speed);
			setRightSideSpeed(right_speed);
		}
		else{
			setLeftSideSpeed(0);
			setRightSideSpeed(0);
		}
		curr_button_state = new_button_state;
	}

	// Receive the game status packet
	else if (received_data[0] == '0' && strlen(received_data) == 3) {
		game_status = (received_data[2] == '0') ? 0 : 1;
	}

	// Receive powerups
	else if (received_data[0] == '4' && strlen(received_data) == 3) {
		if (received_data[2] == '0') CarControl_boost();
		else if (received_data[2] == '1') CarControl_spinout();
	}



}
