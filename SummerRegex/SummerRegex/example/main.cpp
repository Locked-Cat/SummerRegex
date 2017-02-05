#include <iostream>
#include "Re.h"

using namespace std;
using namespace summer;

int main()
{
	Re re("a(ab)*");
	cout << (re.Match("aabab") == true ? "Match" : "Not match") << endl;
	return 0;
}