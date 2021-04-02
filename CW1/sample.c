#include <stdio.h>

int weighted_sensing(int left, int centre, int right){
  float readings[3] = {left, centre, right};
  float proportional[3];
  for (int i = 0; i < 3; i++) {
    float I_total = left+centre+right;
    float temp = (readings[i]/I_total);
    proportional[i] = temp;
    printf("%f\n", proportional[i]);
}
}
int main() {
    weighted_sensing(250,300,200);
}


// #define array_length 5

// int main() {
// 	int before_array[array_length], array_n[array_length], i, temp;
	
//     for(i=0; i<array_length; i++){
//         before_array[i] = 200;
//     }
// 	for(i = 0; i < array_length; i++) {
// 		printf("array_n[%d] = %d\n", i, before_array[i]);
// 	}
// 	// printf("Input the 5 members of the array:\n");
// 	// for(i = 0; i <  array_length; i++) {
// 	// 	scanf("%d", &array_n[i]);
// 	// }

// 	for(i = 0; i < array_length; i++) {
// 		if(i < (array_length)) {
// 			temp = array_n[i];
// 			array_n[i] = array_n[array_length-(i+1)];
// 			array_n[array_length-(i+1)] = temp;
// 		}
// 	}

// 	return 0;
// }