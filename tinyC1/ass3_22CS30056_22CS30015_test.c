#include <stdio.h>

//This is the testing file for the lexer
//Roll No: 22CS30056 Sumit Kumar 
//Roll No: 22CS30015 Aviral Singh


int main() {
    // Test keywords
    int x = 100;                  // 'int' is a keyword, 'x' is an identifier
    float y = 3.14;               // 'float' is a keyword, 'y' is an identifier
    char z = 'c';                 // 'char' is a keyword, 'z' is an identifier
    double pi = 3.14159e-10;      // 'double' is a keyword
    unsigned long n = 123456789;  // 'unsigned' and 'long' are keywords

    // Test identifiers
    int myVar1 = 10;              // 'myVar1' is an identifier
    float myFloat = 5.0f;         // 'myFloat' is an identifier
    char myChar = 'A';            // 'myChar' is an identifier

    // Test integer constants
    int a = 0;                    // Integer constant (zero)
    int b = 12345;                // Integer constant (non-zero)

    // Test float constants
    float c = 1.23;               // Simple float constant
    float d = 1.23e+4;            // Exponential notation

    // Test char constants
    char e = 'e';                 // Simple char constant
    char f = '\n';                // Escape sequence in char constant

    // Test string literals
    char *str1 = "Hello, World!";   // Simple string literal
    char *str2 = "Line 1\nLine 2";  // String literal with escape sequence

    // Test punctuators
    if (x <= y && z != e) {
        x++;                      // '++' is a punctuator
        y -= 0.1;                 // '-=' is a punctuator
    }

    // Test line comments
    // This is a single-line comment

    // Test block comments
    /*
        This is a block comment.
        It spans multiple lines.
    */

    // Test escape sequences in strings and chars
    char newline = '\n';           // Newline escape sequence
    char tab = '\t';               // Tab escape sequence
    printf("Newline and tab:\n\tEnd of test\n");

    return 0;
}
