#define FROM_DABBLE_LIBRARY
#include "DabbleESP32.h"
#include "motorControls.h"

int motorControls::minPulseWidth = 544;
int motorControls::maxPulseWidth = 2400;
int motorControls::minAngle = 0;
int motorControls::maxAngle = 180;

motorControls::motorControls() : ModuleParent(CONTROLS_ID)
{
}

void motorControls::processData()
{
	   /* #ifdef DEBUG
	    Serial.println("Controls:processData");
		#endif */
		functionId=getDabbleInstance().getFunctionId();
		if(functionId == 0x01 || functionId == 0x02)
		{
			
		byte1=getDabbleInstance().getArgumentData(0)[0]; 
		byte2=getDabbleInstance().getArgumentData(0)[1];
		#ifdef DEBUG
		Serial.print("byte1: ");
		Serial.print(byte1);
		Serial.print("byte2: ");
	    Serial.println(byte2);
		#endif
		if(functionId == 0x01)
		{
			
			if(byte1 == 0xf0)
			{
				pwmMotor1=byte2;
			}
			else if(byte1 == 0xff)
			{
				pwmMotor1= -byte2;
			}
			else if(byte1 == 0x0f)
			{
				pwmMotor1= 0;
			}
			else if(byte1 == 0x00)
			{
				pwmMotor1= 0;
			}
			
		}
		
		if(functionId == 0x02)
		{
			if(byte1 == 0xf0)
			{
				pwmMotor2= byte2;
			}
			else if(byte1 == 0xff)
			{
				pwmMotor2= -byte2;
			}
			else if(byte1 == 0x0f)
			{
				pwmMotor2= 0;
			}
			else if(byte1 == 0x00)
			{
				pwmMotor2= 0;
			}
		}
		}
		else if(functionId == 0x03 || functionId == 0x04)
		{
			byte1=getDabbleInstance().getArgumentData(0)[0]; 
			#ifdef DEBUG
			Serial.print("byte1:");
			Serial.println(byte1);
			#endif
			if(functionId == 0x03)
			{
				angleServo1=byte1;
			}
			else if(functionId == 0x04)
			{
			    angleServo2=byte1;
			}
		}
		
}

void motorControls::runMotor1(uint8_t pwm, uint8_t d1, uint8_t d2)
{
    pinMode(d1, OUTPUT);
    pinMode(d2, OUTPUT);

    if (motor1Pin != pwm)
    {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(pwm, 100, 8);
#else
        ledcSetup(0, 100, 8);
        ledcAttachPin(pwm, 0);
#endif
        motor1Pin = pwm;
    }

    int duty = abs(pwmMotor1);

    if (pwmMotor1 > 0)
    {
        digitalWrite(d1, HIGH);
        digitalWrite(d2, LOW);
    }
    else if (pwmMotor1 < 0)
    {
        digitalWrite(d1, LOW);
        digitalWrite(d2, HIGH);
    }
    else
    {
        digitalWrite(d1, LOW);
        digitalWrite(d2, LOW);
    }

#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(pwm, duty);
#else
    ledcWrite(0, duty);
#endif
}

void motorControls::runMotor2(uint8_t pwm, uint8_t d1, uint8_t d2)
{
    pinMode(d1, OUTPUT);
    pinMode(d2, OUTPUT);

    if (motor2Pin != pwm)
    {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(pwm, 100, 8);
#else
        ledcSetup(1, 100, 8);
        ledcAttachPin(pwm, 1);
#endif
        motor2Pin = pwm;
    }

    int duty = abs(pwmMotor2);

    if (pwmMotor2 > 0)
    {
        digitalWrite(d1, HIGH);
        digitalWrite(d2, LOW);
    }
    else if (pwmMotor2 < 0)
    {
        digitalWrite(d1, LOW);
        digitalWrite(d2, HIGH);
    }
    else
    {
        digitalWrite(d1, LOW);
        digitalWrite(d2, LOW);
    }

#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(pwm, duty);
#else
    ledcWrite(1, duty);
#endif
}

void motorControls::runServo1(uint8_t pin)
{
    if (servo1Pin != pin)
    {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(pin, 50, 16);
#else
        ledcSetup(3, 50, 16);
        ledcAttachPin(pin, 3);
#endif
        servo1Pin = pin;
    }
    writeServoAngle(angleServo1, pin);
}

void motorControls::runServo2(uint8_t pin)
{
    if (servo2Pin != pin)
    {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(pin, 50, 16);
#else
        ledcSetup(4, 50, 16);
        ledcAttachPin(pin, 4);
#endif
        servo2Pin = pin;
    }
    writeServoAngle(angleServo2, pin);
}

int motorControls::angleTomicroseconds(int degree)
{
	degree = constrain(degree,minAngle,maxAngle);
	return map(degree,minAngle,maxAngle,minPulseWidth,maxPulseWidth);
}

int motorControls::microsecondsToDuty(int pulse)
{
	pulse=constrain(pulse,minPulseWidth,maxPulseWidth);
	return map(pulse,0,20000,0,65535);
}

void motorControls::writeServoAngle(int angle, uint8_t pin)
{
    int pulse = angleTomicroseconds(angle);
    int duty  = microsecondsToDuty(pulse);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(pin, duty);
#else
    ledcWrite(3, duty);
#endif
}

