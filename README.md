# ILI9341-simple-stm32-library
simple library for ili9341 and stm32
it just have init function, some functions to send data, and to draw rectangles
tested on chinese stm32f407vet6 board, works normally
some notes for chinese displays originally used for arduino
sck = sck
sda/sdi = mosi
thats all for spi, it is used only to send data(what data can we even get from display?)
other are used to chose modes
my pinout looks like:
  
3V	VCC
  
GND	GND
  
PA4	CS
  
PA2	RESET
  
PA3	DC
  
PA7	SDI (MOSI)
  
PA5	SCK	
  
3V LED
