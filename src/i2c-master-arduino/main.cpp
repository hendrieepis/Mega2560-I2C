#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x08  // Alamat slave
uint8_t dataToSend = 0;     // Data yang akan dikirim (0-255)

void setup() {
  Wire.begin();  // Inisialisasi I2C sebagai master
  Serial.begin(9600);  // Inisialisasi komunikasi serial untuk debug
}

void loop() {
  Wire.beginTransmission(SLAVE_ADDRESS);  // Memulai transmisi ke slave
  Wire.write(dataToSend);  // Mengirimkan data
  Wire.endTransmission();  // Mengakhiri transmisi
  
  // Tampilkan data di serial monitor
  Serial.print("Data dikirim: ");
  Serial.println(dataToSend);
  
  // Perbarui data yang akan dikirim (dari 0 ke 255 lalu kembali ke 0)
  dataToSend++;
  if (dataToSend > 255) {
    dataToSend = 0;
  }
  
  delay(1000);  // Tunggu 1 detik sebelum pengiriman berikutnya
}
