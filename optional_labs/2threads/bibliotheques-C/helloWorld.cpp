#include <iostream>
using namespace std;

int main(){
    cout << "Hello, world!\n" << endl;
    return 0;
}
// strace -e trace=write ./Hello_World_cpp
// strace ./helloWorld_cpp 2>&1 | grep -E 'read|write'