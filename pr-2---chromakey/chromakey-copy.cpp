/*********************************************************************
 File:     chromakey.cpp

 Author:   Your name here

 Email address:

********************************************************************/

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

int ar=0, ag=0, ab=0, tr=0, tg=0, tb=0;
    double distance;
    for(int i=0; i<SIZE; i++){
        tr += inImage[i][0][0];
        tr += inImage[0][i][0];

        tg += inImage[i][0][1];
        tg += inImage[0][i][1];

        tb += inImage[i][0][2];
        tb += inImage[0][i][2];
    }

    ar = tr/(SIZE*2);
    ag = tg/(SIZE*2);
    ab = tb/(SIZE*2);

    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            int ir = inImage[i][j][0], ig = inImage[i][j][1], ib = inImage[i][j][2];
            distance = sqrt(pow((ir-ar), 2) + pow((ig-ag), 2) + pow((ib-ab), 2));
            if(distance <= threshold){
                mask[i][j] = 0;
            }
            else{
                mask[i][j] = 1;
            }
        }
    }

}



  





// The "output" of this method is to produce a valid mask array
//  where entries in the mask array are 1 for foreground image
//  and 0 for 'background'
void method2(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE])  
{
   int ar=0, ag=0, ab=0, tr=0, tg=0, tb=0;
    double distance, threshold, total_distance = 0;
    for(int i=0; i<SIZE; i++){
        tr += inImage[i][0][0];
        tr += inImage[0][i][0];
        tr += inImage[i][SIZE-1][0];

        tg += inImage[i][0][1];
        tg += inImage[0][i][1];
        tg += inImage[i][SIZE-1][1];

        tb += inImage[i][0][2];
        tb += inImage[0][i][2];
        tb += inImage[i][SIZE-1][2];
    }

    ar = tr/(SIZE*3);
    ag = tg/(SIZE*3);
    ab = tb/(SIZE*3);

    
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            int ir = inImage[i][j][0], ig = inImage[i][j][1], ib = inImage[i][j][2];
            total_distance += sqrt(pow((ir-ar), 2) + pow((ig-ag), 2) + pow((ib-ab), 2));
        }
    }
    threshold = total_distance / (SIZE * SIZE);

    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            int ir = inImage[i][j][0], ig = inImage[i][j][1], ib = inImage[i][j][2];
            distance = sqrt(pow((ir-ar), 2) + pow((ig-ag), 2) + pow((ib-ab), 2));
            if(distance < threshold){
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
void replace(bool mask[SIZE][SIZE],
	     unsigned char inImage[SIZE][SIZE][RGB],
	     unsigned char bgImage[SIZE][SIZE][RGB],
	     unsigned char outImage[SIZE][SIZE][RGB])
{

for(int i = 0; i < SIZE; i++){
  for(int j=0; j<SIZE; j++){
    if(mask[i][j] == 1){
      outImage[i][j][0] = inImage[i][j][0];
      outImage[i][j][1] = inImage[i][j][1];
      outImage[i][j][2] = inImage[i][j][2];
    }
    else {
      outImage[i][j][0] = inImage[i][j][0];
      outImage[i][j][1] = inImage[i][j][1];
      outImage[i][j][2] = inImage[i][j][2];
    }
  }
}


}
