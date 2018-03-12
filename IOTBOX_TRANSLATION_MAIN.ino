//Declerations of global variable
int rawTemperature = 0;	//storage of raw analog value
float realTemperature = 0.0;

int rawMoisture = 0;
float realMoisture = 0.0;

//Decleration of pins
int temperaturePin = A0;
int moisturePin = A1;


//Uc setup
void setup()
{
	pinMode(temperaturePin, INPUT);
	pinMode(moisturePin, INPUT);
}

//Uc loop
void loop()
{
	rawTemperature = analogRead(temperaturePin);
	rawMoisture = analogRead(moisturePin);
}