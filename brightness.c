//------------------------------------
// Standard C Library Includes
//------------------------------------
#include <stdio.h>



//------------------------------------
// External Library Includes
//------------------------------------
#include <pnmrdr.h>



//------------------------------------
// Custom Library Includes
//------------------------------------
// No custom libraries to include for brightness.c


 
//------------------------------------
// Function Headers
//------------------------------------
static FILE* open_file(char* filename);



//------------------------------------
// Funtion Definitions
//------------------------------------

/** open_file() 
* Description:
* A function to open a file and return the appropriate file 
*
* Inputs:
* @param filename: the file the function will attempt to open
*
* Returns:
* A file pointer to the file described by the filename parameter
* or a null pointer if the file did not exist
*
* Side Effects:
* In the event of a non existant file this function will:
*     1. Print an appropriate error message
*/
static FILE* open_file(char* filename) {
	FILE* fp = fopen(filename, "rb");
	
	if(fp == NULL) {
		printf("Failed to open file: %s\n", filename);
		return fp;
	}	
	
	printf("%s opened successfully\n", filename);

	return fp;
}


//------------------------------------
// Main Function
//------------------------------------
int main(int argc, char * argv[]) {
	Pnmrdr_T image_ptr;
	FILE* fp = NULL;
	if(argc >= 2) {
		fp = open_file(argv[1]);
		if(!fp) return 1;
		image_ptr = Pnmrdr_new(fp);
	}
	else {
		image_ptr = Pnmrdr_new(stdin);
	}

	Pnmrdr_mapdata image_data = Pnmrdr_data(image_ptr);
	//printf("The image is %d wide and %d tall and is of type %d with denom %d\n", image_data.width, image_data.height, image_data.type, image_data.denominator);
	
	long double total = 0;
	for(unsigned int i = 0 ; i < image_data.width ; i++) {
		for(unsigned int j = 0 ; j < image_data.height ; j++) {
			unsigned int brightness = Pnmrdr_get(image_ptr);
			//printf("count: %d brightness: %f\n", i * image_data.width + j, brightness / 255.0);
			total += (double) (brightness / 255.0);

		}
	}

//	printf("total: %Lf percentage: %f\n", total, (double) (total / (image_data.width * image_data.height)));
	printf("%f\n", (double) (total / (image_data.width * image_data.height)));

	if(fp) fclose(fp);
	Pnmrdr_free(&image_ptr);
	return 0;
}