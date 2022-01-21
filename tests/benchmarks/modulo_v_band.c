#include <stdio.h>
#include <time.h>

float tick() {return (float)clock() / CLOCKS_PER_SEC;}
int main() {
	int n = 10000000;
	int s = 1000;
	{
		float arr[s];
		int arr_size = 0;
		
		for (int i = 0; i < s; i++) {
			float st = tick();
			for (int i = 0; i < n; i++) {
				int x = (i & 255 + 255) & 255;
			}
			arr[arr_size++] = tick() - st;
		}
		
		float sum = 0;
		for (int i = 0; i < arr_size; i++) {
			sum += arr[i];
		}
		sum /= arr_size;
		printf("%lf\n", sum);
	}
	
	{
		float arr[s];
		int arr_size = 0;
		
		for (int i = 0; i < s; i++) {
			float st = tick();
			for (int i = 0; i < n; i++) {
				int x = (i % 256 + 256) % 256;
			}
			arr[arr_size++] = tick() - st;
		}
		
		float sum = 0;
		for (int i = 0; i < arr_size; i++) {
			sum += arr[i];
		}
		sum /= arr_size;
		printf("%lf\n", sum);
	}
	
	return 0;
}