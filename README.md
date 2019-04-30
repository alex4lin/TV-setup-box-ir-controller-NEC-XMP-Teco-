# TV-settop-box-ir-controller-NEC-XMP-Teco-
TV/settop box ir controller (NEC, XMP, Teco) based on ir-slinger

Here are some tools to simulate ir controllers of TV/settop box(NEC generic, XMP, Teco)

1. select the available GPIO port (CPIO17) and connect the ir_diodes with it. 
https://www.raspberrypi-spy.co.uk/2012/06/simple-guide-to-the-rpi-gpio-header-and-pins/

2. Compile it by gcc.
gcc NEC.c -lm -lpigpio -pthread -lrt -o NEC
gcc Teco.c -lm -lpigpio -pthread -lrt -o Teco
gcc XMP.c -lm -lpigpio -pthread -lrt -o XMP

3. Run the tool with sudo & assign parameters.
sudo ./NEC key
      Please assign a key code(0000-FFFF)!
sudo ./Teco key      
	  Please assign a key code(0x00-0x3F)!
sudo ./XMP key
      Please assign a key code(00-FF)!

It is based on the ir-slinger library. Refer below link.
https://github.com/bschwind/ir-slinger
