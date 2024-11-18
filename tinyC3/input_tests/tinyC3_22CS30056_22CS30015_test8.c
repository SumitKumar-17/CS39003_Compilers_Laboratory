/*
	Test Cases for a (General file)
*/
int loop_addition(int a[], int n){
	int cumulative_sum = 0;
	int i;
	for (i = 0; i < n; i++) cumulative_sum = cumulative_sum+a[i];
	return cumulative_sum;
}

int main(){
	/*
		A function to add elements of an array and return the sum
	*/
	int n = 10;
	int a[n];
	int ans;
	int cumulative_sum = loop_addition(a, n);
	if (cumulative_sum == n){
		ans = 0;
	}
	else if (cumulative_sum > n){
		ans = cumulative_sum - n;
	}
	else{
		ans = 1;
	}
	return 0;
}
