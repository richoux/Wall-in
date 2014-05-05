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

  int cost = 0;
  double time_accum = 0;
  double cost_accum = 0;
  double best_opt_cost_so_far = -1;
  double best_opt_init_cost_so_far = -1;
  double best_cost_so_far = -1;
  double opt_init_cost_accum_over_attempts = 0;
  double opt_cost_accum_over_attempts = 0;
  double cost_accum_over_attempts = 0;
  double count = 0;
  double count_over_attempts = 0;
  double count_over_attempts_opt = 0;
  int n_solved = 0; // global number of solved
  bool solved_in_at_least_one_attempt = false;
  int n_solved_in_at_least_one_attempt = 0; // solved it at least one of the attempts

  int previous_attempt = 0;
  int attempt = 0;

  while((read = getline(&line, &len, fp)) != -1) {
    for(int i = 0;i<strlen(line);i++) 
      if (line[i]=='\n' || line[i]=='\r') line[i] = 0;

    if (strstr(line,"map size:")!=NULL) {
      int dx,dy;
      sscanf(line,"map size: %i,%i",&dx,&dy);
      printf("%i\t%i\t",dx,dy);
    }
    if (strstr(line,"attempt")!=NULL) {
      previous_attempt = attempt;
      sscanf(line,"attempt %i",&attempt);
      printf("%i\t",attempt);
      if (attempt<=previous_attempt) {
        if (solved_in_at_least_one_attempt) {
          n_solved_in_at_least_one_attempt++;
        }
        solved_in_at_least_one_attempt = false;
        cost_accum_over_attempts+=best_cost_so_far;
        opt_cost_accum_over_attempts+=best_opt_cost_so_far;
        opt_init_cost_accum_over_attempts+=best_opt_init_cost_so_far;
        count_over_attempts++;
        best_cost_so_far = -1;
        best_opt_cost_so_far = -1;
        best_opt_init_cost_so_far = -1;
      }
    }
    if (strstr(line,"Elapsed time:")!=NULL) {
      float t;
      sscanf(line,"Elapsed time: %f",&t);
      printf("%f\t",t);
      time_accum+=t;
    }
    if (strstr(line,"Global cost:")!=NULL) {
      sscanf(line,"Global cost: %i",&cost);
      if (cost==0) {
        printf("%i\n",cost);
      } else {
        printf("%i\n",cost);
      }
      cost_accum+=cost;
      if (best_cost_so_far<0 || cost<best_cost_so_far) best_cost_so_far = cost;
      if (cost==0) {
        n_solved++;
        solved_in_at_least_one_attempt = true;
      }
      count++;
    }
    if (cost==0) {
      if (strstr(line,"Optimization cost:")!=NULL) {
        int c;
        sscanf(line,"Optimization cost: %i",&c);
        if (c==0) {
          printf("%i\n",c);
        } else {
          printf("%i\n",c);
        }
        if (best_opt_cost_so_far<0 || c<best_opt_cost_so_far) best_opt_cost_so_far = c;
        count_over_attempts_opt++;
      }
      if (strstr(line,"Opt Cost BEFORE post-processing:")!=NULL) {
        int c;
        sscanf(line,"Opt Cost BEFORE post-processing: %i",&c);
        if (c==0) {
          printf("%i\n",c);
        } else {
          printf("%i\n",c);
        }
        if (best_opt_init_cost_so_far<0 || c<best_opt_init_cost_so_far) best_opt_init_cost_so_far = c;
      }
    }
  }
  if (solved_in_at_least_one_attempt) n_solved_in_at_least_one_attempt++;
  solved_in_at_least_one_attempt = false;
  cost_accum_over_attempts+=best_cost_so_far;
  count_over_attempts++;
  fclose(fp);

  time_accum/=count;
  cost_accum/=count;
  cost_accum_over_attempts/=count_over_attempts;
  opt_cost_accum_over_attempts/=count_over_attempts_opt;
  opt_init_cost_accum_over_attempts/=count_over_attempts_opt;

  printf("Problems\tbestCost\tnSolved\tOptCostBefore\tOptCostAfter\n");
  printf("%g\t%g\t%d\t%g\t%g\n",count,cost_accum_over_attempts,n_solved_in_at_least_one_attempt,opt_init_cost_accum_over_attempts,opt_cost_accum_over_attempts);
/*
  printf("Number of problems: %g\n", count);
  printf("Average time: %g, average cost: %g\n",time_accum, cost_accum);
  printf("Number of problems with cost = 0: %i\n",n_solved);
  printf("Average cost of best solution across attempts: %g\n", cost_accum_over_attempts);
  printf("Number of problems with cost = 0 (in at least one attempt): %i\n",n_solved_in_at_least_one_attempt);
*/
  return 0;
}

