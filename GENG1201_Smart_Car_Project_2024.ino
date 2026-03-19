#include "GENG1201_Project.h"

int stage = 1;
Servo servo1;
Servo servo2;
int loopCounter;
int turnCount;
bool stage3Completed = false;
unsigned long stage4StartTime = 0;


void setup() {
  Init();   // Initialize Robot Car
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(1, OUTPUT);
  servo1.attach(10);
  servo2.attach(11);




}
void loop() {
 
  TrackingData_R = infraRed_R();
  TrackingData_M = infraRed_M();
  TrackingData_L = infraRed_L();

// LINE FOLLOWER 
  if (stage == 1) {
  myColor(255,0,0, 50);
  if (function_xxx(TrackingData_M, 0 , 100)) {
    move(Forward, 150); }
   else if (function_xxx(TrackingData_L, 0 , 100)) {
    move(Left, 100); }
    else if (function_xxx(TrackingData_R, 0 , 100)) {
     move(Right, 100); }
  else {
    stage = 2;
  }
  }







// OBSTACLE AVOIDANCE
if (stage == 2) {
    myColor(0,255,0, 50);


    readUltrasonicSensor (get_Distance);
    delay(200);
    if(get_Distance > 15){
    move(0, 80);//move forward
    delay(1000);
    readUltrasonicSensor (get_Distance);
    int new_Distance = get_Distance;
      if(function_xxx(new_Distance, 0, 30)) {//look for obstacles within a 20 cm range of the robot stops moves back and turns left
      move(8,100);//Robot stops
      delay(100);
      servo1.write(180);//servocontrol truns left
      delay(1000);
      readUltrasonicSensor (get_Distance);
      int next_distance = get_Distance;
      servo1.write(90);
      if(next_distance > 30){
        move(2,100); //robot car turns left if this is nothing infornt of it when it looks left
        delay(550);
        move(8,0);
        turnCount += 1;
      }
        if(function_xxx(next_distance, 0, 30)) {//if there is still an object within 30cm the robot turns right
        move(8, 100);//robot stops
        delay(100);
        servo1.write(0);//robot car looks right
        delay(1000);
        readUltrasonicSensor (get_Distance);
        int final_distance = get_Distance;
        servo1.write(90);
        delay(1000);
        if(final_distance > 30){
          move(3, 100);//turns right
          delay(550);
          move(8,0);
          turnCount += 1;




        }
       }
    }
    }

// Goes to stage 3 if stage 3 has not been done yet, sensors see white, and the car has turned once.
    if(!stage3Completed && TrackingData_M < 100 && turnCount == 1) { 
      delay(500);
      stage = 3;
      }

// Goes into a post turn delay after the second turn to ensure the car doesnt see the white mat and mistake it for a line.
    if (turnCount >= 2) {
      stage = 8;
    }
   
   
  }



// FERTILIZER DROPPER
if (stage == 3) {
  myColor(0,0,255,50);
  move(Forward, 30);
  for (int i = 0; i <= 120; i += 10 ) {
    servo2.write(i);
    delay(100);
    }
    loopCounter += 1;
    if (loopCounter >= 5) {
      stage = 2;
      stage3Completed = true;
      loopCounter = 0;
    }
  }







// LINE FOLLOWER 2
if (stage == 4) {
  myColor(255,255,0,50);

  if (stage4StartTime == 0) {
        stage4StartTime = millis();  // Start timing when stage 4 starts
    }

  if (function_xxx(TrackingData_M, 0 , 100)) {
    move(Forward, 90); }
  else if (function_xxx(TrackingData_L, 0 , 100)) {
    move(Left, 90); }
  else if (function_xxx(TrackingData_R, 0 , 100)) {
     move(Right, 90); }
  else if (millis() - stage4StartTime > 2000) {  // Ensure robot stays in stage 4 for 2 seconds before switching
        move(stop_it, 0);
        myColor(0, 0, 0, 0);
        stage = 5;
        stage4StartTime = 0;  // Reset timer
    }
  }

// FINDING BOOTH
 if (stage == 5) {

    servo1.write(0); // looks right
    delay(700);
    readUltrasonicSensor(get_Distance);
    int rightd = get_Distance;
    servo1.write(180); // looks left
    delay(700);
    readUltrasonicSensor(get_Distance);
    int leftd = get_Distance;
    delay(200);

// DECIDES WHICH DIRECTION IS CLOSER
    if (leftd > rightd) {
      servo1.write(0);
    }
    else {
      servo1.write(180);
  }
  delay(500);
  stage = 6;
 }



// IR REMOTE 
if (stage == 6) {
  while(ReadRemote() != 0) {
    myColor(0,0,255,100);
    delay(500);
    myColor(0,0,0,100);
    delay(500);
  }
  if(ReadRemote() == 0) {
    stage = 7;
  }
  }
 


// MOVES CAR FORWARD AFTER GETTING IR SIGNAL
if (stage == 7) {
  servo1.write(90);
  move(Forward, 70);
  delay(250);
  if (TrackingData_M < 100 || TrackingData_L < 100 || TrackingData_R < 100) {
    stage = 9;
}
  }




// POST TURN DELAY
if (stage == 8) {
  move(Forward, 90);
  delay(1500);
  if (TrackingData_M > 100 && TrackingData_L > 100 && TrackingData_R > 100) {
      move(Forward, 90);
  }
  else {
    stage = 4;
  }
}



// FINAL LINE FOLLOWER 
if (stage == 9) {
  myColor(255,255,0,50);

  if (function_xxx(TrackingData_M, 0 , 100)) {
    move(Forward, 90); }
  else if (function_xxx(TrackingData_L, 0 , 100)) {
    move(Left, 90); }
  else if (function_xxx(TrackingData_R, 0 , 100)) {
     move(Right, 90); }
  else {
    move(stop_it, 0);
    myColor(0,0,0,0);
  }
}

} //void loop end








