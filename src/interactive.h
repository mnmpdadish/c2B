/* 
*  interactive.h
*    Project: c2B
*    Author: Maxime Charlebois
*    License: MIT License
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "mdc.h"
#include "gnuplot_pipe.h"


void plotMDC(MDC &mdc, int per, FILE *hImage=NULL){
  int Nx=mdc.dimension;    
  gnuplot_image(hImage,mdc.mdc_data,Nx,Nx,0.0,6.0,per); 
}

// define some colors for gnuplot
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KBOLD "\033[1m"

// red if lower, green if higher than last plotted value
inline void chooseColor(float val1,float val2){
  float tol = 0.00001;
  if(val1-val2<-tol){printf(KRED);}
  else if(val1-val2>tol){printf(KGRN);}
  else {printf(KNRM);}
}

// an alternative way to access parameters (not the best patch).
void extractValues(Model & model, vector<double> &values){
  values.resize(8);
  values[0] = model.MU;  values[5] = model.OMEGA;  values[6] = model.ETA;
  values[1] = model.t;   values[2] = model.tp;     values[3] = model.tpp;
  values[4] = model.DELTA;
  return;
}

void setValues(Model & model, int param, double value){

  if(param==0) model.MU =value;
  if(param==1) model.t  =value;
  if(param==2) model.tp =value;
  if(param==3) model.tpp=value;
  if(param==4) model.DELTA=value;
  if(param==5) model.OMEGA=value;
  if(param==6) model.ETA=value;
  return;
}

// small function to choose to erase the same line or printing a new line
void lineKind(int value) {
  static int last_value=1; //initialized only on the first use of the function (static)
  if (last_value!=value) {
    if (last_value!=0) printf("\n");
    last_value=value;
  }
  printf("\r");
  fflush(stdout);
}

// print the values (numbers) with the color to say if the value is raised or lowered
inline void printCompact(vector<double> &values, vector<double> &valuesLast, int lastUpdate=0){   

  lineKind(1);
  string parameterNames[7] = {"mu","t","tp","tpp","delta","w","eta"};

  char tab[1024];
  for (int i=0;i< 7; i++){
    strcpy(tab, parameterNames[i].c_str());
    if(lastUpdate==i) printf(KBOLD);
    chooseColor(values[i],valuesLast[i]);
    printf("%s=% 5.3f  ",tab,values[i]);  
    if(i==3) printf("   ");  
    printf(KNRM);
  }
  chooseColor(0.0,0.0);

  fflush(stdout);
}


// update the value of the amplitude
inline void updateAmplitude(Model & model, int index1, float step, vector<double> & values, vector<double> & valuesLast , int lastUpdate=0){ 
  lastUpdate=index1;
  if(index1 < model.nbOfTerms){
    values[index1] +=step;
    setValues(model, index1, values[index1]);
  }
  printCompact(values,valuesLast,lastUpdate);
}


inline void emptyBuffer()
{   
  int c;
  while ((c=getchar()) == ' ' && c == '.'){read( fileno( stdin ), &c, 1 );}
  fflush(stdin);
}

// give keyboard focus of the terminal to the application.
inline void prepareTerminalInputs()
{
  // simple trick to get the input from terminal.
  // taken from http://www.cplusplus.com/forum/unices/11910/
  struct termios oldSettings, newSettings;    
  tcgetattr( fileno( stdin ), &oldSettings ); // get settings
  newSettings = oldSettings;
  newSettings.c_lflag &= (~ICANON & ~ECHO);  // turn off the ECHO and ICANON option
  // ECHO: controls whether input is immediately re-echoed as output.
  // ICANON: the terminal buffers a line at a time, and enables line editing. 
  // Turning both off input is made available to programs immediately
  // This is also known as “cbreak” mode.
  tcsetattr( fileno( stdin ), TCSANOW, &newSettings ); // set settings
}   

void printHelp(double step, MDC &mdc, char decreaseParamKeys[] , char increaseParamKeys[])
{
  printf("\n----------------- c2B Interactive Mode Help ------------------\n");
  printf("Keyboard commands:\n");
  printf("\tctrl-c  - EXIT the program \n");
  printf("\tSPACE   - compute and plot mdc for selected paramters \n");
  printf("\t+ -     - change resolution \n"); 
  printf("\t) (     - change steps \n");
  printf("\tt       - save mdc file \n");
  printf("\tg       - switch between the different 'periodizations' \n");
  printf("\th       - print this help \n\n");
  printf("Resolution = %d by %d\n",mdc.dimension,mdc.dimension);
  printf("Step =% 1.2f\n",step);
  printf("Controlling keys and corresponding parameters\n");

  string parameterNames[7] = {"mu","t","tp","tpp","delta","w","eta"};
  for(int key=0;key<7;key++)
  {
    printf("%s%s%c%s%c       ",KBOLD,KRED,decreaseParamKeys[key],KGRN,increaseParamKeys[key]);
    for(unsigned int l=0; l<parameterNames[key].length(); l++) printf(" ");  // fill space with the parameter lenght name
    if(key==3) printf("   ");  
  }
  printf("%s\n",KNRM);
}

void interactive_mdc(Model &model, MDC & mdc){
  
  FILE *hImage;
  hImage = gnuplot_init();

  model.verbose=0;
  mdc.calculate(model);
  plotMDC(mdc,model.periodization,hImage);

  vector<double> values, valuesLast;
  extractValues(model,values); 
  valuesLast = values;

  float step=0.05;

  printf("Interactive Mode\n");
  printf("resolution = %d by %d\n",mdc.dimension,mdc.dimension);
  printf("step =% 1.2f\n\n",step);

#ifdef AZERTY

  char increaseParamKeys[] =  { 'a', 'z', 'e', 'r', 'u', 'i', 'o', 'p', '\0' };
  char decreaseParamKeys[] =  { 'q', 's', 'd', 'f', 'j', 'k', 'l', 'm', '\0' };

  printf ("Give the focus to your terminal and use your keyboard keys.\n");
  printf ("Here is a simple representation of the useful keys on a \n");
  printf ("%sAZERTY keyboard%s:\n\n",KBOLD,KNRM);
  printf ("....(-....)+\n");
  printf ("%s%sazer%st.%s%suio%s...\n",KBOLD,KGRN,KNRM,KBOLD,KGRN,KNRM);
  printf ("%s%sqsdf%sgh%s%sjkl%s...\n",KBOLD,KRED,KNRM,KBOLD,KRED,KNRM);
  printf ("............\n");
  printf (".[ SPACE ]..\n\n");
  printf ("See help below to learn the effect of each key.\n\n");
  fflush(stdout);

#else    

  char increaseParamKeys[] =  { 'q', 'w', 'e', 'r', 'u', 'i', 'o', 'p', '\0' };
  char decreaseParamKeys[] =  { 'a', 's', 'd', 'f', 'j', 'k', 'l', ';', '\0' };

  printf ("Give the focus to your terminal and use your keyboard keys.\n");
  printf ("Here is a simple representation of the useful keys on a \n");
  printf ("%sQWERTY keyboard%s:\n\n",KBOLD,KNRM);
  printf ("........()-+\n");
  printf ("%s%sqwer%st.%s%suio%s...\n",KBOLD,KGRN,KNRM,KBOLD,KGRN,KNRM);
  printf ("%s%sasdf%sgh%s%sjkl%s...\n",KBOLD,KRED,KNRM,KBOLD,KRED,KNRM);
  printf ("............\n");
  printf (".[ SPACE ]..\n\n");
  printf ("See help below to learn the effect of each key.\n\n");
  fflush(stdout);

#endif

  printHelp(step,mdc,decreaseParamKeys,increaseParamKeys);
  printCompact(values,valuesLast);

  prepareTerminalInputs();

  while ( 1 ) // yes!
  {
    fd_set set;
    struct timeval tv;

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    FD_ZERO( &set );
    FD_SET( fileno( stdin ), &set );

    int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );
    if( res > 0 )
    {
      model.verbose=0;
      char ch[2561];
      if (read( fileno( stdin ), &ch, 2561 )==1){
        char c=ch[0];
        for(int key=0;key<7;key++)
        {
          if(c==increaseParamKeys[key]) {updateAmplitude(model, key, +step,values,valuesLast,key);}
          else if(c==decreaseParamKeys[key]) {updateAmplitude(model, key, -step,values,valuesLast,key);}
        }

        if(c=='+')      { mdc.resize(mdc.dimension+100); lineKind(4); printf("resolution= %4d by %4d\r",mdc.dimension,mdc.dimension); fflush(stdout);}
        else if(c=='-') { mdc.resize(mdc.dimension-100); lineKind(4); printf("resolution= %4d by %4d\r",mdc.dimension,mdc.dimension); fflush(stdout);}

        else if(c==')' and step < 10)  { step*=10; lineKind(5); printf("steps=%1.3f%50s\r",step,""); fflush(stdout);}
        else if(c=='(' and step > 0.001) { step/=10; lineKind(5); printf("steps=%1.3f%50s\r",step,""); fflush(stdout);}

        else if(c==' ') { mdc.calculate(model); plotMDC(mdc,model.periodization,hImage); lineKind(0); valuesLast=values; printCompact(values,valuesLast); }
        else if(c=='t') { mdc.printFile(model, false);}                
        else if(c=='h') {
          printHelp(step,mdc,decreaseParamKeys,increaseParamKeys);
          printCompact(values,valuesLast);
        }
        else if(c=='g') {
          model.periodization = (model.periodization+1)%4;
          mdc.calculate(model); plotMDC(mdc,model.periodization,hImage); lineKind(0);
          if (model.periodization==0) printf("G periodization ");
          if (model.periodization==1) printf("M periodization ");
          if (model.periodization==2) printf("compact tiling  ");
          if (model.periodization==3) printf("exact           ");
          fflush(stdout);
        }

      }
    }
    else if( res < 0 )
    {
      perror( "select error\n" );
      break;
    }
  }
  //next two lines never used because we ctrl-c out of the program.
  //tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
  //gnuplot_close (hImage);
}


