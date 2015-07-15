/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2015 Arnaud Kapp
 */

#include <iostream>
#include <bitset>

/**
 * FlagSet implements a bitset usable with `enum` and `enum class`.
 *
 * It provide a typesafe interface for manipulating the bitset. This helps
 * prevents mistake as the various operator and function will refuse a
 * parameter that doesn't match the expected enum type.
 *
 * A flagset supports one user-defined enumeration. The number of flags
 * (ie the member of the user enumeration) is not limited, as the underlying
 * bitset (std::bitset) can have an arbitrary large size.
 *
 * REQUIREMENTS:
 *      * This code source requires a C++11 compliant compiler.
 *      * The user enumeration shall not explicitely set any value.
 *      * The last enumeration member shall be: "LAST__"
 *
 */
template <typename T>
struct FlagSet
{
  /**
   * Construct a FlagSet. All bits are set to 0.
   */
  FlagSet() = default;

  /**
   * Construct a new FlagSet by making a copy of another FlagSet.
   */
  FlagSet(const FlagSet &o) = default;

  /**
   * Copy rhs into lhs.
   */
  FlagSet &operator=(const FlagSet &o) = default;

  /**
   * Perform the binary OR operation with *this and a value
   * from the user-provided enumeration.
   */
  FlagSet &operator|=(const T &val)
  {
    bitset.set(static_cast<utype>(val));
    return *this;
  }

  /**
   * Perform the binary AND operation on *this and one value from the
   * user provided enumeration and assing the result to *this.
   */
  FlagSet &operator&=(const T &val)
  {
    bool tmp = bitset[static_cast<utype>(val)];
    bitset.reset();
    bitset[static_cast<utype>(val)] = tmp;
    return *this;
  }

  FlagSet &operator|=(const FlagSet &o)
  {
    bitset |= o.bitset;
    return *this;
  }

  FlagSet &operator&=(const FlagSet &o)
  {
    bitset &= o.bitset;
    return *this;
  }

  /**
   * Return a bitset containing the result of the
   * bitwise AND between *this and val.
   *
   * The resulting bitset can contain at most 1 bit.
   */
  FlagSet operator&(const T &val)
  {
    FlagSet ret(*this);
    ret &= val;

    return ret;
  }

  /**
   * Perform a AND binary operation between *this and
   * `val` and return the result as a copy.
   */
  FlagSet operator&(const FlagSet &val)
  {
    FlagSet ret(*this);
    ret.bitset &= val.bitset;

    return ret;
  }

  /**
   * Return a bitset containing the result of the
   * bitwise OR between *this and val.
   *
   * The resulting bitset contains at least 1 bit.
   */
  FlagSet operator|(const T &val)
  {
    FlagSet ret(*this);
    ret |= val;

    return ret;
  }

  /**
   * Perform a OR binary operation between *this and
   * `val` and return the result as a copy.
   */
  FlagSet operator|(const FlagSet &val)
  {
    FlagSet ret(*this);
    ret.bitset |= val.bitset;

    return ret;
  }

  FlagSet operator~()
  {
    FlagSet cp(*this);
    cp.bitset.flip();

    return cp;
  }

  /**
   * The bitset evaluates to true if any bit is set.
   */
  operator bool() const
  {
    return bitset.any();
  }

  /**
   * Below are the method from std::bitset that we expose.
   */

  bool operator==(const FlagSet &o) const
  {
    return bitset == o.bitset;
  }

  std::size_t size() const
  {
    return bitset.size();
  }

  std::size_t count() const
  {
    return bitset.count();
  }

  FlagSet &set()
  {
    bitset.set();
    return *this;
  }

  FlagSet &reset()
  {
    bitset.reset();
    return *this;
  }

  FlagSet &flip()
  {
    bitset.flip();
    return *this;
  }

  /**
   * Set the value of a bit.
   *
   * If no value parameter is given, the bit is set to true.
   */
  FlagSet &set(const T &val, bool value = true)
  {
    bitset.set(static_cast<utype>(val), value);
    return *this;
  }

  FlagSet &reset(const T &val)
  {
    bitset.reset(static_cast<utype>(val));
    return *this;
  }

  FlagSet &flip(const T &val)
  {
    bitset.flip(static_cast<utype>(val));
    return *this;
  }

  bool operator[](const T &val)
  {
    return bitset[static_cast<utype>(val)];
  }

  /**
   * Overload for std::ostream
   */
  friend std::ostream &operator<<(std::ostream &stream, const FlagSet &me)
  {
    return stream << me.bitset;
  }

private:
  using utype = typename std::underlying_type<T>::type;
  std::bitset<static_cast<utype>(T::LAST__)> bitset;
};

/**
 * Provide a free operator allowing to combine two enumeration
 * member into a FlagSet.
 */
template <typename T>
typename std::enable_if<std::is_enum<T>::value, FlagSet<T>>::type
operator|(const T &lhs, const T &rhs)
{
  FlagSet<T> bs;
  bs |= lhs;
  bs |= rhs;

  return bs;
}