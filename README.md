# ILI9341-simple-stm32-library
simple library for ili9341 and stm32  
it just have init function, some functions to send data, and to draw rectangles  
Originaly i used HAL, but it is too slow, now i am using LL, that is faster  
my pinout looks like:  
  
3V	VCC
  
GND	GND
  
PA4	CS
  
PA2	RESET
  
PA3	DC
  
PA7	SDI (MOSI)
  
PA5	SCK	
  
3V LED
