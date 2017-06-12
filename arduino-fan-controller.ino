/*
  Arduino-based FAN Manager 

  Components: LM35 Temperature sensor
              Arduino UNO
              SRD-05VDC-SL-C switch

  Usage:
  Control a 220V fan using an arduino and a temperature sensor.
  When the temperature goes beyond a given threshold, the Arduino
  automatically starts the fan, and keeps it running until the temperature
  drops to an acceptable value.
*/

bool is_fan_running = false;

const float power_on_threshold = 24; /* Max temperature allowed before fan is triggered */
const float power_off_threshold = 24; /* Temperature to reach to switch off fan while it's running */
const int delay_between_loops_in_ms = 5000; /* Delay (in ms) to wait between temperature read */
const bool debug = true;

// PORTS
const int temp_sensor_port = A0;
const int fan_switch_port = 6;

void setup() {
  Serial.begin(9600);
  pinMode(fan_switch_port, OUTPUT);

  // Setup: power off the fan if the port is HIGH
  powerOffFan();
}

/*
  Main loop:
   - Read temperature
   - if temp > max_threshold and fan is not yet running => trigger the fan
   - if temp < min_threshold and fan is running => stop the fan
   - wait until next loop
 */
void loop() {
  float current_temp = getTemperature();

  if(current_temp >= power_on_threshold && !is_fan_running){
    powerOnFan();
  }
  else if(current_temp <= power_off_threshold && is_fan_running){
    powerOffFan();
  }

  wait();
}

/*
 * Reads and return the LM35 sensor value.
 * Temperature returned is in Celsius
 */
float getTemperature(){
  int returned_value = analogRead(temp_sensor_port);
  float celsius_temp = returned_value * (5.0 / 1023.0 * 100.0);

  if(debug) Serial.println(celsius_temp);
  
  return celsius_temp;
}

/*
 * Start the fan by triggering  the switch
 */
void powerOnFan(){
  digitalWrite(fan_switch_port, LOW);
  is_fan_running = true;
}

/*
 * Stop the fan by triggering  the switch
 */
void powerOffFan(){
  digitalWrite(fan_switch_port, HIGH);
  is_fan_running = false;
}

void wait(){
  delay(delay_between_loops_in_ms); // wait for the next read
}

