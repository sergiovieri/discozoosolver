#include <iostream>

#include "animal.h"
#include "solver.h"

Solver solver;

void handle_animal() {
  std::string animal;
  std::cin >> animal;
  AnimalId id = Animals::get_animal_id(animal);
  if (id == AnimalId::UNKNOWN) {
    std::cout << "Unknown animal" << std::endl;
  } else {
    solver.toggle_animal(id);
  }
}

void handle_observe() {
  std::string rs, cs, animal;
  std::cin >> rs >> cs >> animal;
  Square s = {stoi(rs), stoi(cs)};
  AnimalId id;
  if (animal == "Nothing") {
    id = AnimalId::NOTHING;
  } else if (animal == "Unknown") {
    id = AnimalId::UNKNOWN;
  } else {
    id = Animals::get_animal_id(animal);
  }
  solver.add_observation(s, id);
}

void handle_calc() { solver.calculate(); }

int main(int argc, char* argv[]) {
  while (1) {
    std::string cmd;
    std::cin >> cmd;
    if (cmd == "animal") {
      handle_animal();
    } else if (cmd == "observe") {
      handle_observe();
    } else if (cmd == "calc") {
      handle_calc();
    } else if (cmd == "probs") {
      solver.probabilities();
    } else if (cmd == "quit") {
      break;
    } else {
      std::cout << "Unknown command: " << cmd << std::endl;
    }
    solver.print_status();
  }
  return 0;
}
