////////////////////////////////////////////////////////
//                                                    //
//      German Swiss Internatonal School              //
//      Class I ROV Control via RS485 tethering       //    
//      Slave (ROV) - utilzing tethering data to      //
//                    control vehicle motion          //   
//      Supervisor : John Shearman                    //
//      Technician : Nicky Lam                        //
//                                                    //
////////////////////////////////////////////////////////

#include <ArduinoRS485.h>
#include <Servo.h>

int EN = 2;//RS485 Transmitter(HIGH) and Receiver(LOW) mode switching pin
int LED = 12;//Receiving Data availability
int pwm1, pwm2, pwm3, pwm4, pwm5, pwm6;
Servo esc1, esc2, esc3, esc4, esc5, esc6;

void setup() {
  
  pinMode(EN, OUTPUT); //RS485 shield pin assignment
  pinMode(LED, OUTPUT);//RS485 availability status
  Serial.begin(9600);
  while (!Serial);
  RS485.begin(9600);
  RS485.receive();
  //6 esc corresponds to 6 thrusters attached (pin, min pulse width, max pulse width)
  esc1.attach(5,1100,1900);   //Front Right Thruster
  esc2.attach(6,1100,1900);   //Front Left Thruster
  esc3.attach(7,1100,1900);   //Back Right Thruster
  esc4.attach(8,1100,1900);   //Back Left Thruster
  esc5.attach(9,1100,1900);   //Middle Right Thruster
  esc6.attach(10,1100,1900);  //Middle Left Thruster
}

void loop() {
  digitalWrite(EN, LOW);//Receiver Mode
  if (RS485.available()) {
    digitalWrite(LED, HIGH);
    while (Serial.available() > 0){
      pwm1=RS485.parseInt(); //read thruster pwm output value through tethering
      pwm2=RS485.parseInt();
      pwm3=RS485.parseInt();
      pwm4=RS485.parseInt();
      pwm5=RS485.parseInt();
      pwm6=RS485.parseInt();

      //look for the newline, a signal of data stop
      if (Serial.read() =='\n') {
        esc1.writeMicroseconds(pwm1);
        esc2.writeMicroseconds(pwm2);
        esc3.writeMicroseconds(pwm3);
        esc4.writeMicroseconds(pwm4);
        esc5.writeMicroseconds(pwm5);
        esc6.writeMicroseconds(pwm6);        
      }
    }
  }else{
    digitalWrite(LED, LOW);
  }
}
