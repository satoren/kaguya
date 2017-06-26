#pragma once

struct A {
  A() : data(32) {}
  A(int v) : data(v) {}
  int data;
};
struct DerivedA : A {};

struct B {
  virtual ~B() {}
};
struct DerivedB : B {};
