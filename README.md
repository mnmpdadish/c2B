# c2B

This code calculates the spectral weight and DOS (2D square lattice).
The most interesting part is the interactive plot that can explore 
a simple one-body model through using gnuplot.


## MAIN REPOSITORY:

To get the most recent version of the source code on this project,
in command line on a bash terminal (represented here with the "$" sign), type:

```
$ git clone https://github.com/mnmpdadish/c2B.git
```

## COMPILING:

In order to compile, install gcc, gfortran, Lapack, Gnuplot and 
Cuba (optional) on your computer and type in the source code 
root directory:

```
$ make
```

This makefile should work on Linux and Mac.


## DEPENDENCIES:

+ **gcc**: Tested with version 4.8.1 and 9.3.0. Must be recent enough.

+ **gfortran**: Tested with version 4.6.4 and 9.3.0. Must be recent enough.

+ **Lapack**: Tested with version 3.5.0. Must be recent enough.

+ **Gnuplot**: Tested with version 5.0. If gnuplot is not installed, the 
               interactive version will not work, but the task 'mdc' will still work.

      Troubleshooting:
         - If the interactive task stop, install Gnuplot.
         - If the program does not stop but no gnuplot plot appears, make sure 
           you have the right terminal. By default, if your gnuplot use 'qt', 'wxt' 
           or 'x11', the program will automatically detect it. In the case this automatic 
           detection does not work, the last line printed on the screen will be:
           "No output will be generated. Please select a terminal with 'set terminal'."
           In that case, you must stop the program (CTRL-C) and check which terminal is 
           used by default when you start gnuplot in command line. At the start of gnuplot, 
           a bunch of line should be printed on the screen, and the last line above the 
           "gnuplot>" command line should be the one specifying the terminal. For example: 
           "Terminal type set to 'wxt'". To change the terminal in this code, you need to 
           add a compilation option in the makefile. For example, the flag: 
           -DTERMINAL=\"x11\" forces the use of terminal x11.
           

+ **Cuba** (optional): An numerical integration library available via http://www.feynarts.de/cuba/
                       Version 4.0 or higher should be used to work and compile correctly.
                       This is only required in the calculation of the DOS, but by default
                       this functionality is turned off. To enable it, one must install
                       Cuba and add the flag -DCUBA to the compilation option in the makefile
                       and add -lcuba to the LINK variable.

## RUN:

To check how to use the code, type:

```
$ ./c2B
```

Read this help to learn about the different tasks available ('i', 'mdc', 'dos').
Any other task than the help needs a "para.dat" file that specifies the model 
parameters. An example of "para.dat" files are contained in the "fig1" and 
"fig2" directories. The "para.dat" file can contain the following parameters:

```
ETA            Lorentzian broadening of the spectral weight
OMEGA          frequency (OMEGA=0 is the Fermi level)
MU             chemical potential
t              nearest neighbor hopping energy
tp             second nearest neighbor hopping energy
tpp            third nearest neighbor hopping energy
DELTA          coupling energy
periodization  choice of periodization (0=green, 1=cumulant, 2=compact tiling, 3=exact)

VERBOSE        level of verbose of the program
EPSREL         relative error tolerated
EPSABS         absolute error tolerated
MAXEVAL        maximum number of evaluations
MINEVAL        minimum number of evaluations

nOmega         number of frequency to calculate for the DOS
omegaMin       minimum frequency to calculate for the DOS
omegaMax       maximum frequency to calculate for the DOS
```

If any of these parameters are not found, default parameters will be used 
(and printed on the screen). A file "para.dat" must be present in the 
directory where the program "c2B" is called. 


## EXAMPLES:

Examples of input and results files are found in the "fig1" and "fig2"
directories. Informations on how to reproduce these results by running 
the code are found in the "README" of each directory. As the name suggests,
these parameter files create the results shown in Fig. 1 and 2 of the 
article.


## USAGE:

This software is released under [MIT](https://choosealicense.com/licenses/mit/) 
License (see LICENSE file). Please consider citing this code and the associated 
arxiv and Physical Review article if you reuse this code for your own research.
See the full citation in the most recent release on the github.

