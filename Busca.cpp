#include <bits/stdc++.h>
#include <sys/resource.h>
#include <sys/time.h>
#define pb push_back

using namespace std;


// ____________ QUICK SORT ___________


int partition(int arr[], const int left, const int right) {
	const int mid = left + (right - left) / 2;
	const int pivot = arr[mid];

	swap(arr[mid], arr[left]);
	int i = left + 1;
	int j = right;

	while (i <= j) {
		while(i <= j && arr[i] <= pivot) i++;

		while(i <= j && arr[j] > pivot) j--;

		if (i < j) swap(arr[i], arr[j]);
	}

	swap(arr[i - 1],arr[left]);
	return i - 1;
}

void quick_sort(int arr[], const int left, const int right){

	if (left >= right) return;

	int part = partition(arr, left, right);

	quick_sort(arr, left, part - 1);
	quick_sort(arr, part + 1, right);
}


// ___________ R A D I X  ______________


int max_element (int arr[], int n) {
	int max = 0;
	for (int i = 0; i < n; ++i) if (arr[i] > max) max = arr[i];
	return max;
}

void count_sort(int arr[], int n, int exp) {
	int output[n], i, count[10] = {0};

	for (i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;

	for (i = 1; i < 10; i++) count[i] += count[i-1];

	for (i = n - 1; i >= 0; i--) {
		output[count[(arr[i] / exp) % 10] - 1] = arr[i];
		count[(arr[i] / exp) % 10]--;
	}

	for (i = 0; i < n; i++) arr[i] = output[i];
}

void radix_sort(int arr[], int n) {
	int m = max_element(arr, n);
	for (int exp = 1; m/exp > 0; exp *= 10) count_sort(arr, n, exp);
}


// ____________ M E R G E ______________

void merge(int arr[], int left_low, int left_high, int right_low, int right_high) { 
	int length = right_high-left_low+1;
	int temp[length];
	int left = left_low;
	int right = right_low;
	for (int i = 0; i < length; ++i) { 
		if (left > left_high) temp[i] = arr[right++];
		else if (right > right_high) temp[i] = arr[left++];
		else if (arr[left] <= arr[right]) temp[i] = arr[left++];
		else temp[i] = arr[right++]; 
	}

	for (int i=0; i< length; ++i) arr[left_low++] = temp[i];
}

void merge_sort(int arr[], int low, int high) {
	if (low >= high) return;
	else {
		int mid = (low + high)/2;
		merge_sort(arr, low, mid);
		merge_sort(arr, mid+1, high);
		merge(arr, low, mid, mid+1, high);
	}
}

// ___________ FUNÇAO UTILITARIA PARA CALCULAR TEMPO ___________


double calc_time (const struct rusage *b, const struct rusage *a) {
	if (b == NULL || a == NULL)
		return 0;
	else
		return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
			(b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
	((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
		(b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
	/ 1000000.0);
}


void generator (int vector_size) {

	struct rusage init_time, end_time;
	double time_qsort = 0, time_msort = 0, time_rsort = 0, time_generate_vec = 0;

	int *a = new int [vector_size], *b = new int [vector_size], *c = new int [vector_size], x;

	getrusage(RUSAGE_SELF, &init_time);
		for (int i = 0; i < vector_size; ++i) {
			x = rand() % (vector_size * 5);
			a[i] = x;
			b[i] = x;
			c[i] = x;
		}
	getrusage(RUSAGE_SELF, &end_time);

	time_generate_vec = calc_time (&init_time, &end_time);

	getrusage(RUSAGE_SELF, &init_time);
		quick_sort (a, 0, vector_size - 1);
	getrusage(RUSAGE_SELF, &end_time);
	
	time_qsort = calc_time (&init_time, &end_time);

	getrusage(RUSAGE_SELF, &init_time);
		radix_sort (b, vector_size);
	getrusage(RUSAGE_SELF, &end_time);
	
	time_rsort = calc_time (&init_time, &end_time);	


	getrusage(RUSAGE_SELF, &init_time);
		merge_sort (c, 0, vector_size - 1);
	getrusage(RUSAGE_SELF, &end_time);
	
	time_msort = calc_time (&init_time, &end_time);	


	printf ("With %d elements:\n\n"
			"the computer took %.2lf seconds to generate the array\n"
			"quick sort took %.2lf seconds to sort the array\n"
			"radix sort took %.2lf seconds to sort the array\n"
			"merge sort took %.2lf seconds to sort the array\n\n\n", 
			vector_size, time_generate_vec, time_qsort, time_rsort, time_msort);

	delete a;
	delete b;
	delete c;
}


int main () {
	srand (time(NULL));

	int n;
	printf ("insert array size (0 to leave): ");
	while (scanf("%d", &n), n) generator (n);

	return 0;
}