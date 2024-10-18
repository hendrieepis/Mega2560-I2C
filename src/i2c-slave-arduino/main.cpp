#include <Arduino.h>
#include <Wire.h>

#define LED_INDICATOR_PIN 7    // PB7 terhubung dengan LED indikator
#define BARLED_PORT PORTA      // BarLED terhubung dengan PORTA (Common Anode)

// Deklarasi fungsi callback di atas
void receiveEvent(int howMany);

void setup() {
  // Inisialisasi I2C sebagai slave dengan alamat 0x08
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);  // Atur callback ketika data diterima
  
  // Atur PORTA sebagai output untuk BarLED
  DDRA = 0xFF;
  
  // Atur PB7 sebagai output untuk LED indikator
  DDRB |= (1 << LED_INDICATOR_PIN);
  
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Loop utama kosong karena penanganan data dilakukan dalam fungsi receiveEvent
}

// Fungsi callback saat data diterima
void receiveEvent(int howMany) {
  if (Wire.available()) {
    uint8_t receivedData = Wire.read();  // Membaca data yang dikirim master
    
    // Tampilkan data yang diterima ke BarLED (Common Anode)
    BARLED_PORT = ~receivedData;  // Inversi karena Common Anode
    
    // Nyalakan LED indikator pada PB7
    PORTB |= (1 << LED_INDICATOR_PIN);
    
    // Tampilkan data yang diterima di Serial Monitor
    Serial.print("Data diterima: ");
    Serial.println(receivedData);
    
    delay(500);  // Tunda sedikit untuk memperlihatkan LED menyala
    
    // Matikan LED indikator pada PB7
    PORTB &= ~(1 << LED_INDICATOR_PIN);
  }
}
