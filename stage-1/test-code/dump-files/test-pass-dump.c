;; Function foo (foo, funcdef_no=0, decl_uid=3929, cgraph_uid=1, symbol_order=0)

=== FUnction 1 Name 'printf' ===
=== FUnction 2 Name 'main' ===
=== FUnction 3 Name 'foo' ===


#### End amullagaliev diagnostics, start regular dump of current gimple ####


int foo (int p1, int p2)
{
  int D.3937;
  int _3;

  <bb 2> :
  _3 = p1_1(D) * p2_2(D);

  <bb 3> :
<L0>:
  return _3;

}



;; Function main (main, funcdef_no=1, decl_uid=3931, cgraph_uid=2, symbol_order=1)

=== FUnction 1 Name 'printf' ===
=== FUnction 2 Name 'main' ===
=== FUnction 3 Name 'foo' ===


#### End amullagaliev diagnostics, start regular dump of current gimple ####


int main ()
{
  int c;
  int b;
  int a;
  int D.3939;
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
