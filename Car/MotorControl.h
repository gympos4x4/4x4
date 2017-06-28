class MotorControl{
  
  public:
  
    static void init(int motorPin1, int motorPin2, int enablePin){
      pinMode(motorPin1, OUTPUT);
      pinMode(motorPin2, OUTPUT);
      pinMode(enablePin, OUTPUT);
    }
    
    static void go(int speed, int motorPin1, int motorPin2, int enablePin){
      analogWrite(enablePin, speed);
      int temp = 0;
      if(speed > 0){
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
      }
      else{
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
      }
    }
};

