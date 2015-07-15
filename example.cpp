#include "flagset.hpp"
#include <cassert>

/**
 * Some random enum to use in tests.
 */
enum class Options : uint64_t
{
  FULLSCREEN,
  INVERT_MOUSE,
  BLA,
  RED_BACKGROUND,
  RED_FOREGROUND,
  LAST__ // mandatory sentinel variable to indicate the end of the enumeration.
};


int test_AND()
{
  FlagSet<Options> red(Options::RED_FOREGROUND | Options::RED_BACKGROUND);

  auto ret = red & Options::RED_BACKGROUND;
  assert(ret);
  assert(ret.count() == 1);

  ret = red & Options::RED_FOREGROUND;
  assert(ret);
  assert(ret.count() == 1);

  ret = red & (Options::RED_FOREGROUND | Options::RED_BACKGROUND);
  assert(ret);
  assert(ret.count() == 2);

  ret = ~red & Options::RED_BACKGROUND;
  assert(ret == false);
  assert(ret.count() == 0);
}

int test_OR()
{
  FlagSet<Options> red;
  red |= Options::RED_FOREGROUND | Options::RED_BACKGROUND;
  assert(red.count() == 2);

  FlagSet<Options> opt;
  opt |= (Options::FULLSCREEN | Options::BLA);

  // FULLSCREEN and BLA match, so this evaluates to true.
  assert(opt & (Options::FULLSCREEN | Options::BLA | Options::RED_FOREGROUND |
                Options::RED_BACKGROUND));

  // Ensure that a group of flag is set
  FlagSet<Options> expected;
  expected |= (Options::FULLSCREEN);
  assert((opt & expected) == expected);

  assert((opt & (Options::RED_FOREGROUND | Options::RED_BACKGROUND)) == false);
  assert((opt & red) == false);

  assert(!(opt & Options::INVERT_MOUSE));
  opt |= ~red;
  assert(opt & Options::INVERT_MOUSE);
}

int test_set_reset()
{
  FlagSet<Options> opt;

  assert(opt.count() == 0);
  opt.set();
  assert(opt.count() == opt.size() && opt.size() == 5);
  opt.reset();
  assert(opt.count() == 0);

  opt.set(Options::BLA);
  assert(opt.count() == 1 && opt[Options::BLA]);
  opt.set(Options::BLA, false);
  assert(opt.count() == 0);
}

int test_type_safety()
{
  // The following will not compile.
  FlagSet<Options> bs;

  // bs & 42;
  // bs &= 42;
  // bs |= 42;
  // bs | 42;
}

int main()
{
  auto t = std::bitset<4>();
  assert((int)Options::FULLSCREEN == 0);
  test_AND();
  test_OR();
  test_set_reset();
}
