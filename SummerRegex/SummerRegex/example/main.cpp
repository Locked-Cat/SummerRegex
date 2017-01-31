#include <iostream>
#include "Re.h"

using namespace std;
using namespace summer;

int main()
{
	Re re("[A-Za-z_][A-Za-z_0-9]*-");
	cout << (re.Match("123") == true ? "Match" : "Not match") << endl;
	return 0;
}