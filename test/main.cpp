#include <dlfcn.h>
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

typedef void(*function)(void*, void*, void*);

#define TO_VOID(o) reinterpret_cast<void*>(o)


int main() {
  void *handle;
  vector<function> fnc;
  size_t (*fncSize)();
  function *fncPtr;

  handle = dlopen("./libtest.so", RTLD_LAZY);
  if(!handle) {
    fputs(dlerror(), stderr);
    exit(1);
  }

  fncPtr = (function*)dlsym(handle, "FunctionArray");
  fncSize = (size_t(*)())dlsym(handle, "FunctionSize");

  for (size_t i = 0; i < fncSize(); i++) {
    fnc.push_back(fncPtr[i]);
  }

  vector<double> *x = new vector<double>();
  vector<double> *p = new vector<double>();
  vector<double> *y = new vector<double>();

  x->push_back(1.0);
  x->push_back(2.0);
  p->push_back(3.0);
  p->push_back(4.0);

  fnc[0](TO_VOID(y), TO_VOID(x), TO_VOID(p));

  cout << (*y)[0] << endl << (*y)[1] << endl << (*y)[2] << endl;
  return 0;
}
