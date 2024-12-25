#define speedPinR 5    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  7    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  8    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  9    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  10   //Left Motor direction pin 1 to MODEL-X IN4 


/*motor control*/
void go_Advance(void)  //Forward
{
  digitalWrite(RightMotorDirPin1,LOW);
  digitalWrite(RightMotorDirPin2,HIGH);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,HIGH);
  // analogWrite(speedPinL,100);
  // analogWrite(speedPinR,100);
  set_Motorspeed(140, 140);
}

void go_Left(int t=0)  //Turn left
{
  digitalWrite(RightMotorDirPin1,LOW);
  digitalWrite(RightMotorDirPin2,HIGH);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  analogWrite(speedPinL,200);
  analogWrite(speedPinR,200);
  delay(t);
}
void go_Right(int t=0)  //Turn right
{
  digitalWrite(RightMotorDirPin1,HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,HIGH);
  analogWrite(speedPinL,255);
  analogWrite(speedPinR,255);
  delay(t);
}
void go_Back(int t=0)  //Reverse
{
  digitalWrite(RightMotorDirPin1,HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  set_Motorspeed(140, 140);
  delay(t);
}
void stop_Stop()    //Stop
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,LOW);
}

void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL,speed_L); 
  analogWrite(speedPinR,speed_R);   
}

//Pins initialize
void init_GPIO()
{
  pinMode(RightMotorDirPin1, OUTPUT); 
  pinMode(RightMotorDirPin2, OUTPUT); 
  pinMode(speedPinL, OUTPUT);  
 
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT); 
  pinMode(speedPinR, OUTPUT); 
  stop_Stop();
}


// MY OWN METHODS
int forward(double cm) {
  int tempDelay = (cm+0.682)/0.0268;
  go_Advance();
  // delay((cm+0.682)/0.042);
  delay(tempDelay);
  stop_Stop();
  return tempDelay;
}

int backward(double cm) {
  int tempDelay = (cm+0.682)/0.0268;
  go_Back();
  // delay((cm+0.682)/0.042);
  delay(tempDelay);
  stop_Stop();
  return tempDelay;
}

int turn(int direction) {
  int tempDelay = 0;
  if (direction == 0) {
    tempDelay += forward(14.5);
    stop_Stop();
    delay(500);
    tempDelay += 500;
    go_Left();
    delay(327);
    tempDelay += 290;
    // delay(298); // half delay
  } else if (direction == 1) {
    tempDelay += forward(14.5);
    stop_Stop();
    delay(500);
    tempDelay += 500;
    go_Right();
    // delay(285); // half delay
    // delay(298);
    delay(339);
    tempDelay += 300;
  }

  stop_Stop();
  return tempDelay;
}



// math
double forwardTime(double cm) {
  int tempDelay = (cm+0.682)/0.036;
  // delay((cm+0.682)/0.042);
  return tempDelay;
}

double backwardTime(double cm) {
  int tempDelay = (cm+0.682)/0.036;
  return tempDelay;
}

double turnTime(int direction) {
  int tempDelay = 0;
  if (direction == 0) {
    tempDelay += forwardTime(14.5);
    tempDelay += 500;
    tempDelay += 290;
    // delay(298); // half delay
  } else if (direction == 1) {
    tempDelay += forwardTime(14.5);
    tempDelay += 500;
    tempDelay += 300;
  }
  return tempDelay;
}




void flip() {
  go_Left();
  delay(301);
  stop_Stop();
  go_Left();
  delay(301);
  stop_Stop();
  delay(500);
}

void setup() {
  Serial.begin(9600);
  init_GPIO();

  double delayAccum = 2700;
  delay(2700);



  int numInstructions = 14
  ;
  // 1-forward, 2-backward, 3-turn
  // int instructions[] = {1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 3, 1, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  // double arguments[] = {25, 50, 50, 50, 50, 50, 0, 35.5, 50, 50, 0, 35.5, 0, 1, 0, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};

  int instructions[] = {1,3};
  double arguments[] = {25, 0};
  

  for (int i = 0; i < numInstructions; i++) {
    switch(instructions[i]) {
      case 1:
        delayAccum += forwardTime(arguments[i]);
        break;
      case 2:
        delayAccum += backwardTime(arguments[i]);
        break;
      case 3:
        delayAccum += turnTime(arguments[i]);
        break;
    }
  }


  double timeLeft = 9000 - delayAccum;
  double delayExtraSegment = timeLeft / numInstructions;
  

  for (int i = 0; i < numInstructions; i++) {
    switch(instructions[i]) {
      case 1:
        forward(arguments[i]);
        break;
      case 2:
        backward(arguments[i]);
        break;
      case 3:
        turn(arguments[i]);
        break;
    }
    delay(delayExtraSegment);
  }


  // forward(25);
  // forward(50);
  // forward(50);
  // forward(50);
  // backward(50);
  // backward(50);
  // turn(0);
  // forward(35.5);
  // forward(50);
  // forward(50);
  // turn(0);
  // forward(35.5);
  // turn(0);
  // turn(1);
  // turn(0);



  //  14.5 cm less each turn

  stop_Stop();//Stop

}

void loop(){
}
