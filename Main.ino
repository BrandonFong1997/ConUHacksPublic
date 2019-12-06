int avgX, avgY, avgZ;
int lowExtrX, highExtrX, lowExtrY, highExtrY, lowExtrZ, highExtrZ;
const int SAMPLES = 50;

void getNeutral() {
  int sumX = 0, sumY = 0, sumZ = 0;

  for (int i=0; i<SAMPLES; i++) {

    sumX += analogRead(A0);
    Serial.print(analogRead(A0));
    Serial.print("    ");
    
    sumY += analogRead(A1);
    Serial.print(analogRead(A1));
    Serial.print("    ");
    
    sumZ += analogRead(A2);
    Serial.println(analogRead(A2));
    
    delay(10);
  }

  avgX = sumX/SAMPLES;
  avgY = sumY/SAMPLES;
  avgZ = sumZ/SAMPLES; 
}

void printAvg() {
  Serial.print("avgX = ");
  Serial.print(avgX);

  Serial.print("    avgY = ");
  Serial.print(avgY);

  Serial.print("    avgZ = ");
  Serial.println(avgZ);
}

void getExtremes() { 
  
  int lowestSFx = 0;
  int highestSFx = 0;
  int lowestSFy = 0;
  int highestSFy = 0;
  int lowestSFz = 0;
  int highestSFz = 0;
  
  for (int i=0; i<SAMPLES*2; i++) {

    int temp1 = analogRead(A0) - avgX;
    if (temp1 > highestSFx) {
      highestSFx = temp1;
    }
    else if (temp1 < lowestSFx) {
      lowestSFx = temp1;
    }
    Serial.print(temp1);
    Serial.print("    ");
    /////////
    int temp2 = analogRead(A1) - avgY;
    if (temp2 > highestSFy) {
      highestSFy = temp2;
    }
    else if (temp2 < lowestSFy) {
      lowestSFy = temp2;
    }
    Serial.print(temp2);
    Serial.print("    ");
    ////////
    int temp3 = analogRead(A2) - avgZ;
    if (temp3 > highestSFz) {
      highestSFz = temp3;
    }
    else if (temp3 < lowestSFz) {
      lowestSFz = temp3;
    }
    Serial.println(temp3);
    
    delay(5);
  }
  
  lowExtrX = lowestSFx;
  Serial.print("Lowest deviation of x: ");
  Serial.print(lowExtrX);
  
  highExtrX = highestSFx;
  Serial.print("  Highest deviation of x: ");
  Serial.println(highExtrX);
  
  lowExtrY = lowestSFy;
  Serial.print("Lowest deviation of y: ");
  Serial.print(lowExtrY);
  
  highExtrY = highestSFy;
  Serial.print("  Highest deviation of y: ");
  Serial.println(highExtrY);
  
  lowExtrZ = lowestSFz;
  Serial.print("Lowest deviation of z: ");
  Serial.print(lowExtrZ);
  
  highExtrZ = highestSFz;
  Serial.print("  Highest deviation of z: ");
  Serial.println(highExtrZ);
  
}

int getAcc() {
  int a1 = analogRead(A0) - avgX;
  //Serial.print(a1); Serial.print("      ");
  int a = pow(a1, 2);

  int b1 = analogRead(A1) - avgY;
  int b = pow(b1, 2);

  int c1 = analogRead(A2) - avgZ;
  int c = pow(c1, 2);
  //Serial.print(a); Serial.print("         ");
  //Serial.print(b); Serial.print("         ");
  //Serial.print(c); Serial.print("         ");
  int value = sqrt(a + b + c);
  //Serial.println(value);

  return value;
  //return sqrt(pow((analogRead(A0) - avgX),2) + pow((analogRead(A1) - avgY),2) + pow((analogRead(A2) - avgZ),2));
}

int highPass() { //NOT NECESSARY FOR ACCELEROMETER
  int returnValue = 0;
  for (int i = 0; i < 10; i++) {
    returnValue += getAcc();
    delay(1);
  }
  return returnValue /= 10;
}

bool fallDetect() {
  int vo = 20;
  bool falling = true;
  bool fallen = false;
  for (int i = 0; i<3 & falling == true; i++) {
    int vi = getAcc();
    if (vo < vi || abs(vo-vi) < 5 || vi > 50)
      vo = vi;
    else
      falling = false;
    delay(50);
  }

  if (falling == true) {
    for (int i = 0; i < 50; i++) {
      int temp = getAcc();
      if (temp > 100) {
        fallen = true;
        break;
      }
      
    }
  }

  return fallen;
}

void fallenProtocol() {
  digitalWrite(12, HIGH);
  Serial.println("FALLEN");
  //system("/var/HelpMe/alert.sh"); //JOEL'S SCRIPT
  //enter button mode, measure vitals

  for (int i = 0; i<4; i++){
  delay(500);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  
  getNeutral();  
  printAvg();
  delay(300);
  digitalWrite(12, LOW); 
}

void loop() { //run fall detect and vital tracking at the same time
  
  Serial.println(getAcc());
  /* 
  
  Serial.print(avgX);
  Serial.print("  ");
  Serial.print(avgY);
  Serial.print("  ");
  Serial.println(avgZ);
*/
  
  bool call = fallDetect();

  delay(5);
  
  if (call == true) {
    fallenProtocol();
  }
  
  delay(95);       
}


