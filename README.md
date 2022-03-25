# Hex_coder
A console based EPROM (currently supporting only 2764) file generator  
## How to use
The simplest way to use this is by downloading the source files and building it through VisualStudio, as some of the Windows 
libraries needed for this project are already included by default with the VS compiler.  
Once the Hex_coder.exe file exists, everything is ready.  
Hex_coder supports three command arguments to modify how it looks and what it defaults to. 
### Output file name
The created \*.hex will default to a.hex  
You can use -o *filename* to change this to whatever you like.  
E.g.: -o FILE
### Default memory value
All memory addresses will default to 0xFF as their value.  
To change this, run -d *hex-value*   
E.g.: -d 4F
### Console colors
Hex_coder has three different colors for the memory values: red, blue and green.  
If you'd like to have a specific color, you can use the -c *color* command to change it!  
E.g.: -c blue
