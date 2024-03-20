#include <avr/io.h>
#include <stdio.h>

#define DEBUG_BAUD 38400

static int uputchar0(char c, FILE* stream) {
	if (c == '\n')
		uputchar0('\r', stream);
	while (!(UCSR0A & _BV(UDRE0)))
		;
	UDR0 = c;
	return c;
}

static int ugetchar0(FILE*) {
	while (!(UCSR0A & _BV(RXC0)))
		;
	return UDR0;
}

__attribute__((constructor)) static void init_debug_uart0(void) {
	/* Configure UART0 baud rate, one start bit, 8-bit, no parity and one stop bit */
	UBRR0H = (F_CPU / (DEBUG_BAUD * 16L) - 1) >> 8;
	UBRR0L = (F_CPU / (DEBUG_BAUD * 16L) - 1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

	/* Setup new streams for input and output */
	static FILE uout = FDEV_SETUP_STREAM(uputchar0, NULL, _FDEV_SETUP_WRITE);
	static FILE uin = FDEV_SETUP_STREAM(NULL, ugetchar0, _FDEV_SETUP_READ);

	/* Redirect all standard streams to UART0 */
	stdout = &uout;
	stderr = &uout;
	stdin = &uin;
}
