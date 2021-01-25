# ExploringScaleSymmetry
Runnable examples from the book Exploring Scale Symmetry by Thomas Lowe: <https://www.worldscientific.com/worldscibooks/10.1142/11219>

This is research code, built to operate on Windows and Linux operating systems. The code is provided "as-is" for education purposes and so no guarantees are given for its performance or stability.

Build instructions:

## Windows:

Either run the executable that is available in the Releases section of the github page, or build within Microsoft Visual Studio:

1. Download Eigen 3 from: <http://eigen.tuxfamily.org/index.php?title=Main_Page#Download>
2. Download freeglut from here: <https://www.transmissionzero.co.uk/software/freeglut-devel/> (only used for Ch6)
3. load ExploringScaleSymmetry.sln into Microsoft Visual Studio
4. right click the project in Visual Studio, and select 'properties' to go to the Configuration Properties pages
5. under C/C++ | General | Additional Include Directories, add the path to the Eigen folder
6. also add the path to the freeglut library (...\freeglut\include)
7. under Linker | General | Additional Library Directories, add the path to the freeglut lib folder

Now build the project, and run, specifying the chapter and figure number from the book to run, using the command line parameters, e.g: `exploringScaleSymmetry C F` for chapter C, figure F.

## Linux:

The GUI of Chapter 6 requires freeglut to be installed, using:

```sudo apt install freeglut3 freeglut3-dev libglew-dev
sudo apt install mesa-utils
```

Or by downloading from <http://freeglut.sourceforge.net/> and building and installing according to its instructions. Version 3.2.1 has been tested.

Install the Eigen3 vector library:

```sudo apt install libeigen3-dev
```

or download and install Eigen 3 from: <http://eigen.tuxfamily.org/index.php?title=Main_Page#Download>

Then build, from within the ExploringScaleSymmetry root folder:

```cd ExploringScaleSymemtry
mkdir build
cd build
cmake ..
make
cd ..
```

Now run using `\build/exploringScaleSymmetry C F` where C is the chapter number and F is the figure number to run.
