#include "AD_DA_Converters.h"
#include <SPI.h>

unsigned int MCP3208::receive() {
	SPI.beginTransaction(settings);
		digitalWrite(PIN_CS, LOW);
			SPI.transfer(0b00000110); // start bit:1, ch 0 single end
			byte high_byte = SPI.transfer(0b00000000);
			byte low_byte = SPI.transfer(0x00); // dummy
		digitalWrite(PIN_CS, HIGH);
	SPI.endTransaction();

	unsigned int data_ch0 = ((high_byte & 0x0f) << 8) + low_byte;
	return data_ch0;
}

void MCP4922::emit(unsigned int data, DA_Channel ch)
{
	int flag = 0;
	if (ch == Channel_A) flag = 0x30; // ch A, no buf, 1x, no shdn (0b0011)
	else flag = 0xB0; // ch B, no buf, 1x, no shdn (0b1011)

	SPI.beginTransaction(settings);
		digitalWrite(PIN_LATCH, HIGH);
		digitalWrite(PIN_CS, LOW);
			SPI.transfer((data >> 8) | flag ); // upper 4 bits 
			SPI.transfer( data & 0xff ); // lower 8 bits
		digitalWrite(PIN_CS, HIGH);
		digitalWrite(PIN_LATCH, LOW);
	SPI.endTransaction();
}


/* A-D measurement
512  -> 0.999 v / 0.625 v
1536 -> 2.000 v / 1.875 v
2560 -> 3.002 v / 3.125 v
3584 -> 4.00 v  / 4.375 v

-> 9.768e-4 V/d
*/
double ad_to_volts(unsigned int ad_out)
{
	return 9.76888e-4*(ad_out - 1536) + 2.000;
}

/* D-A measurement
0    -> 0.0014 v
512  -> 0.5935 v
1536 -> 1.786  v
2560 -> 2.973  v
3584 -> 4.17   v
4095 -> 4.76   v
*/
double expected_da_out(unsigned int da_in)
{
	return 1.1607e-3*(da_in - 1536) + 1.786;
}

unsigned int volts_to_da(double v)
{
	return 861.5491*(v - 1.786) + 1536;
}


