#ifndef ANIMAL_H_INCLUDED
#define ANIMAL_H_INCLUDED

#include <map>
#include <vector>

#include "types.h"

using Pattern = std::vector<Square>;

class Animal {
 public:
  Animal(const std::string& name, const std::vector<Square>& pattern)
      : name(name), pattern(pattern) {}

  std::string name;
  Pattern pattern;
};

namespace Animals {

extern std::vector<Animal> animals;

AnimalId get_animal_id(const std::string& name);

Animal get_animal(const AnimalId& id);

}  // namespace Animals

#endif  // !ANIMAL_H_INCLUDED
