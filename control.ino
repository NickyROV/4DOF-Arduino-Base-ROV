////////////////////////////////////////////////////////
//                                                    //
//      German Swiss Internatonal School              //
//      Class I ROV Control via RS485 tethering       //
//      Master - map control input into thruster PWM  //
//               and send it through via RS485        //
//      Supervisor : John Shearman                    //
//      Technician : Nicky Lam                        //
//                                                    //
////////////////////////////////////////////////////////

#include <ArduinoRS485.h>

int EN = 2;//RS485 Transmitter(HIGH) and Receiver(LOW) mode switching pin
int LED = 10;//Transmiting Data availability
int tUpper = 1530 ; //Upper activation thershold
int tLower = 1470 ; //Lower activation thershold
int pwm1,pwm2,pwm3,pwm4,pwm5,pwm6 ; //Calculated PWM for Thruster ESC

void setup(){
  pinMode(EN, OUTPUT);  //RS485 shield pin assignemnt
  pinMode(LED, OUTPUT); //RS485 data sending status
  RS485.begin(9600);
  }
 
void loop(){ 
  digitalWrite(EN, HIGH);//Transmitter Mode
  RS485.beginTransmission();
  digitalWrite(LED,HIGH);//Sending data
  int a0 = map(analogRead(0), 110, 900, 1100, 1900);   //Yaw (110 & 900 are calibrated value)
  int a1 = map(analogRead(1), 110, 900, 1100, 1900);   //Heave (110 & 900 are calibrated value)
  int a2 = map(analogRead(2), 110, 900, 1100, 1900);   //Sway (110 & 900 are calibrated value)
  int a3 = map(analogRead(3), 110, 900, 1100, 1900);   //Surge (110 & 900 are calibrated value)

  if (a1<tLower||a1>tUpper) {                     //Heave - check for activation
    pwm5=pwm6=a1;  //desend and ascend
  }else{
    pwm5=pwm6=1500;//reset 
  }

//Yaw(a0) action supersede Surge(a3) and Sway(a2), if a0 summon, a3 & a2 will be ignored
  if (a0<tLower) {                      //Yaw - Counter ClockWise
    pwm1=pwm4=a0;                        //Thruster Forward Drive
    pwm2=pwm3=reverse(a0);//Thruster Backward Drive      
  
  }else if (a0>tUpper){                                 //Yaw- ClockWise
    pwm1=pwm4=a0;
    pwm2=pwm3=reverse(a0); 

       
  }else{                  //No Yaw action, check for Surge and Sway
    if (a2>tUpper && a3>tUpper){                        //moveForwardRight
      pwm1=pwm4=1500 ;
      pwm2=(a2+a3)/2;                                   //average PWM
      pwm3=reverse((a3+a3)/2);
  
    } else if (a3>tUpper && a2>tLower && a2<tUpper){    //moveForward
      pwm1=pwm2=a3;
      pwm3=pwm4=reverse(a3);
    } else if (a2<tLower && a3>tUpper){                 //moveForwardLeft
      pwm1=(reverse(a2)+a3)/2;
      pwm2=pwm3=1500;
      pwm4=reverse((reverse(a2)+a3)/2);
    } else if (a2>tUpper && a3<tLower){                //moveBackwardRight
      pwm1=reverse((reverse(a3)+a2)/2);
      pwm2=pwm3=1500;
      pwm4=(reverse(a3)+a2)/2;
    } else if ( a2>tLower && a2<tUpper && a3<tLower){ //moveBackward
      pwm1=pwm2=a3;
      pwm3=pwm4=reverse(a3);
    } else if (a2<tLower && a3<tLower){   //moveBackwardLeft
      pwm1=pwm4=1500;
      pwm2=(a2+a3)/2;
      pwm3=reverse((a2+a3)/2);
    } else if (a2>tUpper && a3>tLower && a3<tUpper){ //straftRight
      pwm1=pwm3=reverse(a2);
      pwm2=pwm4=a2;
    } else if (a2<tLower && a3>tLower &&a3<tUpper){ //straftLeft
      pwm1=pwm3=reverse(a2);
      pwm2=pwm4=a2;
    } else { //stayput
      pwm1=pwm2=1500;
      pwm3=pwm4=1500;
    }
  }

  Serial.print(pwm1);//send ASCII
  Serial.print(',');
  Serial.print(pwm2);
  Serial.print(',');
  Serial.print(pwm3);
  Serial.print(',');
  Serial.print(pwm4);
  Serial.print(',');
  Serial.print(pwm5);
  Serial.print(',');
  Serial.print(pwm6);
  Serial.print('\n');
  RS485.endTransmission();
  //  delay(100);                           
}

int reverse(int forward){           //return backward PWM
  int backward = ((forward-1500)*-1+1500);
  return backward;
}
