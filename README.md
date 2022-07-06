# ILI9341-simple-stm32-library   
# Usage  
# Step 1  
activate spi1, and dma in circular mode, use HAL  
# Step 2  
Copy display.h to Inc folder  
# Step 3  
include it in main.c (or other file) after creating spi handle and spi dma handle  
# Step4  
Init display, and draw what you need  
# How it works?  
my idea is that this driver, allow me to set color of every pixel i need, in my work area. That means that i can create an image (framebuffer), send it via dma in Circular mode, set working arrea on fullscreen, and finnaly, just modify framebuffer, to draw.  
Yes, i just put display handling on dma  
my pinout looks like:  
  
3V	VCC
  
GND	GND
  
PA4	CS
  
PA2	RESET
  
PA3	DC
  
PA7	SDI (MOSI)
  
PA5	SCK	
  
3V LED
