// Calvin Le
// 5/30/22
// UW Hyperloop
// temperature sensor arduino code

// add range/threshold for sensor faults
// periodic updates print out less often (print out every ## of data collected)
const int NUMOFSENSORS = 3;
const int OVERHEATVALUE = 100;
const float BETA = 3950; // beta coefficient of the thermistor


int sensorAddr[NUMOFSENSORS] = {A0, A1, A2};// add A# if adding more sensors
int outData[NUMOFSENSORS];
int loopCounter;
int prev[NUMOFSENSORS] = {0, 0, 0};

void setup() {
  // Configuring sensor pin as input
  for (int i = 0; i < NUMOFSENSORS; i++) {
    pinMode(sensorAddr[i],INPUT); 
  }
  // data rate
  Serial.begin(9600);

  for (int i = 0; i < NUMOFSENSORS; i++) {
    String sensorName = "temp " + String(i);
    prev[i] = outputTemp(sensorAddr[i], sensorName);
    delay(500);
  }



}

void loop() {
  for (int i = 0; i < NUMOFSENSORS; i++) {
    String sensorName = "temp " + String(i);
    outData[i] = outputTemp(sensorAddr[i], sensorName);
    delay(500);
  }

  loopCounter++;
  if (((outData[0]-5) >= prev[0]) || (outData[0] + 5 <= prev[0])) {
    Serial.println("sensor fault");
    delay(500);
    exit(0);
  }
  if (outData[0] > OVERHEATVALUE) {
    Serial.println("overheating");
    delay(500);
    exit(0);
  }
  delay(500);
  for (int i = 0; i < NUMOFSENSORS; i++) {
    prev[i] = outData[i];
  }
}

int outputTemp(int sensor, String name) {
  //Reading the value from sensor
  float sensorValue=analogRead(sensor); 

  // get celcius temp
  float tempc = 1 / (log(1 / (1023. / sensorValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

  // Converting to Fahrenheit
  float tempf=(tempc*1.8)+32; 
  if ((loopCounter % 3) == 0) {
    Serial.print(name);
    Serial.print(" in DegreeC=");
    Serial.print("\t");
    Serial.print(tempc);
    Serial.print(" ");
    Serial.print("in Fahrenheit=");
    Serial.print("\t");
    Serial.print(tempf);
    Serial.println();
  }
  
  return tempf;
}
