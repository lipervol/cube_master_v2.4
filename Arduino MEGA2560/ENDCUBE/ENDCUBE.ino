#include <Stepper.h>
using namespace std;

const int STEPS = 807;              // 360/1.8
const int stepperSpeed = 100;       // rpm  15.8V 
const int timeBetweenMoves = 180;     // (5ms)    between U2 R2 or U R etc.
const int timeBetweenComs = 1000; // ms    between commands
const int steps_90 = STEPS/4 ;     
const int steps_180 = STEPS/2;
const int steps_270 = -STEPS/4;   

const int _angle[4]={0,steps_90,steps_180,steps_270};
static String command="";

Stepper FF(STEPS,24,25);
Stepper DD(STEPS,26,27);
Stepper BB(STEPS,28,29);
Stepper LL(STEPS,30,31);
Stepper UU(STEPS,32,33);
Stepper RR(STEPS,34,35);

void Getcom();
void Solve();

void setup()
{
    FF.setSpeed(stepperSpeed);
    UU.setSpeed(stepperSpeed);
    RR.setSpeed(stepperSpeed);
    LL.setSpeed(stepperSpeed);
    DD.setSpeed(stepperSpeed);
    BB.setSpeed(stepperSpeed);
    
    Serial.begin(9600);    
    pinMode(LED_BUILTIN, OUTPUT);  
}

void loop()
{
    delay(timeBetweenComs);
    command="";
    Getcom();
    Solve();
}

void Getcom() 
{    
    while(1){
        if (Serial.available()>0) {
               command += char(Serial.read());
                digitalWrite(LED_BUILTIN, HIGH);   
                delay(100);
        }            
         else{                                    
                digitalWrite(LED_BUILTIN, LOW);    
                delay(100);
                break; 
        }
    }       
}

void Solve()
{
    int com_len=command.length();
    for(int i=0;i<com_len;i+=2)
    {
        char ch=command[i];
        int n=command[i+1]-'0';
        
        switch(ch)
        {
            case 'F':FF.step(_angle[n]); for(int i=24;i<26;i++) digitalWrite(i,LOW); break;
            case 'U':UU.step(_angle[n]); for(int i=32;i<34;i++) digitalWrite(i,LOW); break;
            case 'L':LL.step(_angle[n]); for(int i=30;i<32;i++) digitalWrite(i,LOW); break;
            case 'R':RR.step(_angle[n]); for(int i=34;i<35;i++) digitalWrite(i,LOW); break;
            case 'D':DD.step(_angle[n]); for(int i=26;i<28;i++) digitalWrite(i,LOW); break;
            case 'B':BB.step(_angle[n]); for(int i=28;i<30;i++) digitalWrite(i,LOW); break;            
        }
        delay(timeBetweenMoves);

    }
    
}
