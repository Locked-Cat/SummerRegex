#include <iostream>
#include "Re.h"

using namespace std;
using namespace summer;

int main()
{
	{
		cout << "To check whether a string is a legal variable name in C++." << endl;
		Re re("[A-Za-z_][A-Za-z0-9_]*");
		cout << "abc0: " << (re.Match("abc0") == true ? "Match" : "Not match") << endl;
		cout << "_abc0: " << (re.Match("_abc0") == true ? "Match" : "Not match") << endl;
		cout << "0abc: " << (re.Match("0abc") == true ? "Match" : "Not match") << endl;
		cout << endl;
	}

	{
		cout << "To check whether a string is a standard date format." << endl;
		Re re(R"(([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})\-(((0[13578]|1[02])\-(0[1-9]|[12][0-9]|3[01]))|((0[469]|11)\-(0[1-9]|[12][0-9]|30))|(02\-(0[1-9]|[1][0-9]|2[0-8]))))");
		cout << "1993-01-18: " << (re.Match("1993-01-18") == true ? "Match" : "Not match") << endl;
		cout << "19930118: " << (re.Match("19930118") == true ? "Match" : "Not match") << endl;
		cout << endl;
	}

	{
		cout << "To check whether a string is a positive number." << endl;
		Re re(R"([1-9]\d*)");
		cout << "18: " <<  (re.Match("18") == true ? "Match" : "Not match") << endl;
		cout << "-18: " << (re.Match("-18") == true ? "Match" : "Not match") << endl;
		cout << endl;
	}
	

	{
		cout << "To check whether a string is a QQ number." << endl;
		Re re(R"([1-9][0-9]{4,})");
		cout << "1142010471: " << (re.Match("1142010471") == true ? "Match" : "Not match") << endl;
		cout << "123: " << (re.Match("123") == true ? "Match" : "Not match") << endl;
		cout << endl;
	}

	{
		cout << "To check whether a string is a positive floating-point number." << endl;
		Re re(R"([1-9]\d*\.\d*|0\.\d*[1-9]\d*)");
		cout << "3.14: " << (re.Match("3.14") == true ? "Match" : "Not match") << endl;
		cout << "2: " << (re.Match("2") == true ? "Match" : "Not match") << endl;
		cout << endl;
	}

	return 0;
}