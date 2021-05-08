#include <OneWire.h>
const int IBUTTON_PIN = 2;
OneWire iButton(IBUTTON_PIN); // создаём объект 1-wire на 2 выводе

void setup (void) {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop(void) {
  delay(1000); // задержка 1 сек
  byte addr[8]; // массив для хранения данных ключа
  Serial.println("Waiting for a original . . .");
  while ( !iButton.search(addr) ) { // если ключ не приложен
      iButton.reset_search();
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(500);                       // wait
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(500);
  }

Serial.print("Key : ");
  for(int i=0; i<8; i++) {
    Serial.print(addr[i], HEX); // выводим побайтно данные ключа
    Serial.print(" ");
  }
  Serial.println();
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(2000);                       // wait
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(100);

  
  
  Serial.println("Waiting for a clone . . .");
  
  byte new_addr[8]; // массив для хранения данных ключа
  while (true){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(300);                       // wait
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(300);
    if(!iButton.search(new_addr)) continue;
    bool isSame = true;
    for(int i=0; i<8; i++){
      if(addr[i] != new_addr[i]){
        isSame = false;
        break;
      }
    }
    if(isSame){
      Serial.println("Got the same button, retrying . . .");
      continue;
    }

    
    Serial.println("Got new button key : ");
    for(int i=0; i < 8; i++) {
      Serial.print(new_addr[i], HEX); // выводим побайтно данные ключа
      Serial.print(" ");
    }
    Serial.println();
    Serial.println("Overwriting new ID . . .");
    iButton.skip();
    iButton.reset();
    iButton.write(0xD1);

    digitalWrite(IBUTTON_PIN, LOW); 
    pinMode(IBUTTON_PIN, OUTPUT); 
    delayMicroseconds(60);
    pinMode(IBUTTON_PIN, INPUT); 
    digitalWrite(IBUTTON_PIN, HIGH);
    delay(10);

    
    iButton.skip(); 
    iButton.reset(); 
    iButton.write(0xD5);
    
    for (byte i = 0; i<8; i++){
      writeByte(addr[i]);
      Serial.print("* ");
    }
    Serial.print('\n');
    
    iButton.reset(); // send 0xD1
    iButton.write(0xD1); //send logical 1
    digitalWrite(IBUTTON_PIN, LOW); 
    pinMode(IBUTTON_PIN, OUTPUT); 
    delayMicroseconds(10);
    pinMode(IBUTTON_PIN, INPUT); 
    digitalWrite(IBUTTON_PIN, HIGH); 
    delay(10);

    
    Serial.println("Overrided!");
    iButton.skip(); iButton.reset(); iButton.write(0x33);
    Serial.print("ID after write:");
    for (byte x = 0; x < 8; x++) {
      Serial.print(' '); Serial.print(iButton.read(), HEX);
    }
    for(int i=0; i<10;i++){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);                       // wait
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(100);
    }
    return; 
  }
}

///Производит запись одного байта данных в порт с iButton
int writeByte(byte data){
  int data_bit;
  for(data_bit=0; data_bit<8; data_bit++){
    if (data & 1){
      digitalWrite(IBUTTON_PIN, LOW); pinMode(IBUTTON_PIN, OUTPUT);
      delayMicroseconds(60);
      pinMode(IBUTTON_PIN, INPUT); digitalWrite(IBUTTON_PIN, HIGH);
      delay(10);
    } else {
      digitalWrite(IBUTTON_PIN, LOW); pinMode(IBUTTON_PIN, OUTPUT);
      pinMode(IBUTTON_PIN, INPUT); digitalWrite(IBUTTON_PIN, HIGH);
      delay(10);
    }
    data = data >> 1;
  }
  return 0;
}
