
#pragma once
// colorcode.cpp
//
// Color encoding of flow vectors
// adapted from the color circle idea described at
//   http://members.shaw.ca/quadibloc/other/colint.htm
//
// Daniel Scharstein, 4/2007
// added tick marks and out-of-range coding 6/05/07

#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include "opencv2/core/core.hpp"

//typedef unsigned char uchar;

using namespace std;
using namespace cv;



static int ncols = 0;
#define MAXCOLS 60
static int colorwheel[MAXCOLS][3];

typedef Mat_<float> Im32FC1;
typedef Mat_<uchar> Im8UC1;

inline void setcols(int r, int g, int b, int k)
{
    colorwheel[k][0] = r;
    colorwheel[k][1] = g;
    colorwheel[k][2] = b;
}

inline void makecolorwheel()
{
    // relative lengths of color transitions:
    // these are chosen based on perceptual similarity
    // (e.g. one can distinguish more shades between red and yellow
    //  than between yellow and green)
    int RY = 15;
    int YG = 6;
    int GC = 4;
    int CB = 11;
    int BM = 13;
    int MR = 6;
    ncols = RY + YG + GC + CB + BM + MR;
    //printf("ncols = %d\n", ncols);
    if (ncols > MAXCOLS)
        exit(1);
    int i;
    int k = 0;
    for (i = 0; i < RY; i++) setcols(255,	   255*i/RY,	 0,	       k++);
    for (i = 0; i < YG; i++) setcols(255-255*i/YG, 255,		 0,	       k++);
    for (i = 0; i < GC; i++) setcols(0,		   255,		 255*i/GC,     k++);
    for (i = 0; i < CB; i++) setcols(0,		   255-255*i/CB, 255,	       k++);
    for (i = 0; i < BM; i++) setcols(255*i/BM,	   0,		 255,	       k++);
    for (i = 0; i < MR; i++) setcols(255,	   0,		 255-255*i/MR, k++);
}

inline void computeColor(float fx, float fy, uchar pix[])
{
    if (ncols == 0){
        makecolorwheel();
    }
    float rad = sqrt(fx * fx + fy * fy);
    float a = atan2(-fy, -fx) / M_PI;
    float fk = (a + 1.0) / 2.0 * (ncols-1);
    int k0 = (int)fk;
    int k1 = (k0 + 1) % ncols;
    float f = fk - k0;
    //f = 0; // uncomment to see original color wheel
    for (int b = 0; b < 3; b++) {
        float col0 = colorwheel[k0][b] / 255.0;
        float col1 = colorwheel[k1][b] / 255.0;
        float col = (1 - f) * col0 + f * col1;
        if (rad <= 1)
            col = 1 - rad * (1 - col); // increase saturation with radius
        else
            col *= .75; // out of range
        pix[2 - b] = (int)(255.0 * col);
    }
}

// the "official" threshold - if the absolute value of either
// flow component is greater, it's considered unknown
#define UNKNOWN_FLOW_THRESH 1e9
// value to use to represent unknown flow
#define UNKNOWN_FLOW 1e10
inline bool unknown_flow(float u, float v) {
    return (fabs(u) >  UNKNOWN_FLOW_THRESH)
    || (fabs(v) >  UNKNOWN_FLOW_THRESH)
    || _isnan(u) || _isnan(v);
}
#define ___max(a,b)  (((a) > (b)) ? (a) : (b))
#define ___min(a,b)  (((a) < (b)) ? (a) : (b))


inline void colorFlow(const Mat &u, const Mat &v, Mat &colorMat, float MAXRAD = -1)
{
    //Mat colorMat;
    //CFloatImage motim -> Mat u,v
    
    //CShape sh = motim.Shape();
    int width = u.cols, height = u.rows;
    
    //colim.ReAllocate(CShape(width, height, 3));
    Mat colim=Mat(height, width, CV_8UC3);
    
    int x, y;
    // determine motion range:
    float maxx = -999, maxy = -999;
    float minx =  999, miny =  999;
    float maxrad = -1;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            //float fx = motim.Pixel(x, y, 0);
            //float fy = motim.Pixel(x, y, 1);
            float fx = u.at<float>(y, x);
            float fy = v.at<float>(y, x);
            if (unknown_flow(fx, fy))
                continue;
            maxx = ___max(maxx, fx);
            maxy = ___max(maxy, fy);
            minx = ___min(minx, fx);
            miny = ___min(miny, fy);
            float rad = sqrt(fx * fx + fy * fy);
            maxrad = ___max(maxrad, rad);
        }
    }
    //printf("max motion: %.4f  motion range: u = %.3f .. %.3f;  v = %.3f .. %.3f\n", maxrad, minx, maxx, miny, maxy);
    
    //if (maxmotion > 0) // i.e., specified on commandline
    //maxrad = maxmotion;
    
    if (maxrad == 0) // if flow == 0 everywhere
    {maxrad = 1;}
	
	if (MAXRAD > 0 /*&& maxrad > MAXRAD*/)
		maxrad = MAXRAD;

    //if (verbose)
    //fprintf(stderr, "normalizing by %g\n", maxrad);
    
    colorMat=Mat::zeros(height, width, CV_8UC3);
    
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            //float fx = motim.Pixel(x, y, 0);
            //float fy = motim.Pixel(x, y, 1);
            float fx = u.at<float>(y, x);
            float fy = v.at<float>(y, x);
            
           // vector<uchar> pix(3);
			uchar pix[3];
            //uchar *pix = &colim.Pixel(x, y, 0);
            if (unknown_flow(fx, fy)) {
                pix[0] = pix[1] = pix[2] = 0;
            } else {
                float px=fx/maxrad,py=fy/maxrad;
				float r = sqrt(px*px + py*py);
				if (r > 1.0f)
				{
					px /= r; py /= r;
				}
                computeColor(px, py, pix);
            }
            colorMat.at<Vec3b>(y,x)[0]=pix[0];
            colorMat.at<Vec3b>(y,x)[1]=pix[1];
            colorMat.at<Vec3b>(y,x)[2]=pix[2];
        }
    }
    //return colorMat;
}

inline float getMaxRad(Im32FC1 &gu, Im32FC1 &gv){
    float MAXRAD = 0;
    if (!gu.empty() && !gv.empty())
    {
        int rows=gu.rows;
        int cols=gv.cols;
        float u,v,rad;
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                u=gu(i,j);
                v=gv(i,j);
                rad=sqrt(u*u+v*v);
                if (rad<100000 && rad > MAXRAD){
                    MAXRAD = rad;
                }
            }
        }
    }
    return MAXRAD;
}












