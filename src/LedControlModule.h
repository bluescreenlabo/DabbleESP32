#ifndef LedControlModule_h
#define LedControlModule_h

#include "ModuleParent.h"


class LedControlModule : public ModuleParent
{
public:
	//Constructor
	LedControlModule();
	public: 
	uint8_t readBrightness();
	bool    getpinState();
	uint8_t getpinNumber();
	uint8_t pin = 0xFF;
	uint8_t brightness=0;
    private:
	//Reserve Variables  
	byte value;
	//bool isCallBackAssigned;
	//Process Input Data  
	void processData();
	/*void (*buttonChangeCallBack)(unsigned char , unsigned char ,
								 unsigned char , unsigned char ,
								 unsigned char ,unsigned char  ,
								 unsigned char ,unsigned char );*/
	uint8_t freeChannel = 0;
	uint8_t prevPin = 0xFF;
	uint8_t currentChannel = 0;
	bool pinType = false;   // flag to differentiate pin behaviour means whether pin will be traeted as PWM or digital
};

//Extern Object
extern LedControlModule LedControl;
#endif 