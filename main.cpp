//This program evaluates postfix expressions

#include <iostream>
#include <iomanip>
#include "myStack.h"
#include <fstream>

using namespace std;

void evaluateExpression(ifstream& inpF, ofstream& outF, stackType<double>& stack, char& ch, bool& isExpOk);

void evaluateOpr(ofstream& out, stackType<double>& stack, char& ch, bool& isExpOk);

void discardExp(ifstream& in, ofstream& out, char& ch);

void printResult(ofstream& outF, stackType<double> stack, bool& isExpOk);

int main()
{
	bool expressionOk;
	char ch;

	stackType<double> stack(100);
	ifstream infile;
	ofstream outfile;

	infile.open("RpnData.txt");

	if (!infile)
	{
		cout << "Cannot open the input file. " << "Program terminates!" << endl;
		return 1;
	}

	outfile.open("RpnOutput.txt");

	outfile << fixed << showpoint;
	outfile << setprecision(2);

	infile >> ch;

	while (infile)
	{
		stack.initializeStack();
		expressionOk = true;
		outfile << ch;

		evaluateExpression(infile, outfile, stack, ch, expressionOk);

		printResult(outfile, stack, expressionOk);

		infile >> ch; //begin processing the next expression

	}

	infile.close();
	outfile.close();

	return 0;
}

void evaluateExpression(ifstream& inpF, ofstream& outF, stackType<double>& stack, char& ch, bool& isExpOk)
{
	double num;

	while (ch != ';')
	{
		switch (ch)
		{
		case '#': 
			inpF >> num;
			outF << num << " ";
			if (!stack.isFullStack())
				stack.push(num);
			else
			{
				cout << "Stack overflow. " << "Program terminates!" << endl;
				exit(0); 
			}
			break;
		default:
			evaluateOpr(outF, stack, ch, isExpOk);
		}

		if (isExpOk)
		{
			inpF >> ch;
			outF << ch;

			if (ch != '#')
				outF << " ";
		}
		else
		{
			discardExp(inpF, outF, ch);
		}
	}
}

void evaluateOpr(ofstream& out, stackType<double>& stack, char& ch, bool& isExpOk)
{
	double op1, op2;

	if (stack.isEmptyStack())
	{
		out << " (Not enough operands)";
		isExpOk = false;
	}
	else
	{
		op2 = stack.top();
		stack.pop();

		if (stack.isEmptyStack())
		{
			out << " (Not enough operands)";
			isExpOk = false;
		}
		else
		{
			op1 = stack.top();
			stack.pop();

			switch (ch)
			{
			case '+':
				stack.push(op1 + op2);
				break;

			case '-':
				stack.push(op1 - op2);
				break;

			case '*':
				stack.push(op1 * op2);
				break;
				
			case '/':
				if (op2 != 0)
					stack.push(op1 / op2);
				else
				{
					out << " (Division by 0)";
					isExpOk = false;
				}
				break;

			default:
				out << " (Illegal operator)";
				isExpOk = false;
			}
		}
	}
}

void discardExp(ifstream& in, ofstream& out, char& ch)
{
	while (ch != ';')
	{
		in.get(ch);
		out << ch;
	}
}

void printResult(ofstream& outF, stackType<double> stack, bool& isExpOk)
{
	double result;

	if (isExpOk)
	{
		if (!stack.isEmptyStack())
		{
			result = stack.top();
			stack.pop();

			if (stack.isEmptyStack())
				outF << result << endl;
			else
				outF << " (Error: Too many operands)" << endl;
		}
		else
		{
			outF << " (Error in the expression)" << endl;
		}
	}
	else
	{
		outF << " (Error in the expression)" << endl;
	}
	outF << "___________________________" << endl << endl;
}
