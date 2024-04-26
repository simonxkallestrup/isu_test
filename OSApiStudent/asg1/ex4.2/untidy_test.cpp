#include <iostream>
using namespace std;

const int GlobalConst = 10;

int FunctionNotInCamelBack(int v1,int v2){
  return v1+v2;
}

int main(){

  cout << "GlobalConst er: " << GlobalConst << endl;
  return 0;
}