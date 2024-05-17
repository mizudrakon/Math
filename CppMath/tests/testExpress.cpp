#include "../expression.h"

int main(void)
{
   value_node vn(0);
   if (vn.getOp() == Op::val)
      print("it is working\n");
      vn.printNode();
      print("\n");
}