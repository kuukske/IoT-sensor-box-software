//--Include libraries
#include <TheThingsNetwork.h>  //Needed for LORA module
#include <DHT.h>        //Needed for digital temperature sensor

//--TTN keys
const char *devAddr = "26011834";             //LORA module device address
const char *nwkSKey = "68812A2DF5C6E54EF18E197B5136C9DB"; //TTN network key
const char *appSKey = "F2F66085A51D09A5A86E74A9AF9B8D68"; //TTN application key

//--Defines
#define loraSerial Serial1    //Serial connection to LORA module
#define debugSerial Serial    //Serial connection for debugging
#define freqPlan TTN_FP_EU868 //European frequency plan for LORA module
#define DHTTYPE DHT11     //DHT sensor type
//#define DHTPIN 2

//--Pinnumber definitions
//-Input
const int DHTPIN = 2;   //Digital input DHT sensor pin (NOTE: possibly needs to be in defines)
const int NTCpin = A0;    //Analogue input NTC sensor pin
const int buttonPin = 8;  //Button input pin
const int shockPin = 4;   //Shock sensor input pin
//-Output
const int redPin = 13;    //RGB red output pin
const int greenPin = 11;  //rGB green output pin

//--Global variables
int buttonState = 0;    //Global button state variable
int shockState = 0;     //Global shock state variable
unsigned long previousMillis = 0;   //Previous millis value for blockless delay
unsigned long currentMillis = 0;    //Current millis value for blockless delay
long sendInterval = 10; //Interval for TTN transmission, in seconds
int ledMode = 0;      //Trafficlight mode (1. Red, 2. Orange, 3. Green)
int ledSecondCount = 0;   //Global led second count (always n*sendInterval)

//--Initialisation of objects
DHT dht(DHTPIN, DHTTYPE);                 //Make DHT temperature/humidity object
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);    //Make TTN object

//--Setup
void setup()
{
  //--Serial setup
  loraSerial.begin(57600);  //Serial communication with LORA module begin 
  debugSerial.begin(9600);  //Serial communication for debugging begin
  debugSetup();       //Serial communicaiton for debugging setup

  //--TTN communicaiton setup
  ttn.personalize(devAddr, nwkSKey, appSKey);   //Add all required TTN keys and information
  chan0Only();
  ttn.showStatus();               //Print status of TTN network conneciton
  dht.begin();                  //Begin the DHT humidity/temperature sensor

  //--pinMode setup
  //-Input
  pinMode(NTCpin, INPUT);     //Set NTCpin as input
  pinMode(buttonPin, INPUT);    //set buttonPin as input
  pinMode(shockPin, INPUT);   //set shockPin as input
  //-Output
  pinMode(redPin, OUTPUT);    //set redPin as output
  pinMode(greenPin, OUTPUT);    //set greenPin as output
}

void loop()
{
  if(buttonState == 0)            //if button is not pressed after last transmission..
    buttonState = digitalRead(buttonPin); //check button

  if(shockState == 0)             //if button is not pressed after last transmission..
    shockState = digitalRead(shockPin);   //check shock sensor
  
  /*
  Both humidity and temperature measurements are multiplied by 100 to effecitively hae two decimals.
  The integer number is divided by 100 in the nodeRED appication. 
  NOTE: The DHT11 sensor that we are using doesn't allow decimal values.
  */
  uint16_t humidity = dht.readHumidity(false) * 100;      //Get humidity measurement
  uint16_t temperature = dht.readTemperature(false) *100;    //Get temperature measurement, false = Celcius
  uint16_t NTCtemperature = thermistor(analogRead(NTCpin)); //Get analogue temperature reading from NTC pin

  currentMillis = millis();     //Get the current millis from the millis function

  if(currentMillis - previousMillis > sendInterval * 1000)   //If the difference between current and previous millis > sendInterval:
  {
    previousMillis = currentMillis;   //Make previousMillis currentMillis
    sendPayload(temperature, humidity, buttonState, NTCtemperature, ledMode, shockState);  //Send complete payload

    buttonState = 0;    //Reset buttonState to 0
    shockState = 0;     //Reset shockState to 0

    if(ledSecondCount >= 1)     //If ledSecondCount >= 1 (so 2x sendInterval)   
    {
      ledSecondCount = 0;     //Reset ledSecondCount to 0
      trafficLightSim(ledMode); //Run traficLightSimulation
      ledMode++;          //Increment ledMode for next simulation
      if(ledMode >= 4)      //If 3 led modes are done, start over
        ledMode = 1;
    }

    ledSecondCount++;       //Increment ledSecondCount for next sendInterval
  }

}

