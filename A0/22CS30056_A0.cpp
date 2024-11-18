#include <iostream>
#include <string>
using namespace std;

/* Assignment-0
Name-Sumit Kumar
Roll No.22CS30056
*/

class Calculator
{
public:
    string expression;
    size_t pos;
    Calculator(const string &expr) : expression(expr), pos(0){};

    void skipspaces()
    {
        while (pos < expression.length() && isspace(expression[pos]))
            ++pos;
    }

    int evaluate()
    {
        int result = evalsum();

        if (pos < expression.length())
        {
            throw invalid_argument("wrong length of the expression ");
        }
        return result;
    }

    int evalsum()
    {
        int result = evalterm();
        skipspaces();
        while (pos < expression.length() && (expression[pos] == '+' || expression[pos] == '-'))
        {
            char op = expression[pos++];
            int term = evalterm();
            skipspaces();
            if (op == '+')
                result += term;
            else
                result -= term;
        }
        return result;
    }

    int evalterm()
    {
        int result = evalfactor();
        skipspaces();
        while (pos < expression.length() && expression[pos] == '*')
        {
            ++pos;
            int factor = evalfactor();
            skipspaces();
            result *= factor;
        }
        return result;
    }

    int evalfactor()
    {
        skipspaces();
        int result;
        if (pos < expression.length() && expression[pos] == '(')
        {
            ++pos;
            result = evalsum();
            skipspaces();
            if (pos >= expression.length() || expression[pos] != ')')
            {
                throw invalid_argument("write correct expression ");
            }
            ++pos;
        }
        else
        {
            size_t startPos = pos;
            while (pos < expression.length() && isdigit(expression[pos]))
            {
                ++pos;
            }
            if (startPos == pos)
            {
                throw invalid_argument("wrong expression");
            }
            result = stoi(expression.substr(startPos, pos - startPos));
        }
        skipspaces();
        return result;
    }
};

int main()
{
    string expression;

    cout << "Enter an expression: ";
    getline(cin, expression);

    cout << "You entered: " << expression << endl;

    try
    {
        Calculator calc(expression);
        int result = calc.evaluate();
        cout << "The expression evaluates to " << result << endl;
    }
    catch (const invalid_argument &e)
    {
        cout << "error caught" << e.what() << endl;
    }

    return 0;
}
