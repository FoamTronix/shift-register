#define ONE_SECOND 1000
#define SHIFT_REGISTER_COUNT 2
#define REGISTER_LIGHT_COUNT 8 // 8 output pins on 74HC595

// Lights
// ##########################################
#define DS_PIN 4   // Data pin
#define STCP_PIN 5 // Latch pin
#define SHCP_PIN 6 // Clock pin
#define LIGHT_COUNT (SHIFT_REGISTER_COUNT * REGISTER_LIGHT_COUNT)
#define WHITE_LIGHTS_REGSITER 1
#define COLOR_LIGHTS_REGISTER 2
boolean lights[LIGHT_COUNT];
// ##########################################

// Commands from serial
// ##########################################
String command; // Used to process commands from input

void setup() {
  Serial.begin(9600);
  initLights();
}

void loop() {
  if(Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    processCommand();
  }
}

void processCommand() {
  if(command == "DisplayLights") { displayLights(); } 
  else if(command == "AllLightsOn") { allLightsOn(); } 
  else if(command == "AllLightsOff") { allLightsOff(); }
  else if(command == "ColorLightsOn") { colorLightsOn(); }
  else if(command == "ColorLightsOff") { colorLightsOff(); }
  else if(command == "WhiteLightsOn") { whiteLightsOn(); }
  else if(command == "WhiteLightsOff") { whiteLightsOff(); }
}

//##########################################
// Lights
//##########################################
void initLights() {
  pinMode(DS_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(SHCP_PIN, OUTPUT);
  
  allLightsOff();
}

void displayLights() {
  char buffer[20]; 
  Serial.println("------------------------------------");
  for(int i=0; i < LIGHT_COUNT; i++) {
    sprintf (buffer, "Light %d: %d", i, lights[i]);
    Serial.println(buffer);
  }
  Serial.println("------------------------------------");
}

void allLightsOn() {
  for(int i=0; i<LIGHT_COUNT; i++) {
    lights[i] = HIGH;
  }
  updateLights(); 
}

void allLightsOff() {
  for(int i=0; i<LIGHT_COUNT; i++) {
    lights[i] = LOW;
  }
  updateLights();
}

void colorLightsOn() {
  updateRegisterPosition(COLOR_LIGHTS_REGISTER, HIGH);
  updateLights();
}

void colorLightsOff() {
  updateRegisterPosition(COLOR_LIGHTS_REGISTER, LOW);
  updateLights();
}

void whiteLightsOn() {
  updateRegisterPosition(WHITE_LIGHTS_REGSITER, HIGH);
  updateLights();
}

void whiteLightsOff() {
  updateRegisterPosition(WHITE_LIGHTS_REGSITER, LOW);
  updateLights();
}

void updateLights() {
  digitalWrite(STCP_PIN, LOW);
  for (int i = LIGHT_COUNT-1; i >= 0; i--) {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(DS_PIN, lights[i]);
    digitalWrite(SHCP_PIN, HIGH);
  }
  digitalWrite(STCP_PIN, HIGH);   
}

void updateRegisterPosition(int register_num, int value) {
  int start_val = (register_num - 1) * REGISTER_LIGHT_COUNT;
  int end_val = start_val+REGISTER_LIGHT_COUNT;
  for(int i=start_val; i<end_val; i++) {
    lights[i] = value;
  }
}
//##########################################
//##########################################
