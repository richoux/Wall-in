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
  int nInputFiles = 7;
  const char *inputFiles[7] = {"../data/raw/(2)Benzene-output.txt",
//                         "../data/raw/(2)Breaking Point-output.txt",
                         "../data/raw/(2)Heartbreak Ridge-output.txt",
                         "../data/raw/(3)Aztec-output.txt",
                         "../data/raw/(4)Andromeda-output.txt",
                         "../data/raw/(4)Circuit_Breaker-output.txt",
                         "../data/raw/(4)Fortress-output.txt",
                         "../data/raw/(4)Python-output.txt",
//                         "../data/raw/path01-output.txt"
                        };
  const char *codes[9]= {"benzene",/*"breakingpoint",*/"heartbreakridge","aztec","andromeda","circuitbreaker","fortress","python"/*,"path01"*/};

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
            if (x1!=-1 && y1!=-1 && x2!=-1 && y2!=-1 &&
                std::abs(x1-x2)<20 && std::abs(y1-y2)<20) {
              wall_start.push_back(std::pair<int,int>(x1,y1));
              wall_end.push_back(std::pair<int,int>(x2,y2));
            }
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
          sprintf(buffer,"%s/%s-%i.txt",destinationFolder,code,idx++);
          outfp = fopen(buffer,"w+");
          if (outfp==NULL) return false;

          printf("bin/experiments %s $1\n",buffer);
          fprintf(outfp,"Chokepoint:\n");
          int minx = std::min(start.first,end.first)-3;
          int maxx = std::max(start.first,end.first)+3;
          int miny = std::min(start.second,end.second)-3;
          int maxy = std::max(start.second,end.second)+3;
          if (minx<0) minx = 0;
          if (miny<0) miny = 0;
          if (maxx>=dx) maxx = dx-1;
          if (maxy>=dy) maxy = dy-1;
          fprintf(outfp,"wall from %i,%i to %i,%i\n",start.first-minx,start.second-miny,
                                                     end.first-minx,end.second-miny);
          for(int i = miny;i<maxy;i++) {
            for(int j = minx;j<maxx;j++) {
/*
              if (i==start.second && j==start.first) {
                fprintf(outfp,"s");              
              } else if (i==end.second && j==end.first) {
                fprintf(outfp,"t");              
              } else {
                */
                fprintf(outfp,"%c",map[j][i]);              
//              }
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