void debugSetup()               //Waits for the debugSerial to be ready and prints status info
{
  while(!debugSerial && millis() < 10000);  //Wait for debugSerial to be ready, if not, time out after 10 seconds
    debugSerial.println("-- Personalize");  //Print "-- Personalize"
    debugSerial.println("--  STATUS");    //Print "-- STATUS"
}

/*
Print the current values of temperature, humidity, buttton state, NTC and shock sensor in the serial monitor.
*/
void debugPrint(uint16_t temperature, uint16_t humidity, int buttonState, uint16_t NTC, int shock)
{
  debugSerial.print("Temperature: ");
    debugSerial.println(temperature);
    debugSerial.print("Humidity: ");
    debugSerial.println(humidity);
    debugSerial.print("ButtonState: ");
    debugSerial.println(buttonState);
    debugSerial.print("NTC: ");
    debugSerial.println(NTC);
    debugSerial.print("Shock: ");
    debugSerial.println(shock);
}

/*
Takes the analogue read of the NTC pin and returns a double with the current analogue temperature after calculation.
*/
double thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 275.15;            // Convert Kelvin to Celcius
   //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
   return Temp *100;
}

void redLed()         //Make led red and change ledMode
{
  digitalWrite(redPin,HIGH);  //Turn the red LED on
  digitalWrite(greenPin, LOW);  //Turn the geen LED off
  ledMode = 1;
}

void orangeLed()        //Make led orange and change ledMode
{
  digitalWrite(redPin, HIGH); //Turn the red led on
  digitalWrite(greenPin, HIGH); //Turn the green led on
  ledMode = 2;
}

void greenLed()         //Make led green and change ledMode       
{
  digitalWrite(redPin, LOW);  //Turn the red led off
  digitalWrite(greenPin, HIGH); //Turn the green led on
  ledMode = 3;
}

void offLed()         //Disable led and change ledmode
{
  digitalWrite(redPin,LOW);   //Turn red led off
  digitalWrite(greenPin, LOW);  //Turn green led off
  ledMode = 0;
}

/*
The sendPayload function is used to send all the aquired data to the TTN service using the LORA module
*/
void sendPayload(uint16_t temp, uint16_t hum, int button, uint16_t NTC, int ledMode, int shock)
{
    byte payload[9];
    payload[0] = highByte(temp);  //Send the most significant byte of the temperature
    payload[1] = lowByte(temp);   //Send the least significant byte of the temperature
    payload[2] = highByte(hum);   //Send the most significant byte of the humidity
    payload[3] = lowByte(hum);    //Send the least significant byte of the humidity
    payload[4] = lowByte(button); //Send the status of the button
    payload[5] = highByte(NTC);   //Send the most significant byte of the analogue temperature
    payload[6] = lowByte(NTC);    //Send the least significant byte of the analogue temperature
    payload[7] = lowByte(ledMode);  //Send the led mode
    payload[8] = lowByte(shock);  //Send the value of the shock sensors

    ttn.sendBytes(payload, sizeof(payload));  //Send the complete payload
}


//Only transmit on channel 0 !TEST AND DEBUG ONLY! 
void chan0Only()
{
  loraSerial.println("mac reset 868");
  delay(100);
  loraSerial.println("mac set ch status 0 off");
  delay(100);
  loraSerial.println("mac set ch status 1 off");
  delay(100);
  loraSerial.println("mac set ch status 2 off");
  delay(100);
  loraSerial.println("mac set ch freq 5 868100000");
  delay(100);
  loraSerial.println("mac set ch dcycle 5 9");
  delay(100);
  loraSerial.println("mac set ch drrange 5 0 5");
  delay(100);
  loraSerial.println("mac set ch status 5 on");
  delay(100);
  loraSerial.println("mac set devaddr 26011834");
  delay(100);
  loraSerial.println("mac set nwkskey 68812A2DF5C6E54EF18E197B5136C9DB");
  delay(100);
  loraSerial.println("mac set appskey F2F66085A51D09A5A86E74A9AF9B8D68");
  delay(100);
  loraSerial.println("mac set adr off");
  delay(100);
  loraSerial.println("mac set ar off");
  delay(100);
  loraSerial.println("mac set pwridx 1");
  delay(100);
  loraSerial.println("mac set dr 5");
  delay(100);
  loraSerial.println("mac join abp");
  delay(100);
}

/*
The trafficLightSim functions runs through all the light patterns of a traffic light
*/
void trafficLightSim(int ledMode)
{
   switch(ledMode){
        case 1:
          redLed();   //Enable red led, dissable green
        break;

        case 2:
          orangeLed();  //Enable both red and green led
        break;

        case 3:
          greenLed(); //Enable green led, dissable red
        break;
    }
}