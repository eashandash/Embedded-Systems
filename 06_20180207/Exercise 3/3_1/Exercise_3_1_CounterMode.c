/*
	Exercise 3.1.i : Using Systick timer, generate a square wave of 500us and observe in debugger
					You can connect GPIO pin to ear phone to listen tuning fork sound.
								(i) Using counter method
							*/

#include "..\..\..\TM4C_Common\tm4c123gh6pm.h"
#include "..\..\..\TM4C_Common\PLL.h"

/*Function Prototypes here or say function Declarations*/
void PortA_Init(void);				// function for Initialization of Port A for sound Outputs
void SysInit(void);					// initialize SysTick timer
void SysLoad(unsigned long period);	// Load reload value to SysTick timer

/*main function*/
int main(void)
{
	PLL_Init();
	PortA_Init();
	SysInit();

	while(1)
	{
		// your code goes here
		/*GPIO_PORTA_DATA_R = 0x20;
		SysLoad(40000);
		GPIO_PORTA_DATA_R = 0x00;
		SysLoad(40000);*/
		GPIO_PORTA_DATA_R ^= 0x20; //Toggle the Port A5
		SysLoad(40000); //Reload Systick Timer with the value
	}
}

/*function definition for Port A Initialization*/
void PortA_Init(void)
{
	unsigned long volatile delay;
	/*PORT Initialization*/
	SYSCTL_RCGC2_R |= 0x01;				// Port A clock
	delay = SYSCTL_RCGC2_R;				// wait 3-5 bus cycles
	GPIO_PORTA_DIR_R |= 0x20;			// PA5 output
	GPIO_PORTA_AFSEL_R &= ~0x20;		// not alternative
	GPIO_PORTA_AMSEL_R &= ~0x20;		// no analog
	GPIO_PORTA_PCTL_R &= ~0x00F00000;	// bits for PA5
	GPIO_PORTA_DEN_R |= 0x20;			// enable PA5
}

/*function definition for loading Systick Reload Value and Creating a delay*/
void SysLoad(unsigned long period)
{
	NVIC_ST_RELOAD_R = period -1;			// assign the Reload register value as (Period-1), since the value in register as 0 will also incur a comparison
	NVIC_ST_CURRENT_R = 0;					// any value written to CURRENT clears
	while((NVIC_ST_CTRL_R&0x00010000)==0);	// wait for count flag
}

/*function definition for Systick Initialization*/
void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;								// Control mode register as 0
	NVIC_ST_CURRENT_R = 0;							// any write to current clears it
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R & 0x00FFFFFF;	// priority 0
	NVIC_ST_CTRL_R = 0x00000005;					// enable with core clock and interrupts
}
