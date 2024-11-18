/**
 * 	    C program to test the parser for the tinyC language
 *      Sumit Kumar  - 22CS30056
 *      Aviral Singh - 22CS30015
 */

// Global declarations
extern int global_var;
static int static_var = 42;
const int const_var = 100;
inline int inline_func(int x) {
    return x * x;
}

// A compute function to perform some computation
inline void compute(int *restrict arr, int size, volatile int multiplier, const char *message[const 5]) {
    static int result = 0;
    extern int total;
    auto int temp;

    for (int i = 0; i < size; i++) {
        temp = arr[i] * multiplier;
        result += temp;
    }
    printf("%s: Computed Total = %d\n", message[0], result);
}

const double *get_values(const double *values[static 3], int count, ...) {
    return values[0];
}

inline const float modify_data(char *restrict name, float values) {
    return values;
}

char * const *get_names(const char *const names[const 5], int count, ...) {
	return names;
}

// Function to modify the sender data
inline const long unsigned int ***modify_unsigned_sender_data(const char *restrict rec, char **restrict send);

//Start the signal setup
void setup_signal(){}

//Function to get the line
_Bool get_line(_Bool debug,char *s,char *prompt){
	if(debug){
		printf("%s",prompt);
	}else{
		char *query_str=get_names((const char){"Enter the string: "},1);
		if(query_str){
			strcpy(s,query_str);
			return 1;
		}
	}
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sort_array(int arr[], int length) {
    for (int i = 0; i < length - 1; i++) {
        for (int j = 0; j < length - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void initialize_array(int arr[], int length) {
    for (int i = 0; i < length; i++) {
        arr[i] = rand() % 100;
    }
}

// Function to test various operators
void testFunction(int param1, float param2) {
    // Function body with various operators
    int result = param1 + (int)param2;
    result -= param1;
    result *= 2;
    result /= 1;
    result %= 3;
    result <<= 1;
    result >>= 1;
    result &= 1;
    result |= 2;
    result ^= 3;

    // Unary operators
    result = +param1;
    result = -param2;
    result = !param1;
    result = ~param1;
    result = *(&param1);
    result = sizeof(param1);

    // Conditional operator
    result = (param1 > param2) ? param1 : (int)param2;

    // Inline function call
    result = inline_func(result);

    // Pointer operations
    int *p = &result;
    *p = 42;

    printf("Result: %d\n", result);
}

//The main function
signed main() {
    int values[10];
    char greeting[] = "Hello, World!";
    char *ptr = greeting;
    char *list[] = {"Apple", "Banana", "Cherry", "Date", "Sumit"};

    unsigned long x = 1234567890;
    float pi = 3.14159;
    double euler = 2.718281828459045;
    _Bool flag = 1;

	long int test_var;
	test_var=(unsigned int )c;
	test_var=sizeof(long double);
	test_var=sizeof test_var;

    auto int auto_var = 1;
    register int reg_var = 2;
    int int_var = 3;
    long long_var = 4;
    short short_var = 5;
    unsigned int u_int_var = 6;
    signed int s_int_var = -7;
    double double_var = 8.0;
    float float_var = 9.0;
    char char_var = 'C';
    _Bool bool_var = 1;    // C99 Bool keyword
    double _Complex complex_var = 1.0 ; // C99 Complex
    double _Imaginary imag_var = 3.0; // C99 Imaginary

    //Pointer Declarations
    int *pInt = &int_var;
    int **ppInt = &pInt;
    float *pFloat = &float_var;
    // Function call
    testFunction(int_var, float_var);


    initialize_array(values, 10);
    sort_array(values, 10);
    compute(values, 10, 2, list);

    for (int i = 0; i < 10; i++) {
        printf("values[%d] = %d\n", i, values[i]);
    }

    printf("%s\n", ptr);

    if (flag) {
        for (int i = 0; i < 10; i++) {
            if (i % 2 == 0) {
                printf("Even index: %d\n", i);
            } else {
                printf("Odd index: %d\n", i);
            }
        }
    } else {
        printf("Flag is not set.\n");
    }

    while (x > 0) {
        x--;
        if (x % 100000000 == 0) {
            printf("Countdown: %f\n", x);
        }
    }

    switch (flag) {
    case 1:
        printf("Flag is true.\n");
        break;
    case 0:
        printf("Flag is false.\n");
        break;
    default:
        printf("Unexpected flag value.\n");
    }

    //Testing swap function
    int a = 5, b = 10;
    printf("Before swap: a = %d, b = %d\n", a, b);
    func_ptr(&a, &b);
    printf("After swap: a = %d, b = %d\n", a, b);

    // Testing variable-length arrays
    int n = 5;
    int var_len_arr[n];
    for (int i = 0; i < n; i++) {
        var_len_arr[i] = i * 2;
        printf("var_len_arr[%d] = %d\n", i, var_len_arr[i]);
    }

    // Loop to generate numbers
    for (int i = 0; i < 10;) {
        if (i % 3 == 0) {
            printf("Divisible by 3: %d\n", i);
        }
    }

    return 0;
}
