#include <stdio.h>
#include <stdint.h>

int main()
{
//	uint8_t message [] = {0xbd, 0x04, 0x03, 0xe7, 0x05, 0x1a};
	uint8_t message [] = {0xbd, 0x0a, 0x21, 0xe7, 0x0b, 0x00,
			      0x03, 0x90, 0x90, 0x78, 0x78, 0x3b};
	uint8_t checksum = 0xFF;
	int i;
	for (i = 1; i < 11; i++) {
		printf("Byte = %02X Checksum = %02X\n", message[i], checksum);
		checksum ^= message[i];
	}
	printf("checksum = %02X\n", checksum);
	printf("okay\n");
}
