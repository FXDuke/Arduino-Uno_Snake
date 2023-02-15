// Global Variables

int ScreenSize_X = 200;
int ScreenSize_Y = 200;

bool ScreenMap[ScreenSize_Y][ScreenSize_X][1] = {
  {{true},{false}...}, // add all digital signals to each x here
};

int ScreenMap_LED[ScreenSize_Y][ScreenSize_X] = {
  {{1,4,7,10},{2,5,8,11}...}, // screen map for getting the pin number of the led
};

bool SCREENCHANGED = false; // Change to true when turning LED on, this will make it so LED's are not being updated when nothing has been changed

// LED Manager

void iterateLED(void (*returnFunction)(int,int)) {
  for (int Index_Y = 0; Index_Y < ScreenSize_Y; Index_Y++) {
    for (int Index_X = 0; Index_X < ScreenSize_X; Index_X++) {
      returnFunction(Index_Y,Index_X);
    }
  }
}

void ScreenOff() {
  void TurnOff(int Y, int X) {
    ScreenMap[Y][X][0] = false; 
  }
  iterateLED(TurnOff);
}

void screenBorder() {
  for (int Index_Y = 0; Index_Y < ScreenSize_Y; Index_Y++) 
    if (Index_Y == 0 || Index_Y == ScreenSize_Y-1) { // For top of screen and botton
      for (int Index_X = 0; Index_X < ScreenSize_X; Index_X++) {
        ScreenMap[Index_Y][Index_X][0] = true;
      }
    } else {
      ScreenMap[Index_Y][0][0] = true;
      ScreenMap[Index_Y][ScreenSize_X-1][0] = true;
    }
  }
}

void refreshScreen() {
  void refreshLED(int X,int Y) {
    if (ScreenMap[Y][X][0] == true) {
      digitalWrite(ScreenMap_LED[Y][X], HIGH);
    } else {
      digitalWrite(ScreenMap_LED[Y][X], LOW);
    }
  }
  iterateLED(refreshLED);
  SCREENCHANGED = false;
}

void updateLED(int X,int Y, bool Value) {
  ScreenMap[Y][X][0] = Value;
  SCREENCHANGED = true;
}

// Button Manager

// wip

// Main Functions 

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (SCREENCHANGED == true) {
    refreshScreen();
  }
}

