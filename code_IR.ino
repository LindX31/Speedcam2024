// Arduino Code to measure distance with a Sharp GP2D12 sensor
// www.swanrobotics.com

int IR_SENSOR = 0; // Sensor is connected to the analog A0
int intSensorResult = 0; //Sensor result
float fltSensorCalc = 0; //Calculated value

void setup()
{
    Serial.begin(9600); // Setup communication with computer to present results serial monitor
}

void loop()
{
// read the value from the ir sensor

    intSensorResult = analogRead(IR_SENSOR); //Get sensor value
    fltSensorCalc = (6787.0 / (intSensorResult - 3.0)) - 4.0; //Calculate distance in cm

    Serial.print(fltSensorCalc); //Send distance to computer
    Serial.println(" cm"); //Add cm to result
    delay(200); //Wait
}
