#include <hash_fun.h>
#include <iostream>

using namespace std;

int main()
{
    void *p = (void *)(new int(100));
    cout << hash<int>()(123) << endl;
    cout << hash<long>()(123l) << endl;
    cout << hash<string>()(string("Ace")) << endl;
    cout << hash<const char *>()("Ace") << endl;
}