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
   print("default expression: {}, eone: {}\n", defexp.str(), eone.str());
   ++defexp; // 0 += 1 = 1
   print("def += 1: {}\n",defexp.str());
   defexp*=9; // 1 *= 9 = 9
   print("op*= 9: {}\n{} ",defexp.str(),eone.str());
   eone*=3; // 1 *= 3 = 3
   print("op*= 3 : {}\n{} ",eone.str(),defexp.str());
   defexp--; // 9 -= 1 = 8
   print("-= 1: {}\n",defexp.str(), eone.str());
   defexp /= 4; // 8 /= 4 = 2 defexp
   print("/= 4: {}\n",defexp.str());
   print("\nTESTING NONMEMBER + and -:\n");
   auto a = eone + 9; // 3 + 9 = 12
   print("{} + 9: {}\n\n",eone.str(),a.str());
   defexp = a - 2; // 12 - 2 = 10 dest 2
   print("{} - 2: {}\n\n",a.str(),defexp.str());
   eone /= 4;
   print("3/=4 : {}\n", eone.str());
   eone += 5;
   print("3/4 + 5 : {}\n", eone.str());
   eone += 3;
   print("(3/4 + 5) + 3 : {}\n", eone.str());

}