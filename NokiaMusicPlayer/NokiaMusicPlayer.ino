
enum NoteName {
  nC = 0, nCs ,nD, nDs, nE, nF, nFs, nG, nGs, nA, nAs, nB
};

// a, #a, b, #b, c, #c, d, #d, e, #e, f, #f, g, g#
const NoteName noteMap[14] = {nA, nAs, nB, (NoteName)-1, nC, nCs, nD, nDs, nE, (NoteName)-1, nF, nFs, nG, nGs};

const int noteFq[7][12] = {
  {33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62},
  {65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123},
  {131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247},
  {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494},
  {523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988},
  {1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976},
  {2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951}
};

int atoi(const char* p, int len){
  int number = 0;
  for(int i = 0; i!= len; ++i){
    int n = p[i] - '0';
    number = number * 10 + n;
  }
  return number;
}

class Note{
  bool isPause;
  NoteName name;
  int duration;
  int octave;
public:
  Note(int duration):isPause(true),name(nC),duration(duration),octave(0){}
  Note(NoteName name, int duration, int octave):isPause(false),name(name),duration(duration),octave(octave){}
  Note(){}
  const char* parse(const char* noteString){
//    Serial.print("Parsing string: ");
//    Serial.println(noteString);
    
    const char* p = noteString;
    while(*p == ' ') ++p;
    
    const char* durationStart = p;
    while(*p >= '0' && *p <= '9') ++p;
    
    duration = atoi(durationStart, p-durationStart);
    
    if(*p == '.'){
      duration += duration / 2;
      ++p;
    }
//    Serial.print("Duration: "); Serial.println(duration);
    
    if(*p == '-') {
      isPause = true;
      return ++p;
    }
    isPause = false;
        
    int noteIdx = 0;
    if(*p == '#'){
      noteIdx = 1;
      ++p;
    }
    
    noteIdx = 2*(*p - 'a') + noteIdx;
    if(noteIdx >= 14) return p;
    
    ++p;
    
    name = noteMap[noteIdx];
    
//    Serial.print("Note name: "); Serial.println(name);

    const char* octaveStart = p;
    while(*p >= '0' && *p <= '9') ++p;
    octave = atoi(octaveStart, p - octaveStart);
    
//    Serial.print("Octave: "); Serial.println(octave);
    return p;

  }
  
  // plays the note via tone asynchronously, returs the duration
  int playAsync(int pin){
    int toneDuration = 1500/duration;
    if(!isPause){
      //Serial.print("Play note: "); Serial.print(" duration:"); Serial.print(toneDuration); Serial.print(" note:");Serial.println(noteFq[octave][name]);
      tone(pin, noteFq[octave][name], toneDuration);
    } else {
      noTone(pin);
    }
    return toneDuration;
  }
  
