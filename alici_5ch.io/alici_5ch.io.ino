//  1 Channel Receiver | 1 Kanal Alıcı
//  PWM output on pin D5

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

int ch_width_5 = 0;

Servo ch5;

struct Signal {
byte aux1;      
};

Signal data;

const uint64_t pipeIn = 0xE8E8F0F0E1LL; //00322
RF24 radio(9, 10); 

void ResetData()
{

data.aux1 = 0;                                              // Define the inicial value of each data input. | Veri girişlerinin başlangıç değerleri
                                                            // The middle position for analog channels | Analog kanallar için orta konum
}

void setup()
{
                                                           // Set the pins for each PWM signal | Her bir PWM sinyal için pinler belirleniyor.
  Serial.begin(9600);                                     // Start Serial Monitor with 9600 baud rate

  ch5.attach(5);

                                                           
  ResetData();                                             // Configure the NRF24 module  | NRF24 Modül konfigürasyonu
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  radio.setChannel(100);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);                          // The lowest data rate value for more stable communication  | Daha kararlı iletişim için en düşük veri hızı.
  radio.setPALevel(RF24_PA_MAX);                            // Output power is set for maximum |  Çıkış gücü maksimum için ayarlanıyor.
  radio.startListening();                                   // Start the radio comunication for receiver | Alıcı için sinyal iletişimini başlatır.

}

unsigned long lastRecvTime = 0;

void recvData()
{
    while (radio.available()) {
        radio.read(&data, sizeof(Signal));    // Read the incoming data
        lastRecvTime = millis();              // Update the last received time

        // Print the received data to the Serial Monitor
        Serial.println("Data received:");
        Serial.print("Raw aux1: 0x "); 
        Serial.println(data.aux1, HEX);     // Assuming 'signalValue' is a member of the Signal structure
        Serial.print("Time: ");
        Serial.println(lastRecvTime);         // Print the time when data was received
    }
}


void loop()
{
recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData();                                                // Signal lost.. Reset data | Sinyal kayıpsa data resetleniyor
}

ch_width_5 = map(data.aux1, 0, 255, 1000, 2000);            // pin D5 (PWM signal)

ch5.writeMicroseconds(ch_width_5);                          // Write the PWM signal | PWM sinyaller çıkışlara gönderiliyor

}