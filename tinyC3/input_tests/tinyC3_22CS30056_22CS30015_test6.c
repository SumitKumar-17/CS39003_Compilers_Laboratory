/*
	Test Cases for (Loops)
*/
int main()
{
	int i, j, k;

	/*
		A nested two for loops to check
	*/
	for (i = 0; i < j; i++){
		for (j = 0; j < k && j < 5; ++j, ++k)
			k = j;
	}

	/*
		A nested while loop to check
	*/
	while (i < j || i < k){
		j--;
	}

	/*
		A nested do-while loop to check
	*/
	do{
		i = k++;
		while (i < j)
			j--;
	} while (k <= 10);

	return 0;
}
