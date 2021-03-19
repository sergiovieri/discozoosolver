#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED

#include <vector>

#include "animal.h"

namespace Search {

using GridState = Grid<AnimalId>;

struct Configuration {
  std::vector<Square> locs;
  GridState grid_state;
};

using SolutionSet = std::vector<Configuration*>;

}  // namespace Search

class Solver {
 public:
  Solver();

  void toggle_animal(const AnimalId& id);

  void add_observation(const Square& square, const AnimalId& id);

  void calculate();

  void probabilities();

  int get_animal_idx(const AnimalId& name);

  void print_status();

  std::vector<AnimalId> animals;
  Search::GridState grid_state;
  Grid<double> expected;
};

#endif  // !SOLVER_H_INCLUDED
