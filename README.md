# RGB-LED-cube
Source code for the rgb led cube kit

![Example_img](https://github.com/leonZtiger/RGB-LED-cube/assets/98460339/c57770b5-b4cb-4f19-95ea-a6cf8519e32e)

# Kit
Follow the instructions from the kit and construct the cube.
Purchase kit: https://www.amazon.se/Generic-rgb-led-cube-7x7x7/dp/B0CL3JXH3V/ref=sr_1_11?crid=3AGYSZL191CJ4&keywords=rgb+led+cube&qid=1697807504&sprefix=%2Caps%2C442&sr=8-11
See Video in action:

# Code
This software is meant to only run on the specified kit but can be modified for personal use.
Download the repository and open it in the Arduino IDE, then upload it to the board.

# Rendering lib
simply include the cube.h file to your project and youre done!

The library contains everything to setup the cube and help you render animations.

To use the library call the setup_cube() function in youre setup code.

## Functions

### setup_cube()
Setups the cube. Creates a led controller instance.Should only be called ones at begining of program.

### render_cube()
Renders the content

### clear()
Clears the content

### set_pixel(char x, char y, char z, CRGB color)
Sets the RGB color at specified coordinate. Coordinates most not be out of bound.

### set_pixel(char x, char y, char z, CHSV color)
Sets the HSV color at specified coordinate. Coordinates most not be out of bound.

### add_pixel(char x, char y, char z, CRGB color)
Adds the RGB color at specified coordinate. Coordinates most not be out of bound.

### add_pixel(char x, char y, char z, CHSV color)
Adds the HSV color at specified coordinate. Coordinates most not be out of bound.

