/*
	Test Cases for (Declarations and assignments)
*/
int g1, g2;
char g3 = 'a';

/*
	A function to check the declarations and assignments
*/
void foo(int a, int b);
void foo(int a, int b){
	int i = a + b;
	return;
}

int main(){
	/*
		Checking the array,pointer,string,array assignment her 
	*/
	float f1 = 2.3, f2;
	int n1, n2 = 10, *n3;
	char c1, c2[80];
	char *str1 = "Hello World", *str2;

	n1 = n2 = f1 = f2;
	c2[22] = c1;

	return 0;
}
