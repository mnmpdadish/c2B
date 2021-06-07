# Figure 2

This directory contains the para.dat necessary to reproduce the c2B results
in figure 2 of the article. Results that you should obtain running the
programme are contained in the "results" directory.

In order to reproduce these results, simply type 
(in the present directory):

```
$ c2B mdc
```

to produce the "mdc.dat" file, and

```
$ c2B dos
```

to produce the "dos.dat" file. 

"mdc.dat" contains an image of the Momentum density curve as a function 
          of kx and ky.
"dos.dat" contains the DOS as a function of the frequency $\omega$.

Plot in gnuplot with:
gnuplot> plot 'mdc.dat' matrix with image

Plot in gnuplot with:
gnuplot> plot 'dos.dat' u 1:2 w l

The user can also run an interactive version of the code, to visualize via gnuplot the Fermi
surface as a function of the parameters with the command:

```
$ c2B i
```

Cycle through the different Fermi surfaces by pressing 'g' in the interactive mode. This
reproduce the Fig 1e,d,f,c respectively. If you want to obtain the same results with the
task 'mdc', change the 'periodization parameter in para.dat:

       0 = green          (Fig. 2g)
       1 = cumulant       (Fig. 2b)
       2 = compact tiling (not published)
       3 = exact          (not published)
       
Try changing the "mu" and "eta" to obtain all the spectral weight 
of Fig. 2.

