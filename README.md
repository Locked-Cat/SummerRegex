# SummerRegex

这就是一个傻逼一般的**基本**正则表达式引擎，你能想到的功能它可能都不支持/(ㄒoㄒ)/~，目前的实现好像还和标准语法有点不兼容(⊙﹏⊙)b。

实现思路上参考了[vczh](http://www.cppblog.com/vczh/archive/2008/05/22/50763.html)以前的工作。

基本用法差不多长这样：

```C++
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
```

效果大概是这样的：
![test](https://github.com/Locked-Cat/SummerRegex/blob/master/test.png)

下一步计划：增加捕获与正向/反向预查功能。
