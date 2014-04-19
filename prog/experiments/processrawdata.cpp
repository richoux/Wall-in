#include "stdio.h"

int main(int argc, char **argv)
{
  int nInputFiles = 9;
  char *inputFiles[9] = {"../data/raw/(2)Benzene-output.txt",
                         "../data/raw/(2)Breaking Point-output.txt",
                         "../data/raw/(2)Heartbreak Ridge-output.txt",
                         "../data/raw/(3)Aztec-output.txt",
                         "../data/raw/(4)Andromeda-output.txt",
                         "../data/raw/(4)Circuit_Breaker-output.txt",
                         "../data/raw/(4)Fortress-output.txt",
                         "../data/raw/(4)Python-output.txt",
                         "../data/raw/path01-output.txt"};

  for(int i = 0;i<nInputFiles;i++) {
    if (!breakOutputFile(inputFiles[i],"../data/processed")) {
      printf("Error when processing file %s...\n", inputFiles[i]);
      return 1;
    }
  }

  return 0;
}


bool breakOutputFile(char *fileName, char *destinationFolder) {
  FILE *fp = fopen(fileName,"r+");
  if (fp==NULL) return false;

  fclose(fp);
}
