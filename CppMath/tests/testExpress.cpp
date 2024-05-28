#include "../expression.h"

int main(void)
{
   print("testing value nodes:\n");
   value_node vn(0);
   if (vn.getOp() == Op::val)
      print("it is working\n");
      print("{}",vn.str());
      print("\n");
   
   print("testing operation nodes:\n");
   op_node o_plus(Op::plus);
   print("operation is {}\n", o_plus.str());


   print("testing expression:\n");
   expression defexp{};
   expression eone{1};
   print("default expression: {}, eone: {}\n", defexp.str(), eone.str());
}