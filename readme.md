Saya menggunakan 2 buah mikrokontroller AVR atmega2560 agar bisa saling berkomunikasi menggunakan protokol I2C menggunakan framework Arduino. Master mingirimkan data dari 0-255 ke slave tiap 1 detik. Slave begitu menerima data mkan indikator LED pada PB7 aktif dan data di tampilkan ke port A (common Anoda)
Konfigurasi IO master dan slave sebagai berikut
pin 20 -> pin 20
pin 21 -> pin 21
GND -> GND

## Bagian Master

```c
#include <Wire.h>

#define SLAVE_ADDRESS 0x08 // Alamat slave
uint8_t dataToSend = 0; // Data yang akan dikirim (0-255)

void setup() {
 Wire.begin(); // Inisialisasi I2C sebagai master
}

void loop() {
 Wire.beginTransmission(SLAVE_ADDRESS); // Memulai transmisi ke slave
 Wire.write(dataToSend); // Mengirimkan data
 Wire.endTransmission(); // Mengakhiri transmisi

// Perbarui data yang akan dikirim (dari 0 ke 255 lalu kembali ke 0)
 dataToSend++;
 if (dataToSend > 255) {
 dataToSend = 0;
 }

delay(1000); // Tunggu 1 detik sebelum pengiriman berikutnya
}
```

## Bagian Slave

```c
#include <Wire.h>

#define LED_INDICATOR_PIN 7    // PB7 terhubung dengan LED indikator
#define BARLED_PORT PORTA      // BarLED terhubung dengan PORTA (Common Anode)

void setup() {
  // Inisialisasi I2C sebagai slave dengan alamat 0x08
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);  // Atur callback ketika data diterima

  // Atur PORTA sebagai output untuk BarLED
  DDRA = 0xFF;

  // Atur PB7 sebagai output untuk LED indikator
  DDRB |= (1 << LED_INDICATOR_PIN);

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

    delay(500);  // Tunda sedikit untuk memperlihatkan LED menyala

    // Matikan LED indikator pada PB7
    PORTB &= ~(1 << LED_INDICATOR_PIN);
  }
}
```

Kode di atas berhasil dengan baik. saya ingin kamu konvesikan kode pada master saja menjadi baremetal programming supaya saya tahu cara kerja i2C.
Pastikan kode yang kamu berikan work ya...
