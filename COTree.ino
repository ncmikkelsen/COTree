/*
  Rød= 5V
  gul=12V
  orange=3.3v (så vidt jeg husker)
  sort=jord/nul
*/

// Wither pin
int wPin = 3;
//Stretch pin
int sPin = 2;
//CO2 sensor pin
int cPin = 4;

//Current value
int cVal = 0;
//Threshold value
int tVal = 0;
//The value moving the threshold
//Lower this to increase the time between wither and stretch cycles. Increase it for the opposite effect.
int mVal = 10;

//Average value
float aVal = 0;
//List of values
int values[5];
//Pointing at the next value to be updated
int valuesPointer = 0;


//Last Time
unsigned long lTime = 0;
//Delay value in seconds
long dTime = 5;

void setup() {
	//Setting pinmode on the wither and stretch pins
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);

	//Populating the array to avoid waiting 5 * dTime seconds for true averages
	for(int i=0; i<5; i++){
	    values[i] = analogRead(cPin);
	}

	//Adjusting the last time variable so the loop will run at start of code instead of waiting 'dTime' seconds
	//lTime = millis() - (dTime * 1000);

	//For testing purposes
	Serial.begin(9600);
}

void loop() {
	//Checking if enough time has passed
	if(millis() - lTime > (dTime * 1000)){
		updateVals();
		printVals();
		//If the current value is equal to or larger than the threshold value the wither/stretch sequence will be triggered.
		//Else the threshold value will be lowered according to mVal to increase the likelyhood of a reaction from COTree
		
		if(cVal >= tVal){
			simpleCycle(10, 4, 6, 12);
			//tVal = cVal;
                        
                        tVal = cVal + ((20/dTime)*mVal);
		} else{
			tVal -= mVal;
		}
		


		checkSerial();

	//Updating the timer variable
  	lTime = millis();
  }
}



// ----- Simple movement methods begins here ----- //

//Makes the leaves wither for wT seconds, then after a delay of dT seconds it will stretch the leaves for sT seconds. 
//After that it will cool of for cT seconds before returning to normal behaviour. 
void simpleCycle(int wT, int dT, int sT, int cT){
        Serial.println("***** WITHER *****");
        Serial.println();
	wither(wT);
        Serial.println("***** DELAY *****");
        Serial.println();
	delay(dT * 1000);

        Serial.println("***** STRETCH THEN DELAY *****");  
        Serial.println();
	stretch(sT, cT);
}


//Makes the leaves wither for t seconds
void wither(int t){
	//Simple turn on then off once
	digitalWrite(wPin, HIGH);
	delay(t * 1000);
	digitalWrite(wPin, LOW);
}


//Makes the leaves stretch for t seconds
void stretch(int t, int c){
	//Simple turn on then off once
	digitalWrite(sPin, HIGH);
	delay(t * 1000);
	digitalWrite(sPin, LOW);
	delay(c * 1000);
}

// ----- Simple movement methods ends here ----- //




// ----- Pulse movement methods begins here ----- //


void pulseCycle(){

}

//TODO: Create a for-loop version of wither that will wither over a longer period of time
void witherPulse(){

}

//TODO: Create a for-loop version that will wither over a longer period of time
void stretchPulse(){

}

// ----- Pulse movement methods ends here ----- //


//Updates all the values
void updateVals(){
	//Updating the current value
	cVal = analogRead(cPin);

	//Inserting the current value into the array of values
	values[valuesPointer] = cVal;
        valuesPointer++;
	valuesPointer %= 5;

	//Calculating and updating the average value
	aVal = 0;
	for(int i=0; i<5; i++){
	    aVal += values[i];
	}
	aVal /= 5;
}

//Prints all the value. Use for testing/debugging
void printVals(){
	Serial.println("----------");
	Serial.println("Printing all values:");
	Serial.println();

	Serial.print("Current value:");
	Serial.println(cVal);
	Serial.println();

	Serial.print("Threshold value:");
	Serial.println(tVal);
	Serial.print("Offset value:");
	Serial.println(mVal);
	Serial.println();
	
	Serial.print("Last 5 values:");
        for(int i = 0; i<5; i++){
          Serial.print(values[i]);
          Serial.print("     ");
        }
        Serial.println();
	Serial.print("The average value:");
	Serial.println(aVal);

	Serial.println();
	Serial.println("Done printing values");
	Serial.println("----------");
	Serial.println();
}

//For testing purposes
void checkSerial(){
	char c;
	int wT = 10;
	int dT = 4;
	int sT = 6;
	int cT = 12;

	if(Serial.available()){
		c = Serial.read();
		if(c == 'w'){
			wither(wT);
		} else if(c == 's'){
			stretch(sT, cT);
		} else if(c == 'c'){
			simpleCycle(wT, dT, sT, cT);
		}
		while(Serial.available()){
			Serial.read();
		}
	}
}
