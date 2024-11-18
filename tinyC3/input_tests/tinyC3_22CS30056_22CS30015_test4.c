/*
	Test Cases for (Arrays and pointers)
*/
int main(){
	/*
		Checking the array,pointer,double pointer, triple pointer,string,array assignment here
	*/
	int a1[10], a2[10][20], a3[5][10][15];
	float *f1, **f2, ***f3;

	a1[5] = a2[1][2];
	a2[5][6] = a3[1][2][3];
	a3[0][0][0] = ***f3;
	***f3 = **f2;
	**f2 = a2[9][19];
	*f1 = **f2;
	return 0;
}
