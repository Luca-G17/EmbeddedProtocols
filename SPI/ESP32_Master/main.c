#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include <string.h>

#define SCLK 23
#define SS 22
#define MOSI 21
#define MISO 19
#define CLK_DELAY 20
#define MAX_RX 255

void spi_reset() {
    gpio_set_level(SCLK, 0); // SPI Clock
    gpio_set_level(SS, 1); // Active-low chip select
    gpio_set_level(MOSI, 0); // Master-output Slave-input
    gpio_set_level(MISO, 0); // Master-input Slave-output
}

void spi_duplex(const char* data_out, char data_in[MAX_RX]) {
    gpio_set_level(SS, 0); // Slave select

    const uint32_t n_bytes = strlen(data_out);
    for (uint32_t b = 0; b < MAX_RX; b++) {
        char byte_out = b < n_bytes ? data_out[b] : 0;
        char byte_in = 0;
        for (uint8_t i = 0; i < 8; i++) {
            gpio_set_level(MOSI, ((byte_out >> (7-i)) & 1)); // Tx data

            esp_rom_delay_us(CLK_DELAY);
            gpio_set_level(SCLK, 1);

            byte_in = (byte_in << 1) | gpio_get_level(MISO); // Rx Data

            esp_rom_delay_us(CLK_DELAY);
            gpio_set_level(SCLK, 0);
        }
        data_in[b] = byte_in;

        vTaskDelay(1);
    }

    gpio_set_level(SS, 1); // Slave deselect
}

void gpio_setup() {
    gpio_set_direction(SCLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(SS, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOSI, GPIO_MODE_OUTPUT);
    gpio_set_direction(MISO, GPIO_MODE_INPUT);
    uart_set_baudrate(UART_NUM_0, 9600);
    spi_reset();
}

void app_main() {
    gpio_setup();

    const char* send_str = "Hello, world!";
    char rx_str[MAX_RX];

    spi_duplex(send_str, rx_str);
}

