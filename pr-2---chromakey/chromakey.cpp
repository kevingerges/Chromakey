
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "bmplib.h"

using namespace std;


void method1(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE],
	     double threshold);

void method2(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE]);

void replace(bool mask[][SIZE], 
	     unsigned char inImage[][SIZE][RGB],
	     unsigned char bgImage[][SIZE][RGB],
	     unsigned char outImage[][SIZE][RGB]);

int main(int argc, char *argv[])
{
  static unsigned char inputImage[SIZE][SIZE][RGB];
  static unsigned char bgrndImage[SIZE][SIZE][RGB];
  static unsigned char outputImage[SIZE][SIZE][RGB];
  static bool chromaMask[SIZE][SIZE];

  double threshold;

  if (argc < 6) {
    cerr << "usage: program_name in.bmp background.bmp dist_threshold " 
         << "out1.bmp out2.bmp" << endl;
    return 0;
  }
	
  if (readRGBBMP(argv[1], inputImage)) {
    cerr << "Error reading file: " << argv[1] << endl;
    return 1;
  }

  if (readRGBBMP(argv[2], bgrndImage)) {
    cout << "Error reading file: " << argv[2] << endl;
    return 1;
  }

  string inImage_file = argv[1];
  string bgImage_file = argv[2];
  threshold = stod(argv[3]);
  string out1Image_file = argv[4];
  string out2Image_file = argv[5];


  method1(inputImage, chromaMask, threshold);
  replace(chromaMask, inputImage, bgrndImage, outputImage);
  if (writeRGBBMP(argv[4], outputImage)) {
    cout << "Error writing file: " << argv[4] << endl;
    exit(1);
  }	


  method2(inputImage, chromaMask);
  replace(chromaMask, inputImage, bgrndImage, outputImage);
  if (writeRGBBMP(argv[5], outputImage)) {
    cout << "Error writing file: " << argv[5] << endl;
    exit(1);
  }	

  return 0;
}

// Use user-provided threshold for chroma-key distance
// The "output" of this method is to produce a valid mask array
//  where entries in the mask array are 1 for foreground image
//  and 0 for 'background'
void method1(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE],
	     double threshold)
{
    int average_red=0, average_green=0, average_blue=0, total_red=0, total_green=0, total_blue=0;
    double distance;
    for(int i=0; i<SIZE; i++){
        total_red += inImage[i][0][0]; // iterating through row1 and obtaining rgb(red)
        total_red += inImage[0][i][0]; // iterating through coloumn top left and obtaining rgb(red)

        total_green += inImage[i][0][1]; // iterating through row1 and obtaining rgb(green)
        total_green += inImage[0][i][1]; // iterating through coloumn top left and obtaining rgb(green)

        total_blue += inImage[i][0][2]; // iterating through row1 and obtaining rgb(green)
        total_blue += inImage[0][i][2]; // iterating through coloumn top left and obtaining rgb(green)
    }

    average_red = total_red/(SIZE*2);  // obtaining average_red
    average_green = total_green/(SIZE*2); // obtaining average_green
    average_blue = total_blue/(SIZE*2); // obtaining average_blue

    for(int i=0; i<SIZE; i++){ // iterating through rows
        for(int j=0; j<SIZE; j++){ // iterating through coloumns
            int imaged_red = inImage[i][j][0], imaged_green = inImage[i][j][1], imaged_blue = inImage[i][j][2]; // obtaining image pixels for red / green / blue
            distance = sqrt(pow((imaged_red-average_red), 2) + pow((imaged_green-average_green), 2) + pow((imaged_blue-average_blue), 2)); // calculating distance formula 
            if(distance < threshold){  // checking threshold and comparing it distance
                mask[i][j] = 0;
            }
            else{
                mask[i][j] = 1;
            }
        }
    }
}


