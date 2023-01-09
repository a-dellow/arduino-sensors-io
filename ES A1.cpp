#include <LiquidCrystal.h> //LiquidCrystal library allows use of LCD
LiquidCrystal lcd(11, 10, 6, 5, 4, 3); //assigning pins on Uno for LCD

#define LM335_pin  1 //defining temperature sensor's pin
#define trigPin 13
#define echoPin 12 //defining distance sensor's trigger and echo pin locations
#define intPin 2 //defining the pin for the hardware interrupt

volatile long duration, distance; //defining two variables for the ultrasonic sensor, stored in RAM so that they can be used with the interrupt
volatile int counter = 3; //initialising a variable for the interrupt's cycle

void setup() {

    lcd.begin(16, 2); //defining the size of the LCD
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT); //assigning pin mode to distance sensor's trigger and echo
}

void loop() {


    attachInterrupt(0, Interrupt, LOW); //hardware interrupt (ID 0) occurs when a CHANGE occurs in the button's state

    if (counter % 3 == 0) {
        DistSens();
    }

    if (counter % 3 == 1) {
        PhotoRes();
    }

    if (counter % 3 == 2) {
        TempSens(); //if statements decide which function to run using the modulo operator
    }

}

void Interrupt() {

    counter++; //1 added to counter every time the button is pressed

}

void DistSens() { //function for using the ultrasonic distance sensor

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW); //sends a 10 second HIGH burst out of the trigger sensor (LOW, HIGH, LOW)

    duration = pulseIn(echoPin, HIGH); //function detects the HIGH OUTPUT at pin 12 and assigns the value to the variable duration
    distance = (duration / 2) / 29.1; //distance in cm is calculated with this line of code, assigned to distance variable

    lcd.setCursor(0, 0); //positioning LCD cursor
    lcd.print("Distance:       "); // Prints string "Distance" on the LCD

    lcd.setCursor(0, 1);
    lcd.print("________________"); //print message

    if (distance >= 200 || distance <= 0) {
        lcd.setCursor(0, 1);
        lcd.print("Out of Range!   "); //statement is executed if object is out of sensor's range, set at 200cm
    }

    else {
        lcd.setCursor(0, 1);
        lcd.print(distance);
        lcd.print(" cm"); // Prints the distance value from the sensor
    }

    delay(500);

}

void PhotoRes() { //function for using the photoresistor

    lcd.setCursor(0, 0); //positioning LCD cursor
    lcd.print("Light Level:    "); //print message

    int sensorValue = analogRead(A0); //assigning photoresistor output to a pin and reading value

    lcd.setCursor(0, 1);
    lcd.print("________________");

    lcd.setCursor(0, 1);
    lcd.print(sensorValue);

    delay(260);

}

void TempSens() { //function for using the temperature sensor

    int Kelvin, Celsius; //defining two variables

    delay(100);
    Kelvin = analogRead(LM335_pin) * 0.489; //formula to calculate the Kelvin value from the LM335 produced voltage
    Celsius = Kelvin - 273; //Kelvin value - 273 = Celsius value

    lcd.setCursor(0, 0);
    lcd.print("Temp/K:______");
    lcd.print(Kelvin); //prints Kelvin value to LCD

    lcd.setCursor(0, 1);
    lcd.print("Temp/C:_______");
    lcd.print(Celsius); //prints Celsius value to LCD

}
