#include <LiquidCrystal.h>
#include <Timer.h>
#include <Encoder.h>
#include <EEPROM.h>

#define START 6     // start i nastepny czas
#define WYBOR 7     // wybor i zapis czasu
#define PAUZA 10    // pauza
#define SILNIK1 A0   // silnik
#define SILNIK2 A2 
#define GLOSNIK A1  // glosnik

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Timer tt;
Timer Beep;
Encoder enc(8, 9);  // enkoder

int q, old_t, k, col = 0, row = 0, i, t[8], u, minute, second, T = 0, MIN, SEC;

boolean newState[3] = {false, false, false}, lastState[3] = {false, false, false}, start = false, ticking = false, UP = false, lastUP = false;

void setup() {
  pinMode(START, INPUT); 
  pinMode(WYBOR, INPUT); 
  pinMode(GLOSNIK, OUTPUT); 
  pinMode(PAUZA, INPUT); 
  pinMode(SILNIK1, OUTPUT); 
  pinMode(SILNIK2, OUTPUT);
  digitalWrite(SILNIK1, HIGH);
  digitalWrite(SILNIK2, HIGH);

  lcd.begin(20,4);
  
  for(u = 0; u <= 14; u = u + 2){
    t[u/2] = (int) ((EEPROM.read(u) & 0xFF) << 8) | (EEPROM.read(u+1) & 0xFF);
    delay(2);
  }
  
  int tick = tt.every(1000, count, 0);
  lcdUpdate(7, 8);
  timeUpdate();
}

void beep(int beeps){
  int beeper = Beep.oscillate(GLOSNIK, 150, LOW, beeps);
}

boolean button(int x){
  return lastState[x] == LOW && newState[x] == HIGH;
}

void setcursor(int x, int y){
  col = col + x;
  row = row + y;
  lcd.setCursor(col, row);
}

void count(){
  if(ticking){
    if(T == 0){
      digitalWrite(GLOSNIK, LOW);    
    }
    T++;
    old_t = t[i];
    --t[i];
    if(t[i] > 10){
      if(T%30 == 0 && T%60 != 0){
        beep(1);
      } else if(T%60 == 0){
        beep(2);
      }
    }
    if(t[i] <= 10 && t[i] > 0){
      beep(1);
    }
    if(t[i] <= 0){
      beep(3);          //pikanie
     } 
    if((t[i] == 599 && old_t == 600) || (t[i] == 5999 && old_t == 6000)){
      lcd.clear();
    }
    UP = !UP;
  }
}

void printNumber(int number, int m, int s){
  minute = number / 60;
  second = number - 60*minute;
  if(minute < 10 & m == 1) {
    lcd.print(0);
  }
  if(m == 1){
    lcd.print(minute);
    } else {
      setcursor(5, 0);
    }
  lcd.print(":");
  if(s == 1){
    if(second < 10 & s == 1){
      lcd.print(0);
    }
    lcd.print(second);
  }
}