// Devise a method to automatically come up with a threshold
//  for the chroma key determination
// The "output" of this method is to produce a valid mask array
//  where entries in the mask array are 1 for foreground image
//  and 0 for 'background'
void method2(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE])  
{
    int average_red=0, average_green=0, average_blue=0, total_red=0, total_green=0, total_blue=0;
    double distance, threshold, total_distance = 0;

    for(int i=0; i< SIZE; i++){

      total_red += inImage[i][0][0]; // iterating through row1 and obtaining rgb(red) 
      total_red += inImage[0][i][0]; // iterating through coloumn top left and obtaining rgb(red)
      total_red += inImage[i][SIZE-1][0]; // iterating through coloumn top right and obtaining rgb(red)
        


      total_green += inImage[i][0][1]; // iterating through row1 and obtaining rgb(green) 
      total_green += inImage[0][i][1]; // iterating through coloumn top left and obtaining rgb(green)
      total_green += inImage[i][SIZE-1][1]; // iterating through coloumn top right and obtaining rgb(green)
       



      total_blue += inImage[i][0][2]; // iterating through row1 and obtaining rgb(blue) 
      total_blue += inImage[0][i][2]; // iterating through coloumn top left and obtaining rgb(blue)
      total_blue += inImage[i][SIZE-1][2];  // iterating through coloumn top right and obtaining rgb(blue)



    }

    average_red = total_red/(SIZE*3);  // obtaining average_red
    average_green = total_green/(SIZE*3); // obtaining average_green
    average_blue = total_blue/(SIZE*3); // obtaining average_blue

    
    for(int i=0; i<SIZE; i++){ // iterating through rows
        for(int j=0; j<SIZE; j++){ // iterating through coloumns
            int imaged_red = inImage[i][j][0], imaged_green = inImage[i][j][1], imaged_blue = inImage[i][j][2]; // obtaining image pixels for red / green / blue
            total_distance += sqrt(pow((imaged_red-average_red), 2) + pow((imaged_green-average_green), 2) + pow((imaged_blue-average_blue), 2)); // calculating and obtaining total_distance formula 
        }
    }
    threshold = total_distance / (SIZE * SIZE ); // Creating a threshold formula 
    threshold = threshold * 1.27; // maintaing and adujsting the threshold scale



    for(int i=0; i<SIZE; i++){ // iterating through rows
        for(int j=0; j<SIZE; j++){ // iterating through coloumns
            int imaged_red = inImage[i][j][0], imaged_green = inImage[i][j][1], imaged_blue = inImage[i][j][2]; // obtaining image pixels for red / green / blue
            distance = sqrt(pow((imaged_red-average_red), 2) + pow((imaged_green-average_green), 2) + pow((imaged_blue-average_blue), 2)); // calculating distance formula 
            if(distance < threshold){  // checking threshold and comparing it distance
                mask[i][j] = 0;
            }
            else{
                mask[i][j] = 1;
            }
        }
    }


}



// If mask[i][j] = 1 use the input image pixel for the output image
// Else if mask[i][j] = 0 use the background image pixel
// Create the output image using the mask to determine
//  whether to use the pixel from the Input or Background image
void replace(bool mask[SIZE][SIZE],
	     unsigned char inImage[SIZE][SIZE][RGB],
	     unsigned char bgImage[SIZE][SIZE][RGB], 
	     unsigned char outImage[SIZE][SIZE][RGB])
{
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            if(mask[i][j] == 1){ // checking each through rgbs and if mask equals to 1 then use the input image pixel for the output image 
                outImage[i][j][0] = inImage[i][j][0]; 
                outImage[i][j][1] = inImage[i][j][1];
                outImage[i][j][2] = inImage[i][j][2];
            }
            else{ // checking each through rgbs and if mask equals to 0 then use the background image pixel for the output image 
                outImage[i][j][0] = bgImage[i][j][0];
                outImage[i][j][1] = bgImage[i][j][1];
                outImage[i][j][2] = bgImage[i][j][2];
            }
        }
    }

}
