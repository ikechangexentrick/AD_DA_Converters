#ifndef AD_DA_CONVERTERS__H
#define AD_DA_CONVERTERS__H
#include <SPI.h>

enum DA_Channel
{
	Channel_A, Channel_B
};

class MCP3208
{
public:
	MCP3208(SPISettings &settings, int PIN_CS)
		: settings(settings), PIN_CS(PIN_CS)
	{}

	unsigned int receive();

private:
	SPISettings &settings;
	const int PIN_CS;
};

class MCP4922
{
public:
	MCP4922(SPISettings &settings, int PIN_CS, int PIN_LATCH)
		: settings(settings), PIN_CS(PIN_CS), PIN_LATCH(PIN_LATCH)
	{}

	void emit(unsigned int data, DA_Channel ch);

private:
	SPISettings &settings;
	const int PIN_CS;
	const int PIN_LATCH;
};

#endif // AD_DA_CONVERTERS__H
