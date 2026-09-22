/*
The program sends SOS repeteadly
S: ON 1/2s - OFF 1/2 s - ON 1/2s - OFF 1/2s - ON 1/2s - OFF 1/2s
gap 1.5 s
O: ON 1.5s - OFF 1/2s - ON 1.5s - OFF 1/2s - ON 1.5s - OFF 1/2s
gap 1.5 s
S: ON 1/2s - OFF 1/2 s - ON 1/2s - OFF 1/2s - ON 1/2s - OFF 1/2s
delay 5 seconds between SOS
Pressing SW1 starts SOS
Pressing SW2 stops SOS
Input: SW1(PF4), SW2(PF0)
Output: Red LED(PF1)
*/

#include "TM4C123GH6PM.h"

#define SW1 (1 << 4U)
#define SW2 (1 << 0U)
#define LED (1 << 1U)

#define SYSCLK_HZ 16000000U
#define CYCLES_PER_TICK \
	4U // the delay overhead (setup stack, pop stack ...) from disassembly
#define TICKS_PER_MS ((SYSCLK_HZ / 1000U / CYCLES_PER_TICK))

// prototypes
void initGPIOF(void);
void delay(uint16_t ms);
void sendSOS(void);

int main(void) {
	initGPIOF();

	while (1) {
		// wait till the SW1 is pressed
		while (GPIOF->DATA_Bits[SW1]) {
			;
		}
		// wait till the SW2 is pressed
		while (GPIOF->DATA_Bits[SW2]) {
			sendSOS();
		}
		GPIOF->DATA_Bits[LED] = 0x00;
	}
// return 0; // unreachable code
}

// needed for startup_TM4C123GH6PM.c
__attribute__((naked)) void assert_failed(char const *file, int line) {
	/* TBD: damage control */
	NVIC_SystemReset(); /* reset the system */
}

void initGPIOF(void) {
	// enable clocking
	SYSCTL->RCGCGPIO |= (1 << 5U);
	volatile unsigned long delay =
			SYSCTL->RCGCGPIO; // dummy read to add small delay

	// unlock register
	GPIOF->LOCK = 0x4C4F434B;

	// allow changes to PF0 -PF4
	GPIOF->CR = 0x1F;

	// disable analog functions
	GPIOF->AMSEL = 0x00;

	// enable pull ups for PF0 and PF4
	GPIOF->PUR |= SW1 | SW2;

	// enable direction
	GPIOF->DIR &= ~(SW1 | SW2); // as input
	GPIOF->DIR |= LED;			// as output

	// no alternate functions
	GPIOF->AFSEL = 0x00;
	GPIOF->PCTL = 0x00;

	// enable digital pins
	GPIOF->DEN = 0x1F;
}

void delay(uint16_t ms) {
	volatile uint32_t ticks = ms * TICKS_PER_MS;
	while (ticks)
		--ticks;
}

void sendSOS(void) {
	// S
	for (uint32_t i = 0; i < 3; ++i) {
		GPIOF->DATA_Bits[LED] = LED;
		delay(500);
		GPIOF->DATA_Bits[LED] = 0x00;
		delay(500);
	}
	delay(1500);

	// O
	for (uint32_t i = 0; i < 3; ++i) {
		GPIOF->DATA_Bits[LED] = LED;
		delay(1500);
		GPIOF->DATA_Bits[LED] = 0x00;
		delay(500);
	}
	delay(1500);

	// S
	for (uint32_t i = 0; i < 3; ++i) {
		GPIOF->DATA_Bits[LED] = LED;
		delay(500);
		GPIOF->DATA_Bits[LED] = 0x00;
		delay(500);
	}
	delay(5000); // delay for 5 seconds
}