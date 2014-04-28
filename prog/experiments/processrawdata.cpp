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
  int nInputFiles = 9;
  const char *inputFiles[9] = {"../data/raw/(2)Benzene-output.txt",
                         "../data/raw/(2)Breaking Point-output.txt",
                         "../data/raw/(2)Heartbreak Ridge-output.txt",
                         "../data/raw/(3)Aztec-output.txt",
                         "../data/raw/(4)Andromeda-output.txt",
                         "../data/raw/(4)Circuit_Breaker-output.txt",
                         "../data/raw/(4)Fortress-output.txt",
                         "../data/raw/(4)Python-output.txt",
                         "../data/raw/path01-output.txt"};
  const char *codes[9]= {"benzene","breakingpoint","heartbreakridge","aztec","andromeda","circuitbreaker","fortress","python","path01"};

  for(int i = 0;i<nInputFiles;i++) {
    printf("# Processing '%s'...\n",inputFiles[i]);
    if (!breakOutputFile(inputFiles[i],"../data/processed",codes[i])) {
      printf("Error when processing file %s...\n", inputFiles[i]);
      return 1;
    }
  }

  return 0;
}


bool breakOutputFile(const char *fileName, const char *destinationFolder, const char *code) {
  FILE *fp = fopen(fileName,"r+");
  int idx = 1;
  if (fp==NULL) return false;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while((read = getline(&line, &len, fp)) != -1) {
      // trim newline characters:
      for(int i = 0;i<strlen(line);i++) 
        if (line[i]=='\n' || line[i]=='\r') line[i] = 0;

      if (strcmp(line,"Chokepoint:")==0) {
        std::vector<std::pair<int,int>> wall_start;
        std::vector<std::pair<int,int>> wall_end;
        // ignore the following two lines:
        read = getline(&line, &len, fp);
        read = getline(&line, &len, fp);

        // now read the start and end of a wall:
        do {
          read = getline(&line, &len, fp);
          if (line[0]=='w') {
            // wall specification: wall from 6,12 to 9,6
            int x1,y1,x2,y2;
            sscanf(line,"wall from %i,%i to %i,%i",&x1,&y1,&x2,&y2);
            wall_start.push_back(std::pair<int,int>(y1,x1));
            wall_end.push_back(std::pair<int,int>(y2,x2));
          }
        } while(line[0]=='w');

        char buffer[512];
        sscanf(line,"%s",buffer);
        int dx = strlen(buffer);
        int dy = strlen(buffer);
        char map[dx][dy];
        for(int i = 0;i<dy;i++) {
          if (i!=0) getline(&line, &len, fp);
          sscanf(line,"%s",buffer);
          for(int j = 0;j<dx;j++) {
            map[j][i] = buffer[j];
          }
        }

        for(int i = 0;i<wall_start.size();i++) {
          std::pair<int,int> start = wall_start[i];
          std::pair<int,int> end = wall_end[i];

          FILE *outfp = NULL;
          if (outfp!=NULL) {
            fclose(outfp);
            outfp=NULL;
          }
          char buffer[512];
          while(outfp==NULL) {
            sprintf(buffer,"%s/%s-%i.txt",destinationFolder,code,idx++);
            outfp = fopen(buffer,"w+");
          }

          printf("bin/experiments %s\n",buffer);
          fprintf(outfp,"Chokepoint:\n");
          fprintf(outfp,"wall from %i,%i to %i,%i\n",start.first,start.second,end.first,end.second);
          for(int i = 0;i<dy;i++) {
            for(int j = 0;j<dx;j++) {
              fprintf(outfp,"%c",map[j][i]);
            }
            fprintf(outfp,"\n");
          }

          fclose(outfp);
          outfp = NULL;  
        }
      }
  }  

  fclose(fp);
  return true;
}
