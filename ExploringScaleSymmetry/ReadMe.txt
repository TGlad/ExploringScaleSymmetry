*NOT READY FOR USE, NOT YET COMPLETE*
*Exploring Scale Symmetry*

Code to run the methods shown in the book: https://www.worldscientific.com/worldscibooks/10.1142/11219

You will need to build the code, as described below, then run the executable:

>> exploringScaleSymmetry

This will print the set of figures that can be generated. For chapter C, figure F run >> exploringScaleSymmetry C F

*Building on Windows*
- Download Eigen 3 from: http://eigen.tuxfamily.org/index.php?title=Main_Page#Download
- load ExploringScaleSymmetry.sln into Microsoft Visual Studio
- right click the project in Visual Studio, and select 'properties' to go to the Configuration Properties pages
- Download freeglut from here: https://www.transmissionzero.co.uk/software/freeglut-devel/ (only used for Ch6)
- under C/C++ | General | Additional Include Directories, add the path to the Eigen folder
-  also add the path to the freeglut library (...\freeglut\include)
- under Linker | Genreal | Additional Library Directories, add the path to the freeglut lib folder

Now you should be able to build and run the examples.

*Building on Linux (or through CMake)*

- Download Eigen 3 from: http://eigen.tuxfamily.org/index.php?title=Main_Page#Download
- Download freeglut from http://freeglut.sourceforge.net/
- build and install freeglut according to its build instructions
- in the ExploringScaleSymmetry/ExploringScaleSymmetry folder, mkdir build, cd build
- cmake .. & make

this should generate the exploringScaleSymmetry executable.