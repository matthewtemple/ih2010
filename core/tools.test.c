#include "ih/core/tools.h"
#include "ih/external/external.h"

#define REPETITIONS 32768

int main(int argc, char *argv[])
{
  unsigned long each_toss;
  unsigned short toss_result;

  if (ih_core_bool_false) {
    printf("tossing a coin %d times\n", REPETITIONS);
    for (each_toss = 0; each_toss < REPETITIONS; each_toss++) {
      toss_result = ih_core_coin_toss();
    }
  }

  return 0;
}