void lcdUpdate(int x, int y){
  switch(x){
    case 7:
      col = 11; // 8
      row = 3;
      lcd.setCursor(col, row);
      if(y == 7){ 
        lcd.print("T8>");
      } else if(y == -8){
        lcd.print("T8p");
      }
      else {
        lcd.print("T8-");
      }
      printNumber(abs(t[7]), 1, 1);
    case 6:
      col = 11; // 7
      row = 2;
      lcd.setCursor(col, row);
      if(y == 6){
        lcd.print("T7>");
      } else if(y == -7){
        lcd.print("T7p");
      } else {
        lcd.print("T7-");
      }
      printNumber(abs(t[6]), 1, 1);
    case 5:
      col = 11;  // 6
      row = 1;
      lcd.setCursor(col, row);
      if(y == 5){
        lcd.print("T6>");
      } else if(y == -6){
        lcd.print("T6p");
      } else {
        lcd.print("T6-");
      }
      printNumber(abs(t[5]), 1, 1);
    case 4:
      col = 11; //5
      row = 0;
      lcd.setCursor(col, row);
      if(y == 4){
        lcd.print("T5>");
      } else if(y == -5){
        lcd.print("T5p");
      } else {
        lcd.print("T5-");
      }
      printNumber(abs(t[4]), 1, 1);
    case 3:
      col = 1;  // 4
      row = 3;
      lcd.setCursor(col, row);
      if(y == 3){
        lcd.print("T4>");
      } else if(y == -4){
        lcd.print("T4p");
      } else {
        lcd.print("T4-");
      }
      printNumber(abs(t[3]), 1, 1);
    case 2:
      col = 1;  // 3
      row = 2;
      lcd.setCursor(col, row);
      if(y == 2){
        lcd.print("T3>");
      } else if(y == -3){
        lcd.print("T3p");
      } else {
        lcd.print("T3-");
      }
      printNumber(abs(t[2]), 1, 1);
    case 1:
      col = 1; // 2
      row = 1;
      lcd.setCursor(col, row);
      if(y == 1){
        lcd.print("T2>");
      } else if(y == -2){
        lcd.print("T2p");
      } else {
        lcd.print("T2-");
      }
      printNumber(abs(t[1]), 1, 1);
    case 0:
      col = 1; //1
      row = 0;
      lcd.setCursor(col, row);
      if(y == 0){
        lcd.print("T1>");
      } else if(y == -1){
        lcd.print("T1p");
      } else {
        lcd.print("T1-");
      }
      printNumber(abs(t[0]), 1, 1);
    default: break;
  }
}

void lcdUpdate_set(int x, int m, int s){
  switch(x){
    case 0:
      col = 1; //1
      row = 0;
      lcd.setCursor(col, row);
      lcd.print("T1>");
      printNumber(abs(t[0]), m, s); break;
    case 1:
      col = 1; // 2
      row = 1;
      lcd.setCursor(col, row);
      lcd.print("T2>");
      printNumber(abs(t[1]), m, s); break;
    case 2:
      col = 1;  // 3
      row = 2;
      lcd.setCursor(col, row);
     lcd.print("T3>");
      printNumber(abs(t[2]), m, s); break;
    case 3:
      col = 1;  // 4
      row = 3;
      lcd.setCursor(col, row);
      lcd.print("T4>");
      printNumber(abs(t[3]), m, s); break;
    case 4:
      col = 11; //5
      row = 0;
      lcd.setCursor(col, row);
      lcd.print("T5>");
      printNumber(abs(t[4]), m, s); break;
    case 5:
      col = 11;  // 6
      row = 1;
      lcd.setCursor(col, row);
      lcd.print("T6>");
      printNumber(abs(t[5]), m, s); break;
    case 6:
      col = 11; // 7
      row = 2;
      lcd.setCursor(col, row);
      lcd.print("T7>");
      printNumber(abs(t[6]), m, s); break;
    case 7:
      col = 11; // 8
      row = 3;
      lcd.setCursor(col, row);
      lcd.print("T8>");
      printNumber(abs(t[7]), m, s); break;
    default: break;
  }
}

void timeSetup(){
  delay(500);
  MIN = t[i] / 60;
  SEC = abs(t[i] % 60);
  enc.write(MIN*2);
  beep(1);
  lcd.clear();
  while(true){                          //ustawienie minut
    newState[1] = digitalRead(WYBOR);
    if(button(1)){
      beep(1);
      break;
    }
    if(t[i] < 0){
      enc.write(0);
    }
    if(t[i] > 5940){
      enc.write(198);
    }
    if(enc.read()%2 == 0){
      t[i] =  enc.read()*30 + SEC;
    }
    lcdUpdate_set(i, 1, 0);
    lastState[1] = newState[1];
    Beep.update();
  }
  MIN = t[i] / 60;
  enc.write(t[i]*2);
  delay(500);
  lcd.clear();
  while(true){                           //ustawienie sekund
    newState[1] = digitalRead(WYBOR);
    if(button(1)){    
      byte fbyte = (t[i] >> 8) & 0xFF;  //zapis czasu
      byte sbyte = t[i] & 0xFF;
      EEPROM.write(i*2, fbyte);
      delay(2);
      EEPROM.write(i*2 + 1, sbyte);
      delay(2);
      beep(1);
      lcd.clear();
      lcdUpdate(7, i);
      break;
    }
    if(t[i] < 0){
      enc.write(0);
    }
    if(t[i] > 5940){
      enc.write(0);
    }
    if(t[i] < MIN*60){
      enc.write((t[i] + 60)*2);
    }
    if(t[i] >= MIN*60 + 60){
      enc.write((t[i] - 60)*2);
    }
    t[i] =  enc.read()/2;
    lcdUpdate_set(i, 0 , 1);
    lastState[1] = newState[1];
    Beep.update();
    }
  enc.write(i*2);
  delay(200);
 }

