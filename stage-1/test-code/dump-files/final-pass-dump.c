;; Function foo (foo, funcdef_no=0, decl_uid=2337, cgraph_uid=1, symbol_order=0)

===== Basic block count: 1 =====
----- Statement count: 1 -----
_3 = p1_1(D) * p2_2(D);
===== Basic block count: 2 =====
----- Statement count: 2 -----
<L0>:
----- Statement count: 3 -----
# VUSE <.MEM_4(D)>
return _3;
------------------------------------
Total Basic Blocks: 2
Total Gimple Statements: 3
------------------------------------

int foo (int p1, int p2)
{
  int D.2345;
  int _3;

  <bb 2> :
  _3 = p1_1(D) * p2_2(D);

  <bb 3> :
<L0>:
  return _3;

}



;; Function main (main, funcdef_no=1, decl_uid=2339, cgraph_uid=2, symbol_order=1)

===== Basic block count: 1 =====
----- Statement count: 1 -----
a_1 = 12;
----- Statement count: 2 -----
b_2 = 13;
----- Statement count: 3 -----
# .MEM_4 = VDEF <.MEM_3(D)>
c_5 = foo (a_1, b_2);
----- Statement count: 4 -----
# .MEM_6 = VDEF <.MEM_4>
printf ("%d\n", c_5);
----- Statement count: 5 -----
_7 = 0;
===== Basic block count: 2 =====
----- Statement count: 6 -----
<L0>:
----- Statement count: 7 -----
# VUSE <.MEM_6>
return _7;
------------------------------------
Total Basic Blocks: 2
Total Gimple Statements: 7
------------------------------------

int main ()
{
  int c;
  int b;
  int a;
  int D.2347;
  int _7;

  <bb 2> :
  a_1 = 12;
  b_2 = 13;
  c_5 = foo (a_1, b_2);
  printf ("%d\n", c_5);
  _7 = 0;

  <bb 3> :
<L0>:
  return _7;

}
