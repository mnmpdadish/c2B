//
//  gnuplot_pipe.h
//  c2B
//

#pragma once

#include <stdlib.h>
#include <sys/stat.h>


FILE *gnuplot_init ()
{
  FILE *plot_pipe;

  printf ("\nIf the program stop here, please make sure\n");
  printf ("that gnuplot is installed on your system.\n");
  printf ("See README and 'makefile' for details.\n\n");

  //plot_pipe = popen ("gnuplot > /dev/null 2>&1", "w");  // Open plot_pipe to Gnuplot and check for error
  plot_pipe = popen ("gnuplot", "w");  // Open plot_pipe to Gnuplot and check for error

  //fprintf (plot_pipe, "set term %s\n", _TERMINAL); // Set the plot
  fflush (plot_pipe);                                    // flush the plot_pipe
  return (plot_pipe);
}

int gnuplot_image (FILE *plot_pipe, vector<double> data, int Nx, int Ny, float zMin=0.0, float zMax=6.0, int periodization=0)
{
  int i, j;
  char title[80];


// user imposed terminal choice (optional)
#ifdef  TERMINAL
  fprintf (plot_pipe, "set term %s noraise size %d, %d font 'Helvetica,12'\n", TERMINAL, 820, 800); // Set the plot
#else 
  fprintf (plot_pipe, "if  (GPVAL_TERM eq 'wxt'){set term wxt noraise size %d, %d font 'Helvetica,12';\n", 820, 800); 
  fprintf (plot_pipe, "} else {if(GPVAL_TERM eq 'qt') {set term qt noraise size %d, %d font 'Helvetica,12';\n", 820, 800); 
  fprintf (plot_pipe, "} else {if(GPVAL_TERM eq 'x11')  {set term x11 noraise size %d, %d font 'Helvetica,12';}}}\n", 820, 800); 
#endif
  //fprintf (plot_pipe, "set term %s noraise size %d, %d font 'Helvetica,12'\n", _TERMINAL, 820, 800); // Set the plot
  //printf ("set term %s size %d, %d\n", _TERMINAL, 800, 800); // Set the plot
  
  if(periodization==0)      {sprintf(title,"MDC - G periodization");}
  else if(periodization==1) {sprintf(title,"MDC - M periodization");}
  else if(periodization==2) {sprintf(title,"MDC - compact tiling ");}
  else                      {sprintf(title,"MDC - exact          ");}
  sprintf(title + strlen(title)," ");

  fprintf (plot_pipe, "set title \"%s\"\n",title);

  fprintf (plot_pipe, "set palette defined (0 'white',0.002 '#dddddd',0.2 '#8db0fe',0.5 '#3b4cc0',2 '#f49a7b',4 '#b40426')\n");
  fprintf (plot_pipe, "set pm3d map\nset size square\nset cbrange [%1.3f:%1.3f]\n", zMin, zMax);
  
  fprintf (plot_pipe, "Nx=%d\nNy=%d\nset xrange [-0.5:Nx-0.5]\nset yrange [-0.5:Ny-0.5]\n",Nx,Ny);

  fprintf (plot_pipe, "set xtics ('0' 0, '{/Symbol p}' %d) \n set ytics ('0' 0, '{/Symbol p}' %d)\n", Nx-1, Ny-1);
  fprintf (plot_pipe, "set xlabel 'k_x' offset 0.0,1.4 \n set ylabel 'k_y' offset 1.,0.0\n");

  fprintf (plot_pipe, "plot '-' matrix with image\n"); // Set plot format

  for (j = 0; j < Ny; j++)    
  {
    for (i = 0; i < Nx; i++)  
    {
      fprintf (plot_pipe, "%f ", data[(i* Ny) + j]);
    }
    fprintf (plot_pipe, "\n");
  }
  fprintf (plot_pipe, "\ne\n");
  fflush (plot_pipe);            
  return (0);
}

void gnuplot_close (FILE *plot_pipe)
{
  fprintf (plot_pipe, "exit\n");   // Close GNUPlot
  pclose (plot_pipe);              // Close the plot_pipe to Gnuplot
}
