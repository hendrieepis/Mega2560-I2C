#include <avr/io.h>
#include <util/delay.h>

void initBarLED() {
    // Set PORTA sebagai output
    DDRA = 0xFF; // Semua pin PORTA diatur sebagai output
}

void displayBarLED(uint8_t pattern) {
    // Karena BarLED menggunakan konfigurasi Common Anode (CA),
    // kita harus membalik logika dengan menggunakan NOT (~)
    PORTA = ~pattern;
}

int main(void) {
    // Inisialisasi PORTA untuk BarLED
    initBarLED();

    // Pola untuk menyalakan LED
    // Contoh pola: 0b00001111 akan menyalakan 4 LED di paling kanan
    uint8_t ledPattern[] = {
        0b00000001,  // Hanya LED pertama yang menyala
        0b00000011,  // Dua LED pertama menyala
        0b00000111,  // Tiga LED pertama menyala
        0b00001111,  // Empat LED pertama menyala
        0b00011111,  // Lima LED pertama menyala
        0b00111111,  // Enam LED pertama menyala
        0b01111111,  // Tujuh LED pertama menyala
        0b11111111   // Semua LED menyala
    };

    // Loop utama
    while (1) {
        // Loop melalui pola LED dan tampilkan di BarLED
        for (int i = 0; i < 8; i++) {
            displayBarLED(ledPattern[i]); // Tampilkan pola LED
            _delay_ms(500);               // Tunggu 500 ms sebelum pindah ke pola berikutnya
        }
        
        // Matikan semua LED selama 1 detik
        displayBarLED(0x00);   // Semua LED mati
        _delay_ms(1000);       // Tunggu 1 detik sebelum mengulangi lagi
    }
}
