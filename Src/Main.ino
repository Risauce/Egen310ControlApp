// Servo - Version: Latest

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

/*ARDUINO CODE FOR EGEN310 CONTROL APP  */
//this is arduino code... Took inspiration from: https://create.arduino.cc/projecthub/rafitc/android-arduino-rc-car-e633d7
/*------------------------------------------------------------------------------*/






//int led = 3;        //led
int outPin1 = 5;     //motor1
int outPin2 = 6;    //motor1
int outPin4 = 11;   //motor2
int outPin3 = 12;   //motor2

char btCmd = 0;     //btCmd value to store command.
int servoPos = 0;

uint8_t servonum = 0;

Servo servo1;
Servo servo2;

/*------------------------------------------------------------------------------*/
void setup()
{
  Serial.begin(9600);
  pinMode(outPin1, OUTPUT);
  pinMode(outPin2, OUTPUT);
  pinMode(outPin3, OUTPUT);
  pinMode(outPin4, OUTPUT);
  //pinMode(led, OUTPUT);

  //Attach the servos:
  servo1.attach(4);
  servo2.attach(5);




  pwm.begin();
  /*
     In theory the internal oscillator (clock) is 25MHz but it really isn't
     that precise. You can 'calibrate' this by tweaking this number until
     you get the PWM update frequency you're expecting!
     The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
     is used for calculating things like writeMicroseconds()
     Analog servos run at ~50 Hz updates, It is importaint to use an
     oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
     1) Attach the oscilloscope to one of the PWM signal pins and ground on
        the I2C PCA9685 chip you are setting the value for.
     2) Adjust setOscillatorFrequency() until the PWM update frequency is the
        expected value (50Hz for most ESCs)
     Setting the value here is specific to each individual I2C PCA9685 chip and
     affects the calculations for the PWM update frequency.
     Failure to correctly set the int.osc value will cause unexpected PWM results
  */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}

// Here is where all the magic happens. This loop() function runs constantly within the Uno, constantly taking commands from the bluetooth serial code. 
// The commands themselves are self-explanatory. For example; when the command for forward is recieved, it commands both motors to move the vehicle forward.
// L and R commands are different at the moment. They control the servos to turn the vehicle, but this will be changed. 
void loop()
{
  if (Serial.available() > 0) //As long as there is a connection to BT
  {
    btCmd = Serial.read(); //Read the current command,
    

    switch (btCmd) {
      case 'F': //Moving Forward ASCII: 70
        digitalWrite(outPin1, HIGH); //will turn to analogWrite(outPin1, speed) once speed is implemented.
        digitalWrite(outPin2, LOW);
        digitalWrite(outPin3, HIGH);
        digitalWrite(outPin4, LOW);


        break;
      case 'B': //Moving Backward ASCII: 66
        digitalWrite(outPin1, LOW);
        digitalWrite(outPin2, HIGH);
        digitalWrite(outPin3, LOW);
        digitalWrite(outPin4, HIGH);
        break;
      case 'S': //Stopping Car ASCII: 83
        digitalWrite(outPin1, LOW);
        digitalWrite(outPin2, LOW);
        digitalWrite(outPin3, LOW);
        digitalWrite(outPin4, LOW);
        break;
      case 'R'://Moving Right ASCII: 82
        digitalWrite(outPin1, HIGH);
        digitalWrite(outPin2, LOW);
        digitalWrite(outPin3, LOW);
        digitalWrite(outPin4, LOW);



        pwm.writeMicroseconds(servonum, 1000);  //The writeMicroseconds function writes to the pwm a certain arc to follow. 
        pwm.writeMicroseconds(1, 1000);

        //servo1.write(90);
        //servo2.write(90);
        break;
      case 'L': //Moving Left ASCII: 76
        digitalWrite(outPin1, LOW);
        digitalWrite(outPin2, LOW);
        digitalWrite(outPin3, HIGH);
        digitalWrite(outPin4, LOW);


        //for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        //  pwm.setPWM(servonum, 0, 4000);
        //}

        pwm.writeMicroseconds(servonum, 2000);
        pwm.writeMicroseconds(1, 2000);

        



        //servo1.write(-90); //The standard way to write to servos without PWM control
        //servo2.write(-90);
        break;
      case 'I': //Moving forward and Right
        digitalWrite(outPin1, HIGH);
        digitalWrite(outPin2, LOW);
        digitalWrite(outPin3, LOW);
        digitalWrite(outPin4, HIGH);
        break;
      case 'G': //Moving forward and Left
        digitalWrite(outPin1, LOW);
        digitalWrite(outPin2, HIGH);
        digitalWrite(outPin3, HIGH);
        digitalWrite(outPin4, LOW);
        break;
    }

  }
}
/*------------------------------------------------------------------------------*/