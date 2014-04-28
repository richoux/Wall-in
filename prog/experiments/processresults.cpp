#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>
#include <cstring>

#include "stdio.h"
#include "string.h"

bool breakOutputFile(const char *fileName, const char *destinationFolder, const char *code);


int main(int argc, char **argv)
{
  if (argc!=2) {
    printf("usage: processresults [outputfile]\n");
    return 1;
  }

  FILE *fp = fopen(argv[1],"r+");
  if (fp==NULL) {
    printf("cannot open file '%s'\n",argv[1]);
    return 2;
  }
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  double time_accum = 0;
  double cost_accum = 0;
  double count = 0;
  int n_solved = 0;

  while((read = getline(&line, &len, fp)) != -1) {
    for(int i = 0;i<strlen(line);i++) 
      if (line[i]=='\n' || line[i]=='\r') line[i] = 0;

    if (strstr(line,"map size:")!=NULL) {
      int dx,dy;
      sscanf(line,"map size: %i,%i",&dx,&dy);
      printf("%i\t%i\t",dx,dy);
    }
    if (strstr(line,"Elapsed time:")!=NULL) {
      float t;
      sscanf(line,"Elapsed time: %f",&t);
      printf("%f\t",t);
      time_accum+=t;
    }
    if (strstr(line,"Global cost:")!=NULL) {
      int c;
      sscanf(line,"Global cost: %i",&c);
      printf("%i\n",c);
      cost_accum+=c;
      if (c==0) n_solved++;
      count++;
    }
  }
  fclose(fp);

  time_accum/=count;
  cost_accum/=count;

  printf("Number of problems: %g\n", count);
  printf("Average time: %g, average cost: %g\n",time_accum, cost_accum);
  printf("Number of problems with cost = 0: %i\n",n_solved);

  return 0;
}

