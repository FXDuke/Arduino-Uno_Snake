

// Global Variables

// LED Variables

int ScreenSize_X = 200;
int ScreenSize_Y = 200;

int ScreenMap_LED[ScreenSize_Y][ScreenSize_X] = {
  {{1,4,7,10},{2,5,8,11}...}, // screen map for getting the pin number of the led
};

bool SCREENCHANGED = false; // Change to true when turning LED on, this will make it so LED's are not being updated when nothing has been changed

bool ScreenMap[ScreenSize_Y][ScreenSize_X][1] = {
  {{true},{false}...}, // add all digital signals to each x here
};

// Snake Variables 

bool Gameplay = false; // will update game when true
int Speed = 1;
int Direction = 1; // or -1 
bool Up = false;
int CurrentSize = 1;
int Size[ScreenSize_Y*ScreenSize_X][2]; // Snake's max size is the size of the display
Size[0] = {round(ScreenSize_X/2),round(ScreenSize_Y/2)};

// Button Variables

int ControlSize = 4; // up down left right (for now)
int ButtonCooldown = 0;
int Minimum_Button_Read = 1000;
int ButtonCooldown_Amount = 200;

int ButtonMap[ControlSize] = {2,3,4,5}; // analog pins here

// Refresh Rate

int _RefreshRate;
int _OldRate = millis();

// LED Manager

void iterateLED(void (*returnFunction)(int,int)) { // function passed is called for every led
  for (int Index_Y = 0; Index_Y < ScreenSize_Y; Index_Y++) {
    for (int Index_X = 0; Index_X < ScreenSize_X; Index_X++) {
      returnFunction(Index_Y,Index_X);
    }
  }
}

void screenOff() { // turns all LED's off 
  void TurnOff(int Y, int X) {
    ScreenMap[Y][X][0] = false; 
  }
  iterateLED(TurnOff);
  SCREENCHANGED = true;
}

void screenBorder() { // Enables a 1 LED border surrounding the screen
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
  SCREENCHANGED = true;
}

void updateLED(int X,int Y, bool Value) { // set individual LED's value
  ScreenMap[Y][X][0] = Value;
  SCREENCHANGED = true;
}

void refreshScreen() { // Used in the loop to refresh the screen when an LED's state has been changed
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

// Button Manager

int[2] buttonDown() { // returns which button has the highest analog signal
  int greatest = 0;
  int FinalPin;
  for (int Index = 0; Index < ControlSize; Index++) {
    int Read = analogRead(ButtonMap[Index]);
    if (Read > greatest) {
      greatest = Read;
      FinalPin = Index;
    }
  }
  return FinalPin,greatest;
}

// Snake Movements

void MoveSnake() { // "Moves" the snake and updates the screen accordingly
  updateLED(Size[0][0],Size[0][1],false);
  if (Up == true) {
    Size[0][1] += Direction;
  } else {
    Size[0][0] += Direction;
  }
  updateLED(Size[0][0],Size[0][1],true);
  for (int Index = 0; Index < CurrentSize; Index++) {
    updateLED(Size[Index][0],Size[Index][1],false);
    Size[Index] = Size[Index-1];
    updateLED(Size[Index][0],Size[Index][1],true);
  }
  SCREENCHANGED = true;
}

// Main Functions 

void setup() {
  Serial.begin(9600);
  screenBorder(); // initializes screen border
  updateLED(round(ScreenSize_X/2),round(ScreenSize_Y/2),true); // snake start position
}

void loop() {
  
  _RefreshRate = millis()-_OldRate;
  _OldRate = _RefreshRate;
  
  if (ButtonCooldown == 0) {
    int ButtonHeld[2] = ButtonDown();
    if (ButtonHeld[1]>Minimum_Button_Read) {
      // add function stuff here
      
      ButtonCooldown = ButtonCooldown_Amount;
    }
  } else {
    ButtonCooldown = (ButtonCooldown-_RefreshRate>0) ? ButtonCooldown-_RefreshRate : 0;
  }
  
  if (SCREENCHANGED == true) {
    refreshScreen();
  }
  
  delay(10);
}

