//Defining led diodes pin. 
#define LED_R1 11
#define LED_R2 10

#define LED_Y1 9
#define LED_Y2 6

#define LED_G1 5
#define LED_G2 3

//Defining button digital pin.
#define BUTTON 7

//Defining potentiometer analog pin
#define POTENTIOMETER 0

//Value for button
int buttonVal = 0;

//Max level for diodes to light
int diodeMaxVal = 100;

//Value for potentiometer
int potVal = 0;

//Current ledLight power
int ledLight = 0;

//Is potFunkA active
int isPotA = 0;

//Are diodes power incrementing or decrementing
int isInc = 1;

//All leds
int leds[] = {LED_R1, LED_R2, LED_Y1, LED_Y2, LED_G1, LED_G2};

//Number of leds total
int numOfLeds = 6;

//Input String for buffer
String inputString;

void setup() {
   
  Serial.begin(9600);

//pinMode for all leds
  for(int i = 0; i < numOfLeds; i++) {
      pinMode(leds[i], OUTPUT);
  }
  pinMode(BUTTON, INPUT);   
  pinMode(POTENTIOMETER, INPUT);
}

void loop() {

  if(Serial.available()) {
    serialControl();
  }
  else {
  //Reading the input value of button. If button == high == 1 then it's clicked on it, if button == LOW == 0 then it's released
    buttonVal = digitalRead(BUTTON);

  //Reading the input valufe of potentiometer. 0 - 1023.
    potVal = analogRead(POTENTIOMETER);
    
    if(!buttonVal) {  //if button is released then do funkA
      potFunkA();    
    }
    else {            //if button is held then do funB
      potFunkB();
    }
  }
}

//Serial controling arduino lightshow. First number is delay for each diode to light up on maximum power, second number is order of diods to light.
void serialControl() {
  for(int i = 0; i < numOfLeds; i++) {
    analogWrite(leds[i], 0);
  }
  
  inputString = Serial.readString();
  int delayTime = (inputString.substring(0, inputString.indexOf(' '))).toInt();
  inputString = inputString.substring(inputString.indexOf(' '), inputString.length());
  int led;
  for(int i = 0; i < inputString.length(); i++){
      char ch = inputString.charAt(i);
      switch(ch) {
        case '1': led = LED_R1; break;
        case '2': led = LED_R2; break;
        case '3': led = LED_Y1; break;
        case '4': led = LED_Y2; break;
        case '5': led = LED_G1; break;
        case '6': led = LED_G2; break;
      }

    analogWrite(led, 100);
    delay(delayTime);
    analogWrite(led, 0);
  }
}

//Increment and decrement power of light in diodes. Potentiometer decides speed of incrementing and decrementing
void potFunkA() {
  isPotA = 1;
  
  if(ledLight < diodeMaxVal && isInc == 1) {
    funkLedInc(leds, numOfLeds);
  }
  else {
    funkLedDec(leds, numOfLeds);
  }
}

//Increment and decrement power of light in diodes. Potentiometer decides number of active diodes
void potFunkB() {
  isPotA = 0;

  int currentAct = potVal/170; //1023 / 6 = 170.5

  //Temporary array of diodes for lightshow.
  int tempLeds[currentAct];
  
  for(int i = 0; i < currentAct; i++) {
    tempLeds[i] = leds[i];
  }
  
  if(ledLight < diodeMaxVal && isInc == 1) {
    funkLedInc(tempLeds, currentAct);
  }
  else {
    funkLedDec(tempLeds, currentAct);
  }
}

//Function for incrementing the power of light in diodes alternately.
void funkLedInc(int diods[], int n) {
  //Checking if we came to funkLedInc from potFunkA or potFunkB. If potFunkB then turn off all diodes except these in the temporary array.
  if(!isPotA) {
    for(int j = n; j < numOfLeds; j++) {
      analogWrite(leds[j], 0);
    }
  }
  
  for(int i = 0; i < n; i++) {
    if(i%2 == 0) {
      analogWrite(diods[i], ledLight);
    }
    else {
      analogWrite(diods[i], diodeMaxVal - ledLight);
    }
  }

  if(potVal >= 200 && isPotA == 1) {
    ledLight += potVal/50;
    if(ledLight > 100) {
      ledLight = 100;
    }
  }
  else {
    ledLight += 2;
  }
  
  if(ledLight >= 100) {
    isInc = 0;
  }
}

//Function for decrementing the power of light in diodes alternately.
void funkLedDec(int diods[], int n) {  
  for(int i = 0; i < n; i++) {
    if(i%2 == 0) {
      analogWrite(diods[i], ledLight);
    }
    else {
      analogWrite(diods[i], diodeMaxVal - ledLight);
    }
  }
  
  if(potVal >= 200 && isPotA == 1) {
    ledLight -= potVal/50;
    if(ledLight < 0) {
      ledLight = 0;
    }  
  }
  else {
    ledLight -= 2;
  }
  
  if(ledLight <= 0) {
    isInc = 1;
  }
}

