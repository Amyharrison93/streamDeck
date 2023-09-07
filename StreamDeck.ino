#include <Keyboard.h>

#define NUM_SLIDERS 4
#define COLUMNS 12

const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3}; //analog pins to be read
const int digitalInputs[COLUMNS] = {2,3,4,5,6,7,8,9,10,11,16,14}; //digital pins to be read
const int Key[COLUMNS] = {KEY_F4, KEY_F3, KEY_F2, KEY_F1, KEY_F8, KEY_F7, KEY_F6, KEY_F5, KEY_F12, KEY_F10, KEY_F11, KEY_F9}; //specify your keybinds here
int inputFlags[COLUMNS] = {0,0,0,0,0,0,0,0,0,0,0,0}; //this should match how many buttons you have

int analogSliderValues[NUM_SLIDERS];
int value = 0;
int count = 0;

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  for (int i = 0; i < COLUMNS; i++) {
    pinMode(digitalInputs[i], INPUT);
    digitalWrite(digitalInputs[i], HIGH);
  }
  Serial.begin(115200);
  Keyboard.begin();
  delay(200);
}

void loop() {
  updateSliderValues();
  sendSliderValues();
  keypress(count);
  if(count >= COLUMNS){
    count = 0;
  }
  else{
    count++;
  }
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

void keypress(int i)
{  
  value = digitalRead(digitalInputs[i]);
  if(value == 0){
    value = 0;
  }
  else{
     value = 1;
  }
  if(value == 0){
    inputFlags[i] = 1;
  }
  else{
    inputFlags[i] = 0;
  }

  if(inputFlags[i] == 1){
    Keyboard.press(Key[i]);
    delay(10);
  }
  else if(inputFlags[i] == 0){
    Keyboard.release(Key[i]);
    delay(10);
  }
}