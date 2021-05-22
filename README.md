##########
# README #
##########

This code calculates the spectral weight and DOS (2D square lattice).
The most interesting part is the interactive plot that can explore 
a simple one-body model through using gnuplot.


################
MAIN REPOSITORY:
################

To get the most recent version of the source code on this project,
in command line on a bash terminal (represented here with the "$" sign), type:

$ git clone https://github.com/mnmpdadish/c2B.git


##########
COMPILING:
##########

In order to compile, install gcc, gfortran, Lapack, Gnuplot and 
Cuba (optional) on your computer and type in the source code 
root directory:

$ make

This makefile should work on Linux and Mac.


#############
DEPENDENCIES:

gcc: Tested with version 4.8.1. Must be recent enough.

gfortran: Tested with version 4.6.4. Must be recent enough.

Lapack: Tested with version 3.5.0. Must be recent enough.

Gnuplot: In principle Any version of gnuplot should be enough to run the 
         interactive version of the program properly, "$ ./c2B i" but the code 
         was developped using only version 5.0. If gnuplot is not installed, the 
         interactive version will not work. 

         Troubleshooting:
         - If the interactive task stop, try to install Gnuplot 5.0
         - If the program does not stop but no gnuplot graph appears,
           make sure you have the right terminal. By default, QT is used,
           but you can also choose x11 or wxt. Check which terminal is use by default
           by starting gnuplot, the line "Terminal type set to 'qt'" to specify
           which terminal is used. Then change the terminal in the makefile before 
           compilation using either the option "-DQT" for 'qt', "-DWXT" for 'wxt' or
           "-DX11" for 'x11'. By default, 'qt' is used.

Cuba:      An integration library available via http://www.feynarts.de/cuba/
(optional) Version 4.0 or higher should be used to work and compile correctly.
           This is only required in the calculation of the DOS, but by default
           this functionnality is turned off. To enable it, one must install
           Cuba and add the flag -DCUBA to the compilation option in the makefile.

####
RUN:

To check how to use the code, type:
$ ./c2B

Read this help to learn about the different tasks available ('i', 'mdc', 'dos').
Any other task than the help needs a "para.dat" file that specify the model 
parameters. An example of a "para.dat" file is contained in the "example" 
directory. The "para.dat" file can contain the following parameters:

ETA            Lorentzian broadening of the spectral weight
OMEGA          frequency (OMEGA=0 is the Fermi level)
MU             chemical potential
t              nearest neighbor hopping energy
tp             second nearest neighbor hopping energy
tpp            third nearest neighbor hopping energy
DELTA          coupling energy
periodization  choice of periodization (0=green, 1=cumulant, 2=compact tiling, 3=exact)

VERBOSE  level of verbose of the program
EPSREL   relative error demanded
EPSABS   absolute error demanded
MAXEVAL  maximum number of evaluations
MINEVAL  minimum number of evaluations

nOmega    number of frequency to calculate for the DOS
omegaMin  minimum frequency to calculate for the DOS
omegaMax  maximum frequency to calculate for the DOS

If any of these parameters are not found, default parameters will be used 
(and printed on the screen). A file "para.dat" must be present in the 
directory where the programme "c2B" is called. 


#########
EXAMPLES:

Examples of input and results files are found in the "fig1" and "fig2"
directories. Informations on how to reproduce these results by running 
the code are found in the "README" of each directory. As the name suggest,
these parameter files create the results shown in Fig. 1 and 2 of the 
article.


######
USAGE:

This software is released under MIT License (see LICENSE file). Please consider citing
this code and the associated arxiv and Physical Review article if you reuse this code for
your own research.


