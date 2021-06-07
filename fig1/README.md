# Figure 1

This directory contains the para.dat necessary to reproduce the c2B results
in figure 1 of the article. Results that you should obtain running the
programme are contained in the "results" directory.

```
$ c2B mdc
```

to produce the "mdc.dat" file. 

"mdc.dat" contains an image of the Momentum density curve as a function of kx and ky.

Plot in gnuplot with:
gnuplot> plot 'mdc.dat' matrix with image

The user can also run an interactive version of the code, to visualize via gnuplot the Fermi
surface as a function of the parameters with the command:

```
$ c2B i
```

Cycle through the different Fermi surfaces by pressing 'g' in the interactive mode. This
reproduce the Fig 1e,d,f,c respectively. If you want to obtain the same results with the
task 'mdc', change the 'periodization parameter in para.dat:

       0 = green          (Fig. 1e)
       1 = cumulant       (Fig. 1d)
       2 = compact tiling (Fig. 1f)
       3 = exact          (Fig. 1c)
