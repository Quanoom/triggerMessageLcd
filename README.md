# PIC18F452 LCD Display Control
This project demonstrates how to interface a character LCD with the PIC18F452 microcontroller. The code initializes the LCD, responds to external interrupts, and updates the display accordingly. The interrupt toggles between displaying a welcome message and clearing the display.

## Features : 
- Microcontroller: PIC18F452

- LCD Interface: 8-bit mode

- Interrupt Handling: External interrupt (INT0) to toggle display content

- Busy Flag Handling: Ensures LCD is ready before each command/data write

- Delay using Timer0: Custom delay function based on TMR0

## Hardware Setup : 
- PORTD: Connected to LCD data lines (D0–D7)

- PORTCbits.RC0 (RS): Register Select

- PORTCbits.RC1 (RW): Read/Write

- PORTCbits.RC2 (EN): Enable

- PORTBbits.RB0: External interrupt (INT0) input

## How It Works
On the first button press (rising edge on RB0), the LCD displays "WELCOME TO LCD".

On the second button press, the display is cleared by writing blank characters.

This toggle continues on each button press.

## Project Structure
main(): Initializes peripherals and enters infinite loop to monitor state.

lcdInitialization(): Sets up the LCD in 8-bit mode.

command(), data(), busyFlag(): Functions to communicate with LCD.

function(): ISR for INT0 external interrupt.

delay(): Custom delay using Timer0.

## Compilation & Deployment
Use MPLAB IDE v8.91 with MCC18 Compiler.

Create a new project targeting the PIC18F452.

Add this source file.

Compile and then upload the hex file to the microcontroller's memory

### Note : 
This project was tested and verified using the Proteus isi simulator
