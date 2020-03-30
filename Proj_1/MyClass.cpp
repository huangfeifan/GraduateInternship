#include "pch.h"
#include "MyClass.h"

using namespace std;

double MyClass::Calculate(double x, char oper, double y)
{
	switch (oper) {
	case '+':
		return x + y;
	case '-':
		return x - y;
	case '*':
		return x * y;
	case '/':
		/*
		if (y == 0)
			cout << "error";
		else
		*/
		return x / y;
	default:
		return 0.0;
	}
}
