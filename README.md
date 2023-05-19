# repro-entt-views-1016
Reproduction of [issue 1016](https://github.com/skypjack/entt/issues/1016)  
``` Assertion `is_power_of_two(mod)' failed.``` after repacking of component tuple.

### Tested with:  
  g++ 12.2.0 *(Assertion)*  
  Clang-15 *(won't compile)*  
  Clang-16 *(works)*  
