#include <vector>
#include "base/logging.h"
#include "constraint_solver/constraint_solver.h"

namespace operations_research 
{
  void Wall_In() 
  {
    // Constraint programming engine
    Solver solver("Wall-in");
    
    // Decision variables
    IntVar* const a = solver.MakeBoolVar("A");
    IntVar* const b = solver.MakeBoolVar("B");
    
    // We need to group variables in a vector to be able to create
    // the DecisionBuilder
    std::vector<IntVar*> vars;
    vars.push_back(a);
    vars.push_back(b);

    // Constraints
    solver.AddConstraint(solver.MakeLessOrEqual(solver.MakeSum(a,b)->Var(),1));
    solver.AddConstraint(solver.MakeGreaterOrEqual(solver.MakeSum(a,b)->Var(),1));

    // Make the DecisionBuilder
    DecisionBuilder* const db = solver.MakePhase(vars,
						 Solver::CHOOSE_FIRST_UNBOUND,
						 Solver::ASSIGN_MIN_VALUE);

    // Launch the search
    solver.NewSearch(db);

    // If a solution exists
    if (solver.NextSolution()) 
    {
      LOG(INFO) << "Solution found:";
      LOG(INFO) << "A=" << a->Value() << " " << "B=" << b->Value();
    } 
    // otherwise
    else 
    {
      LOG(INFO) << "Cannot solve problem.";
    } 
    
    solver.EndSearch();    
  }
}

// ----- MAIN -----
int main(int argc, char **argv) {
  operations_research::Wall_In();
  return 0;
}
