#include <iostream>
#include "serialization/serializable.hh"

struct test : public serialization::Serializable{
  /*
   * some member and default ctor for test
   */
  int		i1;
  int		i2;

  float		f1;
  float		f2;

  std::list<int>	l1;
  std::list<test*>	l2;
  test() : i1(1), i2(2), f1(1.1), f2(2.2) { }

  void	log_self() {
    std::cout
      << i1 << " "
      << i2 << " "
      << f1 << " "
      << f2 << " "
      << std::endl;

    std::cout << "[ ";
    std::for_each(l1.begin(), l1.end(), [] (int i) { std::cout << i << " "; });
    std::cout << "]\n";

    std::cout << "[ ";
    std::for_each(l2.begin(), l2.end(), [] (test* i) { i->log_self(); std::cout << "\n"; });
    std::cout << "]\n";
  }

  /*
   * stringifiable part
   */
  EASILY_SERIALIZABLE(test
		      ,: test()
		      , {
      __serial["i1"] & i1;
      __serial["i2"] & i2;

      __serial["f1"] & f1;
      __serial["f2"] & f2;

      __serial["l1"] & l1;
      __serial["l2"] & l2;
    });
};
REGISTER_FOR_SERIALIZATION(test);

int main() {
  test* t = new test;

  {
    serialization::Serial	json;

    json & t;
    std::cout << "no modification:\n" << json.Stringify() << std::endl;
  }

  t->i1 += 11;
  t->i2 += 22;

  t->f1 += 1.01;
  t->f2 += 2.02;

  t->l1.emplace_back(5);
  t->l1.emplace_back(5);
  t->l1.emplace_back(5);
  t->l1.emplace_back(5);
  t->l1.emplace_back(5);
  t->l2.emplace_back(new test);
  {
    serialization::Serial	json;

    json & t;
    std::cout << "with modification:\n" << json.Stringify() << std::endl;

    dynamic_cast<test*>(serialization::Serializable::Instantiate( json ))->log_self();
  }
}
