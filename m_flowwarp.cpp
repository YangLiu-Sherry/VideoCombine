#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include "colorflow.h"
#include "flowIO.h"

using namespace std;
using namespace cv;

typedef Mat_<float> Im32FC1;

int main(int argc, const char * argv[]) {

	string flo_s_t="src4-tar4.flo";//epicflo
	string flo_t_s="tar4-src4.flo";//epicflo

	//src to tar
	Im32FC1 u1,v1;
	//Mat_<float> u1,v1;
	ReadFlowFile(u1, v1, flo_s_t);
	Mat colorMat1;
	colorFlow(u1, v1, colorMat1);
	imshow("colorMat1", colorMat1);
	imwrite("src4-tar4.png", colorMat1);

	//tar to src
	Im32FC1 u2,v2;
	//Mat_<float> u2,v2;
	ReadFlowFile(u2, v2, flo_t_s);
	Mat colorMat2;
	colorFlow(u2, v2, colorMat2);
	imshow("colorMat2", colorMat2);
	imwrite("tar4-src4.png", colorMat2);

	Mat src = imread("src4.png");
	Mat tar = imread("tar4.png");
	//Mat dst_s_t,dst_t_s;
	//dst_s_t.create( src.size(), src.type());
	//dst_t_s.create( src.size(), src.type());

	//Mat map_x;
	//Mat map_y;
	//map_x.create( src.size(), CV_32FC1);
	//map_y.create( src.size(), CV_32FC1);
	//char image_name[25]; 


	//for(int count =1;count<=10;count++){

	//	Mat dst_s_t,dst_t_s;
	//	dst_s_t.create( src.size(), src.type());
	//	dst_t_s.create( src.size(), src.type());

	//	for( int i = 0; i < src.rows; i++)   // y
	//	{
	//		for( int j = 0; j < src.cols;j++)   // x
	//		{
	//			if( i + 0.1*count*(int)v1[i][j] < src.rows && j + 0.1*count*(int)u1[i][j]< src.cols && i + 0.1*count*(int)v1[i][j] >0 && j + 0.1*count*(int)u1[i][j]>0)
	//			{
	//				dst_s_t.at<cv::Vec3b>( i + 0.1*count*(int)v1[i][j] , j + 0.1*count*(int)u1[i][j])[0] = src.at<cv::Vec3b>(i,j)[0]; // mat.at(y,x) , first y location  ,and then x location
	//				dst_s_t.at<cv::Vec3b>( i + 0.1*count*(int)v1[i][j] , j + 0.1*count*(int)u1[i][j])[1] = src.at<cv::Vec3b>(i,j)[1];
	//				dst_s_t.at<cv::Vec3b>( i + 0.1*count*(int)v1[i][j] , j + 0.1*count*(int)u1[i][j])[2] = src.at<cv::Vec3b>(i,j)[2];

	//			}
	//		}
	//	}



	//	for( int i = 0; i < tar.rows; i++)   // y
	//	{
	//		for( int j = 0; j < tar.cols;j++)   // x
	//		{
	//			if( i + 0.1*(10-count)*(int)v2[i][j] < tar.rows && j + 0.1*(10-count)*(int)u2[i][j]< tar.cols  && i + 0.1*(10-count)*(int)v2[i][j]>0  && j + 0.1*(10-count)*(int)u2[i][j]>0)
	//			{
	//				dst_t_s.at<cv::Vec3b>( i + 0.1*(10-count)*(int)v2[i][j] , j + 0.1*(10-count)*(int)u2[i][j] )[0] = tar.at<cv::Vec3b>(i,j)[0]; // mat.at(y,x) , first y location  ,and then x location
	//				dst_t_s.at<cv::Vec3b>( i + 0.1*(10-count)*(int)v2[i][j] , j + 0.1*(10-count)*(int)u2[i][j] )[1] = tar.at<cv::Vec3b>(i,j)[1]; // v[行y][列x]
	//				dst_t_s.at<cv::Vec3b>( i + 0.1*(10-count)*(int)v2[i][j] , j + 0.1*(10-count)*(int)u2[i][j] )[2] = tar.at<cv::Vec3b>(i,j)[2];

	//			}
	//		}
	//	}


	//	// 用dst_t_s的内容填补dst_s_t
	//	for( int i = 0; i < dst_s_t.rows; i++)   // y
	//	{
	//		for( int j = 0; j < dst_s_t.cols;j++)   // x
	//		{
	//			if(dst_s_t.at<cv::Vec3b>(i,j)[0]==205 && dst_s_t.at<cv::Vec3b>(i,j)[1]==205 && dst_s_t.at<cv::Vec3b>(i,j)[2]==205)
	//			{
	//				dst_s_t.at<cv::Vec3b>(i,j)[0] = dst_t_s.at<cv::Vec3b>(i,j)[0]; // mat.at(y,x) , first y location  ,and then x location
	//				dst_s_t.at<cv::Vec3b>(i,j)[1] = dst_t_s.at<cv::Vec3b>(i,j)[1];
	//				dst_s_t.at<cv::Vec3b>(i,j)[2] = dst_t_s.at<cv::Vec3b>(i,j)[2];

	//			}
	//		}
	//	}

	//	for( int i = 0; i < dst_s_t.rows; i++)   // y
	//	{
	//		for( int j = 0; j < dst_s_t.cols;j++)   // x
	//		{
	//			if(dst_s_t.at<cv::Vec3b>(i,j)[0]==205 && dst_s_t.at<cv::Vec3b>(i,j)[1]==205 && dst_s_t.at<cv::Vec3b>(i,j)[2]==205 &&
	//				i-1>0 && i+1<dst_s_t.rows && j-1>0 && j+1< dst_s_t.cols)

	//			{
	//				dst_s_t.at<cv::Vec3b>(i,j)[0] = (dst_s_t.at<cv::Vec3b>(i-1,j-1)[0]+dst_s_t.at<cv::Vec3b>(i-1,j)[0]+dst_s_t.at<cv::Vec3b>(i-1,j+1)[0]
	//				+ dst_s_t.at<cv::Vec3b>(i,j-1)[0]+dst_s_t.at<cv::Vec3b>(i,j+1)[0]
	//				+ dst_s_t.at<cv::Vec3b>(i+1,j-1)[0]+dst_s_t.at<cv::Vec3b>(i+1,j)[0]+dst_s_t.at<cv::Vec3b>(i+1,j+1)[0])/8;

	//				dst_s_t.at<cv::Vec3b>(i,j)[1] = (dst_s_t.at<cv::Vec3b>(i-1,j-1)[1]+dst_s_t.at<cv::Vec3b>(i-1,j)[1]+dst_s_t.at<cv::Vec3b>(i-1,j+1)[1]
	//				+ dst_s_t.at<cv::Vec3b>(i,j-1)[1]+dst_s_t.at<cv::Vec3b>(i,j+1)[1]
	//				+ dst_s_t.at<cv::Vec3b>(i+1,j-1)[1]+dst_s_t.at<cv::Vec3b>(i+1,j)[1]+dst_s_t.at<cv::Vec3b>(i+1,j+1)[1])/8;

	//				dst_s_t.at<cv::Vec3b>(i,j)[2] = (dst_s_t.at<cv::Vec3b>(i-1,j-1)[2]+dst_s_t.at<cv::Vec3b>(i-1,j)[2]+dst_s_t.at<cv::Vec3b>(i-1,j+1)[2]
	//				+ dst_s_t.at<cv::Vec3b>(i,j-1)[2]+dst_s_t.at<cv::Vec3b>(i,j+1)[2]
	//				+ dst_s_t.at<cv::Vec3b>(i+1,j-1)[2]+dst_s_t.at<cv::Vec3b>(i+1,j)[2]+dst_s_t.at<cv::Vec3b>(i+1,j+1)[2])/8;

	//			}
	//		}
	//	}


	//	imshow("dst_s_t", dst_s_t);	
	//	sprintf(image_name, "%s%d%s", "flowarp_s_t_",count ,".png");
	//	imwrite(image_name, dst_s_t);

	//	imshow("dst_t_s", dst_t_s);	
	//	sprintf(image_name, "%s%d%s", "flowarp_t_s_", 10-count,".png");
	//	imwrite(image_name, dst_t_s);


	//}



	//for( int i = 0; i < src.rows; i++)
	//{
	//	for( int j = 0; j < src.cols; j++)
	//	{
	//		map_x.at<float>(i, j) =  j + u[i][j]; //
	//		map_y.at<float>(i, j) =  i + v[i][j];
	//	}
	//}
	//remap(tar, dst, map_x, map_y, INTER_LINEAR);

	/*imshow("dst_s_t_0.5", dst_s_t);	
	sprintf(image_name, "%s%s", "flowarp_s_t_0.5", ".png");
	imwrite(image_name, dst_s_t);

	imshow("dst_t_s_0.5", dst_t_s);	
	sprintf(image_name, "%s%s", "flowarp_t_s_0.5", ".png");
	imwrite(image_name, dst_t_s);
	*/




	waitKey();
	return 0;
}