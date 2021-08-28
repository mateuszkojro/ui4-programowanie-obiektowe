#include "catch2/catch.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <utility>
#include "utils.h"

class Value {
public:
  Value() = default;
  virtual ~Value() = default;

  enum Type { Number } type_;
};

using std::string;

class Number {
public:
  Number(double number);
  static std::pair<Number, string> Parse(string number);

  bool operator==(const Number &other) const;
  bool operator!=(const Number &other) const;

  ~Number() = default;

  friend std::ostream &operator<<(std::ostream &os, const Number &n);

private:
  double value_;
};

class Operator {
public:
  enum Type { Add, Subtract, Multiply, Divide };

  static std::pair<Operator, string> Parse(const string &op);
  Operator(Type t);

  Type type();

  bool operator==(const Operator &op) const;
  bool operator==(const Operator::Type &op) const;

  ~Operator() = default;

  friend std::ostream &operator<<(std::ostream &os, const Operator &n);

private:
  Type value_;
};

class Expr {
public:
  static Expr Parse(const string &expr);
  Expr(const Number &lhs, const Number &rhs, const Operator &op);

  // Value eval();

  bool operator==(const Expr &other) const;

  ~Expr() = default;

  friend std::ostream &operator<<(std::ostream &os, const Expr &);

private:
  Number lhs_;
  Number rhs_;
  Operator op_;
};

class BindDef {
public:
  static BindDef Parse(const string &text);

  BindDef(const std::string &name, const Expr &expr);

  bool operator==(const BindDef &op) const;

private:
  string name_;
  Expr expr_;
};

static bool is_alphanumeric(char c) { return std::isalpha(c); }

static Str2 extract(const string &text, const string &prefix) {
  if (text.rfind(prefix, 0) == 0) { // pos=0 limits the search to the prefix
    return std::make_pair(prefix, text.substr(prefix.size()));
  } else {
    std::cerr << "Expected: " << prefix << std::endl;
    assert(false);
  }
}

static Str2 extract_identifier(const string &text) {
  // FIXME: First character in id can be an number using that we should not
  // allow it
  return take_whle(text, is_alphanumeric);
}