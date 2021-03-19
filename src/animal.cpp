#include "animal.h"

std::vector<Animal> Animals::animals = {
    {"Sheep", {{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
    {"Pig", {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    {"Rabbit", {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
    {"Horse", {{0, 0}, {1, 0}, {2, 0}}},
    {"Cow", {{0, 0}, {0, 1}, {0, 2}}},
    {"Unicorn", {{0, 0}, {1, 1}, {1, 2}}},
    {"Chicken", {{2, 0}, {1, 1}, {0, 2}}},

    {"Kangaroo", {{0, 0}, {1, 1}, {2, 2}, {3, 3}}},
    {"Platypus", {{0, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {"Crocodile", {{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
    {"Koala", {{0, 0}, {0, 1}, {1, 1}}},
    {"Cockatoo", {{0, 0}, {1, 1}, {2, 1}}},
    {"Tiddalik", {{1, 0}, {0, 1}, {2, 2}}},
    {"Echidna", {{1, 0}, {1, 1}, {0, 2}}},

    {"Zebra", {{0, 1}, {1, 0}, {1, 2}, {2, 1}}},
    {"Hippo", {{0, 0}, {2, 0}, {0, 2}, {2, 2}}},
    {"Giraffe", {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
    {"Lion", {{0, 0}, {0, 1}, {0, 2}}},
    {"Elephant", {{0, 0}, {0, 1}, {1, 0}}},
    {"Gryphon", {{0, 0}, {1, 1}, {0, 2}}},
    {"Rhinoceros", {{1, 0}, {0, 1}, {2, 1}}},

    {"Bear", {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
    {"Skunk", {{1, 0}, {1, 1}, {0, 1}, {0, 2}}},
    {"Beaver", {{1, 0}, {1, 1}, {0, 2}, {2, 2}}},
    {"Moose", {{0, 0}, {1, 1}, {0, 2}}},
    {"Fox", {{0, 0}, {0, 1}, {1, 1}}},
    {"Sasquatch", {{0, 0}, {1, 0}}},
    {"Otter", {{0, 0}, {1, 0}, {1, 1}}},

};

AnimalId Animals::get_animal_id(const std::string& name) {
  for (int i = 0; i < (int)animals.size(); ++i) {
    if (animals[i].name == name) {
      return AnimalId(i);
    }
  }
  return AnimalId::UNKNOWN;
}

Animal Animals::get_animal(const AnimalId& id) { return animals[id]; }
