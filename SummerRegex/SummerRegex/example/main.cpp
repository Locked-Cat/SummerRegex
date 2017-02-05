#include <iostream>
#include "Re.h"

using namespace std;
using namespace summer;

int main()
{
	Re re("a(ab)*c");
	cout << (re.Match("aababc") == true ? "Match" : "Not match") << endl;
	return 0;
}