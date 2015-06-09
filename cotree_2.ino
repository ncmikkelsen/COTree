// Wither pin
int wPin = 2;
//Stretch pin
int sPin = 3;
//CO2 sensor pin
int cPin = 4;

//Current value
int cVal = 0;
//Threshold value
int tVal = 0;
//The value moving the threshold
int mVal = 0;

//Average value
int aVal = 0;
//List of values
int values[5];
//Pointing at the next value to be updated
int valuesPointer = 0;


//Last Time
int lTime = 0;
//Delay value in seconds
int dTime = 30;

void setup() {
	//Setting pinmode on the wither and stretch pins
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);

	//Populating the array to avoid waiting 5 * dTime seconds for true averages
	for(int i=0; i<5; i++){
	    values[i] = analogRead(cPin);
	}

	//Adjusting the last time variable so the loop will run at start of code instead of waiting 'dTime' seconds
	lTime = millis() - (dTime * 1000);
}

void loop() {
	//Checking if enough time has passed
	if(millis() - lTime > (dTime * 1000)){
		updateVals();

		//If the current value is equal to or larger than the threshold value the wither/stretch sequence will be triggered.
		//Else the threshold value will be lowered according to mVal to increase the likelyhood of a reaction from COTree
		if(cVal >= tVal){
			wither();
			tVal = cVal;
		} else{
			tVal -= mVal;
		}

	//Updating the timer variable
  	lTime = millis();
  }
}



//Makes the leaves wither for t seconds
void wither(int t){
	//Simple turn on then off once
	digitalWrite(wPin, HIGH);
	delay(t * 1000);
	digitalWrite(wPin, LOW);
}


//Makes the leaves stretch for t seconds
void stretch(int t){
	//Simple turn on then off once
	digitalWrite(sPin, HIGH);
	delay(t * 1000);
	digitalWrite(sPin, LOW);
}




//TODO: Create a for-loop version of wither that will wither over a longer period of time
void witherPulse(){

}

//TODO: Create a for-loop version that will wither over a longer period of time
void stretchPulse(){

}

void updateVals(){
	//Updating the current value
	cVal = analogRead(cPin);

	//Inserting the current value into the array of values
	values[valuesPointer] = cVal;
	values = (values + 1) % 5;

	//Calculating and updating the average value
	aVal = 0;
	for(int i=0; i<5; i++){
	    aVal = values[i];
	}
	aVal /= 5;
}
