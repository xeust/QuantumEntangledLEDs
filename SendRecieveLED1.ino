/*
This a program utilizing Spark.publish() and Spark.subscribe() to have pushbutton toggle LEDs communicate between two Spark Cores,
Core1 and Core2. When the local pushbutton is pressed on Core1, "sendLed" on Core1 will toggle state, while "receiveLed" on Core2 will follow
this state, and vice-versa. This is the code for Core1.
*/


int sendLedPin = D2; //choose the pin for the send (local) LED
int inputPin = D0; //choose the input pin (for a pushbutton)
int receiveLedPin = D1; //choose the pin for the receive (Core2-dependent LED)
int val = 0; //variable for reading the pushbutton pin status
int sendLedVal = 0; //variable for keeping the state of the local pushbutton dependent LED (D2)
void ledTwoToggle(const char *toggle, const char *onOff); //handler function for Spark.subscribe()

void setup() {
    pinMode(receiveLedPin, OUTPUT); //Set the pin controlling the Core2-dependent LED as an OUTPUT
    pinMode(sendLedPin, OUTPUT); //Set the pin controlling the local pushbutton-dependent LED as an OUTPUT
    pinMode(inputPin, INPUT); //Set the pin connected to the pushbutton as an INPUT
    digitalWrite(sendLedPin, LOW); //Ensure the local LED is set to off to begin with
    Spark.publish("Core1Toggle", "State", 0, PRIVATE); //Set up Spark.publish() so that the state of the local LED is published to the Spark Cloud PRIVATELY
    Spark.subscribe("Core2Toggle", ledTwoToggle, MY_DEVICES); //Set up Spark.subscribe() so that the state of Core1's Led is recorded and handled by ledTwoToggle
}

void loop() {
    val = digitalRead(inputPin); //read value of pushbutton
    
    if (val == LOW) { //if clause activated if pushbutton pressed and thus inputPin reads LOW
        sendLedVal = !sendLedVal; //if clause activated if pushbutton pressed and thus inputPin reads LOW
        digitalWrite(sendLedPin, sendLedVal ? HIGH : LOW); //write the appropriate HIGH/LOW to the sendLed pin to turn it ON/OFF
        Spark.publish("Core1Toggle", sendLedVal ? "ON" : "OFF"); //publish the state to the Spark Cloud as ON/OFF
        delay(250); //primitive button debouncing
    }
}

void ledTwoToggle(const char *toggle, const char *onOff){ //handler function for Spark.subscribe()
    if (strcmp(onOff, "ON") == 0){ //if sendLed on Core2 is ON according to Spark.publish()
        digitalWrite(receiveLedPin, HIGH); //then turn on receiveLed
    } else if (strcmp(onOff, "OFF") == 0){ //if sendLed on Core2 is OFF according to Spark.publish()
        digitalWrite(receiveLedPin, LOW); //then turn off receiveLed
    }
}