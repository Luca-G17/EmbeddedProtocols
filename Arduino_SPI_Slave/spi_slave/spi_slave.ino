#define SCLK 2
#define SS 3
#define MOSI 4
#define MISO 5
#define MAX_RX_BYTES 255

void setup() {
  Serial.begin(9600);
  pinMode(SCLK, INPUT);
  pinMode(SS, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);

  digitalWrite(SS, HIGH); // Active low chip select
}


void spi_slave() {
  Serial.println("Recieving...");
  Serial.println();

  uint8_t prev_clk = LOW;
  uint8_t curr_clk = LOW;

  char curr_byte = 0;
  char rx_bytes[MAX_RX_BYTES];
  
  uint8_t byte_pos = 0;
  int8_t bit_pos = 7;

  unsigned long start = micros(); 
  while (digitalRead(SS) == LOW) {
    curr_clk = digitalRead(SCLK);
    if (curr_clk == HIGH && prev_clk == LOW) {

      // Read bit from MOSI
      char rx_bit = digitalRead(MOSI);
      curr_byte |= (rx_bit << bit_pos);
      bit_pos--;
      
      if (bit_pos < 0) {
        rx_bytes[byte_pos] = curr_byte;
        byte_pos++;
        bit_pos = 7;
        curr_byte = 0;
      }
    }
    prev_clk = curr_clk;
  }
  unsigned long end = micros(); 

  rx_bytes[byte_pos] = '\0';
  Serial.println(rx_bytes);

  int bits = 8 * byte_pos;
  float secs = (float)(end - start) / 1000000.0;
  float bit_rate = bits / secs;
  Serial.print("Rx (bits): ");
  Serial.print(bits);
  Serial.print(", Duration (s): ");
  Serial.print(secs, 6);        // 6 decimal places
  Serial.print(", bit-rate: ");
  Serial.println(bit_rate, 2);  // 2 decimal places
}

uint8_t prev_sel = LOW;
uint8_t chip_sel = HIGH;
void loop() {
  // Check for rising edge on Chip select
  chip_sel = digitalRead(SS);
  if (chip_sel == LOW && prev_sel == HIGH) {
    spi_slave();
  }
  prev_sel = chip_sel;
}
