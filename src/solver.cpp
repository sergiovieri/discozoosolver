#include "solver.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <unordered_map>

using namespace Search;

namespace {

std::unordered_map<GridState, double> memo;

template <typename T>
void print_grid(const Grid<T>& grid) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      std::cout << std::fixed << std::setprecision(2) << grid.data[i][j]
                << '\t';
    }
    std::cout << std::endl;
  }
}

template <AnimalId ID>
int count_grid(const GridState& grid_state) {
  int res = 0;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (grid_state.data[i][j] == ID) ++res;
    }
  }
  return res;
}

int count_grid(const GridState& grid_state, const AnimalId& id) {
  int res = 0;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (grid_state.data[i][j] == id) ++res;
    }
  }
  return res;
}

void count_probability(const SolutionSet& solution_set,
                       Grid<double>& probability) {
  Grid<int> count = {};
  for (const auto& config : solution_set) {
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        if (config->grid_state.data[i][j] >= 0) {
          count.data[i][j]++;
        }
      }
    }
  }
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      probability.data[i][j] = (double)count.data[i][j] / solution_set.size();
    }
  }
}

bool fill_grid_state_with_animal(GridState& grid_state, const AnimalId& id,
                                 const Square& loc) {
  const Pattern& pattern = Animals::get_animal(id).pattern;
  int num = count_grid(grid_state, id);
  for (const auto& s : pattern) {
    Square s2 = s.offset(loc);
    if (!s2.valid()) return false;
    const auto cur = grid_state.get(s2);
    if (cur != AnimalId::UNKNOWN && cur != id) return false;
    if (cur == id) --num;
  }
  if (num) return false;
  for (const auto& s : pattern) {
    Square s2 = s.offset(loc);
    grid_state.set(s2, id);
  }
  return true;
}

void generate_solution_set_rec(int idx, Configuration& config,
                               const GridState& grid_state,
                               const std::vector<AnimalId>& animals,
                               SolutionSet& solution_set) {
  if (idx == (int)animals.size()) {
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        AnimalId id = grid_state.data[i][j];
        if (id == AnimalId::UNKNOWN) id = AnimalId::NOTHING;
        config.grid_state.data[i][j] = id;
      }
    }
    solution_set.push_back(new Configuration(config));
    return;
  }
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      auto next_grid_state = grid_state;
      if (!fill_grid_state_with_animal(next_grid_state, animals[idx], {i, j})) {
        continue;
      }
      config.locs.emplace_back(i, j);
      generate_solution_set_rec(idx + 1, config, next_grid_state, animals,
                                solution_set);
      config.locs.pop_back();
    }
  }
}

void generate_solution_set(const GridState& grid_state,
                           const std::vector<AnimalId>& animals,
                           SolutionSet& solution_set) {
  Configuration config;
  config.locs.reserve(animals.size());
  generate_solution_set_rec(0, config, grid_state, animals, solution_set);
}

double search(const SolutionSet& solution_set, const GridState& grid_state);

#if 0
int calculate_split(const SolutionSet& solution_set,
                    const GridState& grid_state, const Square& loc) {
  std::map<AnimalId, SolutionSet> pt;
  int res = 0;
  for (const auto ptr : solution_set) {
    pt[ptr->grid_state.get(loc)].push_back(ptr);
  }
  for (const auto& i : pt) {
    res = std::max(res, (int)i.second.size());
  }
  return res;
}

void get_best_guess(const SolutionSet& solution_set,
                    const GridState& grid_state, Square& s) {
  int best_split = 1e9;
  double best_prob = -1.0;
  Grid<double> probs;
  count_probability(solution_set, probs);
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (grid_state.data[i][j] != AnimalId::UNKNOWN) continue;
      int cur_split = calculate_split(solution_set, grid_state, {i, j});
      double cur_prob = probs.data[i][j];
      std::cout << "split " << i << ' ' << j << ": " << cur_split << ' '
                << cur_prob << std::endl;
      if (cur_split < best_split ||
          (cur_split == best_split && cur_prob > best_prob)) {
        best_split = cur_split;
        best_prob = cur_prob;
        s = {i, j};
      }
    }
  }
}
#endif

