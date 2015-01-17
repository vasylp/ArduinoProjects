

class LedArray{
  
#define nbits 8
  const int array[nbits] = {8,9,2,3,4,5,6,7};
  
  public:
  LedArray(){};
  
  void init(){
    for(int i = 0; i != nbits; ++i) pinMode(array[i], OUTPUT);
    print(0);
  }
  
  void print(int x){
    if(x < 0 || x > 255) return;
    for(int i=0; i != nbits; ++i){
      digitalWrite(array[i], (x%2)?HIGH:LOW);
      x /= 2;
    }
  }
  
  void setNth(int n, bool value){
    if(n < 0 || n >= nbits) return;
    digitalWrite(array[n], (value)?HIGH:LOW);
  }
};

class PushButton{
  const int clickTime = 100;
  const int releaseTime = 50;
  unsigned long lastActionTimestamp;
  bool isPressed;
  int pin;
  bool isClick;
  
  public:
  PushButton(int pin):lastActionTimestamp(0),isPressed(false),pin(pin),isClick(false){}
  
  void init(){
    pinMode(pin, INPUT);
  }
  
  bool click() { return isClick; }
  bool clickAndReset() {
    bool clk = isClick; 
    isClick = false;
    return clk;
  }
  
  void loop(){
    int buttonState = digitalRead(pin);
    if(isPressed && buttonState == LOW){
      unsigned long released = millis();
      isClick = (released - lastActionTimestamp) > clickTime;
      isPressed = false;
      lastActionTimestamp = released;      
    } else if(!isPressed && buttonState == HIGH){
      unsigned long pressed = millis();
      isClick = false;
      isPressed = true;
      lastActionTimestamp = pressed;
    } else if(!isPressed && buttonState == LOW && isClick){
      if(millis() - lastActionTimestamp > releaseTime){
        isClick = false;
      }
    }
  }
};

/*
  Main Programm
*/
LedArray ledArray;
PushButton buttonNext(12);
PushButton buttonPrev(11);

int ledOn = 4;

void setup() {
  ledArray.init();
  buttonNext.init();
  buttonPrev.init();
  
  Serial.begin(9600);
}

void loop() {
  buttonNext.loop();
  buttonPrev.loop();
  
  ledArray.setNth(ledOn, true);
  
  if(buttonNext.clickAndReset()){
    ledArray.setNth(ledOn, false);
    ledOn += 1;
  } if(buttonPrev.clickAndReset()){
    ledArray.setNth(ledOn, false);
    ledOn -= 1;
  }
  if(ledOn < 0) ledOn = 7;
  if(ledOn >= 8) ledOn = 0;
  
}