  int playSync(int pin){
    delay(playAsync(pin)*1.3);
    noTone(pin);
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

void playMelody(int pin, class Note* noteArray, int arrayLength){
  for(int i = 0; i!=arrayLength; ++i){
    noteArray[i].playSync(pin);
  }
}

PushButton button(12);

void setup() {
  Serial.begin(9600);
  button.init();
  /*
  const char* notes = "4g1 4#c2 4d2 4f2 8e2 2e2 4c2 4d2 4f2 8e2 2b1 4c2 4d2 4f2 8e2 "
  "2e2 4g2 4f2 4e2 8d2 2b1 4- 4f2 8e2 2e2 8d2 8e2 8g2 8f2 4e2 8d2 2a1 4- 4e2 8d2 2d2 4f2 4e2 4d2 "
  "8c2 2c2 4- 4d2 4#d2 4d2 4d2 8b1 2c2";
  */
  /*
  const char* notes = "16#c2 16#c2 8#c2 8#a1 8#g1 8#f1 4.#d1 8#g1 4.f1 8#d1 4.#f1 16#c2 16#c2 8#c2 8#a1 8#g1 8#f1 4.#d1"
  " 8#g1 4.f1 8#c2 4.#c2 16#c2 16#c2 8#c2 8#a1 8#g1 8#f1 4.#d1 8#g1 4.f1 8d1 2#d1 4- 8#d1 16#d1 8#f1 16#f1 8#f1 8#f1 8#a1 4#g1 8f1 2#f1";
  */
  /*
  const char* notes = "8#c1 8#c1 16#c1 2#f1 2#c2 8b1 16#a1 8#g1 2#f2 4#c2 8b1 16#a1 8#g1 2#f2 "
  "4#c2 8b1 16#a1 8b1 2#g1 8#c1 8#c1 16#c1 2#f1 2#c2 8b1 16#a1 8#g1 2#f2 4#c2 8b1 16#a1 8#g1 2#f2 4#c2"
  " 8b1 16#a1 8b1 2#g1 4#c1 16#c1 2#d1 8#c2 8b1 8#a1 8#g1 8#f1 16#f1 8#g1 16#a1 4#g1";*/
  
/*
  Note noteTest;
  noteTest.parse(noteTest.parse(noteTest.parse(noteTest.parse("32f2 32#d2 32f2 8#d2 32#d2 32#d2 32f2 32g2"))));
  // 32f2 32#d2 32f2 8#d2 32#d2 32#d2 32f2 32g2 32f2 16.#d2 32- 16f2 8#d2 16#g2 32#g2 32#g2 32#g2 16g2 16.#d2 32-
  Note melody[20] = {Note(nF, 32, 2), Note(nDs, 32, 2), Note(nF, 32, 2), Note(nDs, 8, 2),
                    Note(nDs, 32, 2), Note(nDs, 32, 2), Note(nF, 32, 2), Note(nG, 32, 2),
                    Note(nF, 32, 2), Note(nDs, 16, 2), Note(32), Note(nF, 16, 2), Note(nDs, 8, 2),
                    Note(nGs, 16, 2), Note(nGs, 32, 2), Note(nGs, 32, 2), Note(nGs, 32, 2),
                    Note(nG, 16, 2), Note(nDs, 16, 2), Note(32)};
                    
  playMelody(10, melody, 20);*/
}


const char* notes[5] = {"4a1 4a1 4a1 4f1 16c2 4a1 4f1 16c2 2a1 4e2 4e2 4e2 4f2 16c2 4#g1 4f1 16c2 2a1 4a2 4a1 16a1"
  " 4a2 4#g2 16g2 16#f2 16f2 4#f2 8#a1 4#d2 4d2 16#c2 16c2 16b1 4c2 8f1 4#g1 4f1 16#g1 4c2 4a1 16c2 2e2",
  "8a1 8a1 8b1 4a1 16c2 8- 16- 16c2 8- 16- 8c2 8e2 8d2 4f1 4- 8g1 8g1 8a1 4g1 16a1 8- 16- 8c2 8- 8c2 8d2 8c2 4g1",
  "8g2 8e2 8b1 8g2 8e2 8b1 8#f2 8e2 8b1 8#f2 8e2 8b1 8d2 8c2 8g1 8d2 8c2 8g1 8e2 8d2 8a1 8e2 8d2 8a1  8e2 16a1",
  " 16g2 16g2 8g2 16f2 16f2 8f2 2g2 8g2 8g2 8f2 8f2 2f2 16f2 16g2 8g2 16f2 16f2 8f2 16f2 2g2 8g2 8g2 8g2 16f2 16f2 16f2 16#d2 2c2 16g2 16g2 8g2 16f2 16#d2 8#d2 16f2 2g2 16#d2 16g2 16g2 8g2 8f2 8#d2 2#d2 16#d2 16f2 16f2 16f2 16f2 16f2 16#d2",
  "8c2 8#d2 8g1 8#g1 4c2 8- 8#g1 4g1 8- 8#g1 16g1 16#g1 8g1 8#f1 8g1 8c2 8#d2 8g1 8#g1 4c2 8- 8#g1 4g1 8- 8#g1 16g1 16#g1 8g1 8#f1 8g1 8c2 8#d2 8g1 8#g1 4c2 8- 8#g1 4g1 8- 8#g1 16g1 16#g1 8g1 8#f1 8g1"
  
};
int melodyNumber = 0;

void loop() {
  button.loop();
   
  if(button.clickAndReset()){
      const char* n = notes[melodyNumber];
      Note melody[75];
      int i = 0;
      for(; i!=75; ++i){
        if(*n == 0)break;
        n = melody[i].parse(n);
      }
      playMelody(10, melody, i);
      melodyNumber = (melodyNumber + 1) % 5;
  }

}
