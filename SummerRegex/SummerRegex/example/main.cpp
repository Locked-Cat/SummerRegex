#include <iostream>
#include "Re.h"

using namespace std;
using namespace summer;

int main()
{
	Re re("(a)|(c)");
	cout << (re.Match("aababc") == true ? "Match" : "Not match") << endl;
	auto result = re.Search("xyzaabababababacd");
	cout << (result.success ? "Success" : "Not success") << endl;
	cout << result.value << endl;


	return 0;
}