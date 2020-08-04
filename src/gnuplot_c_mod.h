#pragma once

#include <stdlib.h>
#include <sys/stat.h>

#ifdef __APPLE__
    #define _TERMINAL "qt"
#else
    #define _TERMINAL "wxt"
#endif

FILE *gpc_init_image ()
{
	FILE *pipe;
	
  pipe = popen ("gnuplot > /dev/null 2>&1", "w");                  // Open pipe to Gnuplot and check for error
  if (pipe == NULL)
  {
    printf ("Can not find the required Gnuplot executable.\n");
    printf ("\nGraph creation failure\n");
    exit (1);
  }
  
  //fprintf (pipe, "set term %s\n", _TERMINAL); // Set the plot
  fflush (pipe);                                    // flush the pipe
  return (pipe);
}

int gpc_plot_image (FILE *pipe,vector<double> pData, int Nx, int Ny, float zMin=0.0, float zMax=6.0, int type=0, bool flip=false)
{
  int i, j;
  char title[80];
  fprintf (pipe, "set term %s noraise size %d, %d\n", _TERMINAL, 800, 800); // Set the plot
  //printf ("set term %s size %d, %d\n", _TERMINAL, 800, 800); // Set the plot
  
  if(type==0){sprintf(title,"MDC ");}
  else if(type==1){sprintf(title,"RE Gorkov ");}
  else if(type==2){sprintf(title,"IM Gorkov ");}
  else {sprintf(title," ");}
  if(flip) {sprintf(title + strlen(title),"flipped");}
  else {sprintf(title + strlen(title)," ");}

  
  fprintf(pipe, "unset label {1}\n set label 1 \"%s\" at %f,%f \n",title,Nx*0.46,Ny*1.05);
//  printf("set label \"%s\" at %f,%f \n",title,Nx*0.46,Ny*1.05);
  
  
  if(type==0) {
      fprintf (pipe, "set palette defined (0 'white',0.002 '#dddddd',0.2 '#8db0fe',0.5 '#3b4cc0',2 '#f49a7b',4 '#b40426')\n");
      fprintf (pipe, "set pm3d map\nset size square\nset cbtics ticspace\nset cbrange [%1.3f:%1.3f]\n", zMin, zMax);
  } else if (type == 4) {
      fprintf (pipe, "set palette defined (0 '#000000', 0.25 '#141b43', 0.50 '#384e9b', 0.75 '#8488bd', 1 '#ffffff')\n");
      fprintf (pipe, "set pm3d map\nset size square\nset cbtics ticspace\nset cbrange [%1.3f:%1.3f]\n", zMin, zMax);
  } else if (type == 5) {
      fprintf (pipe, "set palette defined (0 '#000000', 0.25 '#7f5728', 0.50 '#ffb346', 0.75 '#ffd487', 1 '#ffffff')\n");
      fprintf (pipe, "set pm3d map\nset size square\nset cbtics ticspace\nset cbrange [%1.3f:%1.3f]\n", zMin, zMax);
  } else {
      fprintf (pipe, "set palette defined (0 '#3b4cc0',0.25 '#8db0fe', 0.49 '#dddddd',0.5 '#ffffff',0.51 '#dddddd',0.75 '#f49a7b',1 '#b40426')\n");
      fprintf (pipe, "set pm3d map\nset size square\nset cbtics ticspace\nset cbrange [%1.3f:%1.3f]\n", zMin, zMax);
  }
  
  fprintf (pipe, "Nx=%d\nNy=%d\nset xrange [-0.5:Nx-0.5]\nset yrange [-0.5:Ny-0.5]\n",Nx,Ny);
  fprintf (pipe, "plot '-' matrix with image\n"); // Set plot format

  for (j = 0; j < Ny; j++)                 // For every row
  {
    for (i = 0; i < Nx; i++)               // For every pixel in the row
    {
      if(flip){fprintf (pipe, "%f ", pData[i + (j*Ny)]);}
      else{    fprintf (pipe, "%f ", pData[(i* Ny) + j]);}
    }
    fprintf (pipe, "\n");                           // End of isoline scan
  }
  fprintf (pipe, "\ne\ne\n");                       // End of spectrogram dataset
    fflush (pipe);                                    // Flush the pipe
  return (0);
}

void gpc_close (FILE *pipe)
{
  fprintf (pipe, "exit\n");                         // Close GNUPlot
  pclose (pipe);                                    // Close the pipe to Gnuplot
}
