#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>
#include <cstring>

#include "/home/flo/ghost/include/variables/building.hpp"
#include "/home/flo/ghost/include/domains/wallinDomain.hpp"
#include "/home/flo/ghost/include/constraints/wallinConstraint.hpp"
#include "/home/flo/ghost/include/objectives/wallinObjective.hpp"
#include "/home/flo/ghost/include/misc/tools.hpp"
#include "/home/flo/ghost/include/misc/wallinTerran.hpp"
#include "/home/flo/ghost/include/solver.hpp"

using namespace ghost;

int main(int argc, char **argv)
{
  if (argc!=4) {
    printf("Usage %s <datafile> <time limit> <attempts>\n", argv[0]);
    printf("The datafile is the result of analyzing a .scx map using Alberto's offline BWTA\n");
    return 1;
  }

  int time_limit = 20;
  int attempts = 3;
  sscanf(argv[2],"%i",&time_limit);
  sscanf(argv[3],"%i",&attempts);

  // yes, yes, I know this is old school C, but I code in "old fashioned" C++ ;)
  FILE *fp = fopen(argv[1],"r+");
  if (fp!=NULL) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while((read = getline(&line, &len, fp)) != -1) {
//      printf("line was '%s'\n", line);
      // trim newline characters:
      for(int i = 0;i<strlen(line);i++) 
        if (line[i]=='\n' || line[i]=='\r') line[i] = 0;

      if (strstr(line,"Chokepoint ")!=NULL) {
        int dx = 0;
        int dy = 0;
        sscanf(line, "Chokepoint %i %i",&dx,&dy);
        std::vector<std::pair<int,int>> wall_start;
        std::vector<std::pair<int,int>> wall_end;
        printf("Processing chokepoint data...\n");
        // ignore the following two lines:
//        read = getline(&line, &len, fp);
//        read = getline(&line, &len, fp);

        // now read the start and end of a wall:
        do {
          read = getline(&line, &len, fp);
          if (line[0]=='w') {
            // wall specification: wall from 6,12 to 9,6
            int x1,y1,x2,y2;
            sscanf(line,"wall from %i,%i to %i,%i",&x1,&y1,&x2,&y2);
            printf("wall from %i,%i to %i,%i\n",x1,y1,x2,y2);
            wall_start.push_back(std::pair<int,int>(y1,x1));
            wall_end.push_back(std::pair<int,int>(y2,x2));
          }
        } while(line[0]=='w');

        // now read the map:
        std::vector< std::pair<int, int> > unbuildables;
        char buffer[512];
        sscanf(line,"%s",buffer);
        for(int i = 0;i<dy;i++) {
          printf("%s\n",buffer);
          for(int j = 0;j<dx;j++) {
            if (buffer[j]!='2') unbuildables.push_back(std::pair<int,int>(i,j));
//            if (buffer[j]!='2') unbuildables.push_back(std::pair<int,int>(j,i)); // since the solver has x/y inverted :)
          }
          if (i!=dy-1) {
            read = getline(&line, &len, fp);
            sscanf(line,"%s",buffer);
          }
        }

        for(int i = 0;i<wall_start.size();i++) {
          std::pair<int,int> start = wall_start[i];
          std::pair<int,int> end = wall_end[i];

	  vector< Building > vec = makeTerranBuildings();
	  WallinDomain domain( dx, dy, unbuildables, &vec, start.first, start.second, end.first, end.second );
	  vector< shared_ptr<WallinConstraint> > vecConstraints = makeTerranConstraints( &vec, &domain );
	  shared_ptr<WallinObjective> objective = make_shared<GapObj>();

          // for(int attempt = 0;attempt<attempts;attempt++) {
          //   printf("attempt %i\n",attempt+1);
	  printf("map size: %i,%i\n",dx,dy);
	  printf("calling solver...\n");

	  Solver<Building, WallinDomain, WallinConstraint> solver(&vec, &domain, vecConstraints, objective );

	  std::cout << "File name: " << argv[1] << std::endl; 
	  solver.solve( time_limit );    
	}

        printf("\n\n");        
      }
    }
    if (line!=NULL) free(line);
    fclose(fp);
  }
}