double find_expectation(const SolutionSet& solution_set,
                        const GridState& grid_state, const Square& loc) {
  double res = 0.0;
  std::map<AnimalId, SolutionSet> pt;
  for (const auto ptr : solution_set) {
    pt[ptr->grid_state.get(loc)].push_back(ptr);
  }
  if (pt.size() == 1 && pt.begin()->first == AnimalId::NOTHING) return -1.0;
  for (const auto& i : pt) {
    GridState next_grid_state = grid_state;
    next_grid_state.set(loc, i.first);
    res += (double)i.second.size() / solution_set.size() *
           search(i.second, next_grid_state);
  }
  return res;
}

double search(const SolutionSet& solution_set, const GridState& grid_state) {
  if (solution_set.size() == 0) {
    return std::nan("");
  }
  int guesses =
      count_grid<AnimalId::NOTHING>(grid_state) +
      (25 - count_grid<AnimalId::NOTHING>(solution_set[0]->grid_state));
  if (solution_set.size() == 1) {
    return (guesses <= 10) ? 1.0 : 0.0;
  }

  if (guesses > 10) return 0.0;

  auto it = memo.find(grid_state);
  if (it != memo.end()) return it->second;

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (grid_state.data[i][j] != AnimalId::UNKNOWN) continue;
      if (solution_set[0]->grid_state.data[i][j] == AnimalId::NOTHING) continue;
      bool definite = true;
      for (const auto& config : solution_set) {
        if (config->grid_state.data[i][j] == AnimalId::NOTHING) {
          definite = false;
          break;
        }
      }
      if (definite) {
        return find_expectation(solution_set, grid_state, {i, j});
      }
    }
  }
#if 0
  int best = 1e9;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (grid_state.data[i][j] != AnimalId::UNKNOWN) continue;
      best = std::min(best, calculate_split(solution_set, grid_state, {i, j}));
    }
  }
#endif
  double res = -1.0;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (grid_state.data[i][j] != AnimalId::UNKNOWN) continue;
      res = std::max(res, find_expectation(solution_set, grid_state, {i, j}));
    }
  }
  return memo[grid_state] = res;
}

}  // namespace

Solver::Solver() {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      grid_state.data[i][j] = AnimalId::UNKNOWN;
    }
  }
}

void Solver::toggle_animal(const AnimalId& id) {
  int idx = get_animal_idx(id);
  if (idx == -1) {
    animals.push_back(id);
  } else {
    animals.erase(animals.begin() + idx);
  }
}

void Solver::add_observation(const Square& square, const AnimalId& id) {
  grid_state.set(square, id);
}

void Solver::calculate() {
  SolutionSet solution_set;
  generate_solution_set(grid_state, animals, solution_set);
  std::cout << "Number of solutions: " << solution_set.size() << std::endl;

  if (solution_set.size() == 0) {
    std::cout << "Invalid!" << std::endl;
    return;
  }
  Grid<double> probs;
  count_probability(solution_set, probs);
  std::pair<std::pair<int64_t, int>, Square> best = {{-1, -1}, {-1, -1}};
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (grid_state.data[i][j] != AnimalId::UNKNOWN) {
        expected.data[i][j] = -1.0;
        continue;
      }
      std::cout << "Computing " << i << ", " << j << ": " << std::flush;
      expected.data[i][j] = find_expectation(solution_set, grid_state, {i, j});
      best = std::max(
          best, {std::make_pair((int64_t)std::round(expected.data[i][j] * 100),
                                (int)std::round(probs.data[i][j] * 100)),
                 {i, j}});
      std::cout << expected.data[i][j] << std::endl;
    }
  }

  print_grid(expected);
  std::cout << "Best: " << best.second.r << ' ' << best.second.c
            << " with win prob: " << best.first.first
            << "%, discover prob: " << best.first.second << "%" << std::endl;

  for (const auto& i : solution_set) {
    delete i;
  }
  solution_set.clear();
}

void Solver::probabilities() {
  SolutionSet solution_set;
  generate_solution_set(grid_state, animals, solution_set);
  std::cout << "Number of solutions: " << solution_set.size() << std::endl;

  Grid<double> probs;
  count_probability(solution_set, probs);
  print_grid(probs);
}

int Solver::get_animal_idx(const AnimalId& id) {
  auto it = std::find(animals.begin(), animals.end(), id);
  if (it == animals.end()) return -1;
  return it - animals.begin();
}

void Solver::print_status() {
  std::cout << "Animals list:" << std::endl;
  for (const auto& id : animals) {
    std::cout << Animals::get_animal(id).name << std::endl;
  }
  print_grid(grid_state);
}