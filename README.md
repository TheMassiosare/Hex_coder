# Hex_coder
A console based EPROM (currently supporting only 2764) file generator  
## How to use
This project provides a CMake file. Download the src folder, generate a cmake project, specifying
the targeted platform (by default no OS is targeted) setting the pertinent variable and
build it in your directory of choice.   
Hex_coder supports three command arguments to modify how it looks and what it defaults to. 
### Output file name
The created \*.hex will default to a.hex  
You can use -o *filename* to change this to whatever you like.  
The output file will be generated in the same folder the executable is.  
E.g.: -o FILE
### Default memory value
All memory addresses will default to 0xFF as their value.  
To change this, run either -h *hex-value* or -d *decimal-value*   
E.g.: -h 4F
### Console colors
Hex_coder has three different colors for the memory values: red, blue and green.  
If you'd like to have a specific color, you can use the -c *color* command to change it!  
E.g.: -c blue
