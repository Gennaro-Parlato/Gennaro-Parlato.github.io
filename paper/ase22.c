// Save this file into: /home/cbmc-ssm/Documents/sm_example.c
// Open a Terminal and run with: /home/cbmc-ssm/Documents/bin/cbmc-ssm --unwind 1 --no-unwinding-assertions --32 /home/cbmc-ssm/Documents/sm_example.c --stop-on-fail --copy-shadow-memory
// No assertions should fail

int gd1;

void fnc(int arg){
  // remember: arg is a copy of main's d2
  assert(__CPROVER_get_field(&arg, "f1") == 4);
}

int main(){
  typedef struct {
    int s1;
    float s2;
    struct str* s3;
  } str;
  str s, t, u;

  int d2;
  float f1;
  float f2;
  int a[5];

  // Shadow memory field declarations
       // __CPROVER_field_decl_global is used only for global memory locations
  __CPROVER_field_decl_global("f1", (unsigned char) 0);
       // __CPROVER_field_decl_global is used only for local memory locations
  __CPROVER_field_decl_local("f1", (unsigned char) 0);

  __CPROVER_field_decl_global("f2", (_Bool) 0);
  __CPROVER_field_decl_local("f2", (_Bool) 0);

  // read shadow memory: everything is 0, now
  assert(__CPROVER_get_field(&gd1, "f1") == 0);
  assert(__CPROVER_get_field(&d2, "f1") == 0);
  assert(__CPROVER_get_field(&f1, "f1") == 0);
  assert(__CPROVER_get_field(&f2, "f1") == 0);
  assert(__CPROVER_get_field(&s, "f1") == 0);
  assert(__CPROVER_get_field(&(s.s1), "f1") == 0);
  assert(__CPROVER_get_field(&(s.s2), "f1") == 0);
  assert(__CPROVER_get_field(&(s.s3), "f1") == 0);
  assert(__CPROVER_get_field(&gd1, "f2") == 0);
  assert(__CPROVER_get_field(&d2, "f2") == 0);
  assert(__CPROVER_get_field(&f1, "f2") == 0);
  assert(__CPROVER_get_field(&f2, "f2") == 0);
  assert(__CPROVER_get_field(&s, "f2") == 0);
  assert(__CPROVER_get_field(&(s.s1), "f2") == 0);
  assert(__CPROVER_get_field(&(s.s2), "f2") == 0);
  assert(__CPROVER_get_field(&(s.s3), "f2") == 0);

  // set d2's SM field f1 to 4
  __CPROVER_set_field(&d2, "f1", 4);

  // the SM is unchanged for the other memory locations and fields
  assert(__CPROVER_get_field(&gd1, "f1") == 0);
  assert(__CPROVER_get_field(&d2, "f1") == 4); // this is the only updated SM field
  assert(__CPROVER_get_field(&f1, "f1") == 0);
  assert(__CPROVER_get_field(&f2, "f1") == 0);
  assert(__CPROVER_get_field(&s, "f1") == 0);
  assert(__CPROVER_get_field(&(s.s1), "f1") == 0);
  assert(__CPROVER_get_field(&(s.s2), "f1") == 0);
  assert(__CPROVER_get_field(&(s.s3), "f1") == 0);
  assert(__CPROVER_get_field(&gd1, "f2") == 0);
  assert(__CPROVER_get_field(&d2, "f2") == 0);
  assert(__CPROVER_get_field(&f1, "f2") == 0);
  assert(__CPROVER_get_field(&f2, "f2") == 0);
  assert(__CPROVER_get_field(&s, "f2") == 0);
  assert(__CPROVER_get_field(&(s.s1), "f2") == 0);
  assert(__CPROVER_get_field(&(s.s2), "f2") == 0);
  assert(__CPROVER_get_field(&(s.s3), "f2") == 0);

  // we now consider structs 
  __CPROVER_set_field(&(s.s2), "f1", 5);
  __CPROVER_set_field(&(s.s1), "f1", 2);
  // we access struct fields SM alike regular variables
  assert(__CPROVER_get_field(&(s.s1), "f1") == 2);
  assert(__CPROVER_get_field(&(s.s2), "f1") == 5);
  assert(__CPROVER_get_field(&(s.s3), "f1") == 0);
  // we can get the SM of the entire structure: it returns the maximum of all fields SM (here, 2, 5, and 0: 5)
  assert(__CPROVER_get_field(&(s), "f1") == 5);
  
  // this is an example where access the memory location using its address 
  str* sp = &u;
  assert(__CPROVER_get_field(&(sp->s1), "f1") == 0);
  assert(__CPROVER_get_field(&(sp->s2), "f1") == 0);
  assert(__CPROVER_get_field(&(sp->s3), "f1") == 0);
  sp = &s;
  assert(__CPROVER_get_field(&(sp->s1), "f1") == 2);
  assert(__CPROVER_get_field(&(sp->s2), "f1") == 5);
  assert(__CPROVER_get_field(&(sp->s3), "f1") == 0);

  // struct and union variable copies, copy also the SM
  t = s;
  assert(__CPROVER_get_field(&(t.s1), "f1") == 2);
  assert(__CPROVER_get_field(&(t.s2), "f1") == 5);
  assert(__CPROVER_get_field(&(t.s3), "f1") == 0);

  // we now treat arrays: for individual elements, SM works alike struct fields.
  __CPROVER_set_field(&(a[1]), "f1", 7);
  assert(__CPROVER_get_field(&(a[1]), "f1") == 7);
  assert(__CPROVER_get_field(&(a[2]), "f1") == 0);
  // unlike structs, reading the SM associated to an array returns the SM associated to the first element.
  assert(__CPROVER_get_field((int*)a, "f1") == __CPROVER_get_field(&(a[0]), "f1")); // this assert never fails, in any code
  // basically, the array is treated exactly as a pointer to its first element.

  // function calls: the SM status of the arguments is copied to the parameters at every invocation
  fnc(d2);
  return 0;
}
