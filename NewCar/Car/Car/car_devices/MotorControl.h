// Autor:Peter Ridilla

#define IN1 A3
#define IN2 A2

class MotorControl{
	
	public:
	
	static void init(){
		pinMode(IN1, OUTPUT);
		pinMode(IN2, OUTPUT);
	}
	
	static void set_throttle(int speed){
		if(speed)
		{
			if(speed > 0)
			{
				analogWrite(IN1, (speed * 2) + 1);
				digitalWrite(IN2, LOW);
			}
			else
			{
				digitalWrite(IN1, LOW);
				analogWrite(IN2, (speed * -2) + 1);
			}
		}
		else
		{
			digitalWrite(IN1, LOW);
			digitalWrite(IN1, LOW);
		}
	}
};

