#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>  //Pendeklarasian Library

#define SS_PIN 10  // Pemilihan Pin dan pendeklarasian variabel
#define RST_PIN 9
#define LED_G 5
#define LED_R 4 
#define RELAY 3 
#define BUZZER 2 
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup(){  //Pengaturan Pin dan Variabel
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();   // memulai MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH);
  Serial.println("Letakkan kartu anda ke pembaca...");
  Serial.println();

}

void loop(){  //Perulangan Program
  if ( ! mfrc522.PICC_IsNewCardPresent())   {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())   {
    return;
  }
  //Menampilkan UID ke monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
//  i adalah nilai nya 0 trus selama 1 nya dbawah dari size atau nilai uid rfid maka dia akan bertambah terus 1 angka dari 0 ke 1 dan seterusnya
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "5A 8F 79 81") // memberi akses UID
  {
    Serial.println("Authorized access"); //Apabila menggunakan RFID Tag yang benar
    Serial.println();
    delay(500);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 100);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(RELAY, HIGH);
    noTone(BUZZER);
  }

  else   {
    Serial.println(" Access denied");  //Apabila menggunakan RFID Tag yang Salah
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 1000);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}
