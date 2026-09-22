# SOS Blink — TM4C123GH6PM

---

A simple program that blinks SOS on the TM4C123GH6PM MCU

## Environment
Project is build with Code Compose Studio(Theia) using GNU Arm Toolchain and CMSIS. 

## Requirements
- Code Composer Studio (CCS) installed
- GNU Arm Toolchain installed and added to the CCS

## Usage
1. Open the SOS project in CCS.
2. Flash the code to the board.
3. Press SW1 to start blinking SOS.
4. Press SW2 to stop blinking SOS.

## Limitations
The program uses busy-wait loop to poll switch state, which is not power-efficient and can miss button presses. 
For better responsiveness, the program should be rewritten using GPIO interrupts.
