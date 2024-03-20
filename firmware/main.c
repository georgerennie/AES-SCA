#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>
#include "aes.h"
#include "keys.h"

static void print_block(const aes_block_t block) {
	for (uint8_t i = 0; i < 16; i++)
		printf("%02X", block[i]);
	putchar('\n');
}

const uint8_t trig_mask = 1 << 2;

int main() {
	puts("\033[2J");

	DDRD |= trig_mask;

	for (;;) {
		PORTD &= ~trig_mask;
		puts("Enter 128 byte block (32 char hex string)");

		aes_block_t block;
		bool valid_block = true;

		for (uint8_t i = 0; i < 32; i++) {
			char c;
			scanf("%c", &c);

			uint8_t nibble;
			if (c >= '0' && c <= '9') {
				nibble = c - '0';
			} else if (c >= 'a' && c <= 'f') {
				nibble = c - 'a' + 10;
			} else if (c >= 'A' && c <= 'F') {
				nibble = c - 'A' + 10;
			} else {
				puts("Invalid char");
				valid_block = false;
				break;
			}

			if (i % 2 == 0)
				block[i / 2] = nibble << 4;
			else
				block[i / 2] |= nibble;
		}

		if (!valid_block)
			continue;

		print_block(block);

		bool keep_going = false;
		do {
			keep_going = false;

			puts("1: Calculate sub_bytes(block ^ key)");

			char mode;
			scanf("%c", &mode);
			switch (mode) {
				case '1': {
					PORTD |= trig_mask;
					aes_first_round_s_box(block, s_box_key);
					PORTD &= ~trig_mask;
					print_block(block);
				} break;
				default: {
					puts("Unknown option, try again");
					keep_going = true;
				} break;
			}
		} while (keep_going);
	}
}