void timeUpdate(){
    delay(500);
    int old_i = 1;
    enc.write(0);
    while(true){
      newState[0] = digitalRead(START);
      newState[1] = digitalRead(WYBOR);     
      if(button(0)){
        delay(500);
        ticking = true;
        digitalWrite(SILNIK1, LOW);
        digitalWrite(SILNIK2, LOW);
        beep(1);
        break;
      }
      if(button(1)){
        timeSetup();
        ++i;
      }
      i = enc.read()/2;
      if(enc.read() < 0){
        enc.write(14);
      }
      if(enc.read() > 15){
        enc.write(0);
      }
      if(i != old_i){
        beep(1);
        lcd.clear();
        lcdUpdate(7, i);
      }
         
      lastState[0] = newState[0];
      lastState[1] = newState[1];
      
      old_i = i;
      Beep.update();
    }
    lcd.clear();
    i = 0;
  }
 

void loop() {
  newState[0] = digitalRead(START);
  newState[1] = digitalRead(WYBOR);
  newState[2] = digitalRead(PAUZA);
  
  if(button(0)){                                  // nastepny czas
    newtime:
    delay(500);
    t[i] = T;
    ticking = true;
    digitalWrite(SILNIK1, LOW);
    digitalWrite(SILNIK2, LOW);
    T = 0;
    beep(1);
     ++i;
     if(i == 8){
      lcdUpdate(7, 8);
      digitalWrite(SILNIK1, HIGH);
      digitalWrite(SILNIK2, HIGH);
      lastState[0] = LOW;
      newState[0] = LOW;
      while(true){
        newState[0] = digitalRead(START);
        if(button(0)){
          break;
        }
        delay(100);
        Beep.update();
        lastState[0] = newState[0];
      }
      for(u = 0; u <= 14; u = u + 2){
        t[u/2] = (int) ((EEPROM.read(u) & 0xFF) << 8) | (EEPROM.read(u + 1) & 0xFF);
        delay(2);
      }
      beep(1);
      lcd.clear();
      digitalWrite(SILNIK1, LOW);
      digitalWrite(SILNIK2, LOW);
      i = 0;
     }
    }

  if(button(2)){                                  // pauza
    lcdUpdate(i, -i-1);
    delay(500);
    ticking = false;
    beep(1);
    digitalWrite(SILNIK1, HIGH);
    digitalWrite(SILNIK2, HIGH);
    while(true){
      newState[0] = digitalRead(START);
      newState[2] = digitalRead(PAUZA);
      
      if(button(0)){                              // koniec pauzy i nastepny czas
        goto newtime;
        break;
      }
      
      if(button(2)){                              // koniec pauzy
        beep(1);
          break;
      }
      
      lastState[0] = newState[0];
      lastState[2] = newState[2];
      Beep.update();
    } 
    delay(400);
    ticking = true;
    digitalWrite(SILNIK1, LOW);
    digitalWrite(SILNIK2, LOW);
  }

  lastState[0] = newState[0];
  lastState[1] = newState[1];
  lastState[2] = newState[2];

  if(UP != lastUP){
    lcdUpdate(i, 8);
    lastUP = UP;
  }
  tt.update();
  Beep.update();
}
