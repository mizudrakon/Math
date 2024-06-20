#include "../expression.h"

int main(void)
{
   print("testing value nodes:\n");
   value_node vn(0);
   if (vn.getOp() == Op::val)
      print("it is working\n");
      print("{}",vn.str());
      print("\n\n");
   
   print("testing operation nodes:\n");
   op_node o_plus(Op::plus);
   print("operation is {}\n\n", o_plus.str());


   print("testing expression:\n");
   expression defexp{};
   expression eone{1};
   print("default expression: {}, eone: {}\n", defexp, eone);
   ++defexp; // 0 += 1 = 1
   print("0 += 1: {} -> {}\n",defexp, "{1}" == defexp.str());
   defexp*=9; // 1 *= 9 = 9
   print("op*=:\n1 *= 9: {} -> {}\n", defexp, "{9}" == defexp.str());
   eone*=3; // 1 *= 3 = 3
   print("1 *= 3 : {} -> {}\n",eone, "{3}" == eone.str());
   defexp--; // 9 -= 1 = 8
   print("9 -= 1: {}\n",defexp, defexp.str() == "{8}");
   defexp /= 4; // 8 /= 4 = 2 defexp
   print("op/=:\n8 /= 4: {}\n",defexp.str());
   print("\nTESTING NONMEMBER + and -:\n");
   auto a = eone + 9; // 3 + 9 = 12
   print("{} + 9: {} -> {}\n\n",eone.str(),a, a.str() == "{12}");
   defexp = a - 2; // 12 - 2 = 10 dest 2
   print("{} - 2: {} -> {}\n\n",a.str(), defexp,defexp.str() == "{10}");
   eone /= 4;
   print("3/=4 : {} -> {}\n", eone.str(), "{3}/{4}" == eone.str());
   eone *= 2;
   print("3/4 *= 2 : {} -> {}\n", eone, "{6}/{4}" == eone.str());
   eone += 5;
   print("6/4 + 5 : {} -> {}\n", eone, "{6}/{4}+{5}" == eone.str());
   eone += 3;
   print("(6/4 + 5) + 3 : {} -> {}\n", eone, "{6}/{4}+{8}" == eone.str());
   eone *= 2;
   print("(6/4 + 8) * 2 : {} -> {}\n", eone, "{12}/{4}+{16}" == eone.str());
   eone /= 8;
   print("(12/4) + 16) / 8 : {} -> {}\n", eone, "{12}/{32}+{2}" == eone.str());
}