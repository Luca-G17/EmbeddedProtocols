# Serial Peripheral Interface

SPI is a fully duplex 4 wire synchronous serial communication protocol (MISO/MOSI/SCLK/CS):
- MISO: Master Input Slave Output
- MOSI: Master Output Slave Input
- SCLK: Clock
- CS: Active low chip select

SPI can operate in different modes depending on wether we transmit/recieve on the rising or falling edge of the clock. This implementation is using **CPOL=0, CPHA=0 (Clock Polarity, Clock Phase)**. Generally each slave will have it's own chip select pin, enabled on demand by the master with a common MISO, SCLK and MOSI. The throughput is determined by the clock speeds of the transmitter and reciever as well as the transmission medium. No bus arbitration is required thanks to the single master node. The throughput can be extended using Dual-SPI where full duplex is not required, both data lines can be used to send two bits. This is further extended by Quad-SPI (QSPI) which uses a further two I/O lines. 
