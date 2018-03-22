#include <TheThingsNetwork.h>
#include <DHT.h>

//--ttn keys
const char *devAddr = "26011834";
const char *nwkSKey = "68812A2DF5C6E54EF18E197B5136C9DB";
const char *appSKey = "F2F66085A51D09A5A86E74A9AF9B8D68";

#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868
#define DHTPIN 2
#define DHTTYPE DHT11

//--Pin defines
const int buttonPin = 8;

//--Global variables
int buttonState = 0;
long previousMillis = 0;  //previous value of the millis function
long sendInterval = 10;   //interval of ttn transmission, in seconds

DHT dht(DHTPIN, DHTTYPE);
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{

  //--serial setup
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  debugSetup();

  //ttn setup
  ttn.personalize(devAddr, nwkSKey, appSKey);
  ttn.showStatus();
  dht.begin();

  //--PinMode setup
  pinMode(buttonPin, INPUT);
}

void loop()
{
  //debugSerial.println("-- LOOP");

  int buttonState = digitalRead(buttonPin);

  //-- Read sensor values and multiply by 100 to effictively have 2 decimals
  //-- No decimal value available for the humidity model we're using.
  uint16_t humidity = (dht.readHumidity(false)) * 100;

  //-- false: Celsius (default)
  //-- true: Farenheit
  uint16_t temperature = (dht.readTemperature(false)) * 100;

  //interval past? send payload.
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > sendInterval *1000)
  {
    previousMillis = currentMillis;
    sendPayload(temperature, humidity, buttonState);    //--generate and send payload
  }

  //--Print values to serial monitor, only for debugging
  //debugPrint(temperature, humidity, buttonState);

}

void sendPayload(int temp, uint16_t hum, int button)
{
  byte payload[5];
  payload[0] = highByte(temp);
    payload[1] = lowByte(temp);
    payload[2] = highByte(hum);
    payload[3] = lowByte(hum);
    payload[4] = lowByte(button);

    ttn.sendBytes(payload, sizeof(payload));
}


//--Wait 10 seconds for the serial to initialize. 
//--Send info to serial monitor
void debugSetup()
{
  while (!debugSerial && millis() < 10000);
    debugSerial.println("-- PERSONALIZE");
    debugSerial.println("-- STATUS");
}

//--Print the temperature humidity and button state to the serial monitor
void debugPrint(uint16_t temperature, uint16_t humidity, int buttonState)
{
  debugSerial.print("Temperature: ");
    debugSerial.println(temperature);
    debugSerial.print("Humidity: ");
    debugSerial.println(humidity);
    debugSerial.print("ButtonState: ");
    debugSerial.println(buttonState);
}