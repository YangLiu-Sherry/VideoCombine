#ifndef readflow_h
#define readflow_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "colorflow.h"

// flow_io.cpp
//
// read and write our simple .flo flow file format

// ".flo" file format used for optical flow evaluation
//
// Stores 2-band float image for horizontal (u) and vertical (v) flow components.
// Floats are stored in little-endian order.
// A flow value is considered "unknown" if either |u| or |v| is greater than 1e9.
//
//  bytes  contents
//
//  0-3     tag: "PIEH" in ASCII, which in little endian happens to be the float 202021.25
//          (just a sanity check that floats are represented correctly)
//  4-7     width as an integer
//  8-11    height as an integer
//  12-end  data (width*height*2*4 bytes total)
//          the float values for u and v, interleaved, in row order, i.e.,
//          u[row0,col0], v[row0,col0], u[row0,col1], v[row0,col1], ...
//

// first four bytes, should be the same in little endian
#define TAG_FLOAT 202021.25  // check for this when READING the file
#define TAG_STRING "PIEH"    // use this when WRITING the file

struct CError : public exception
{
	CError(const char* msg)                 { strcpy(message, msg); }
	CError(const char* fmt, int d)          { sprintf(message, fmt, d); }
	CError(const char* fmt, float f)        { sprintf(message, fmt, f); }
	CError(const char* fmt, const char *s)  { sprintf(message, fmt, s); }
	CError(const char* fmt, const char *s,
		int d)                          { sprintf(message, fmt, s, d); }
	char message[1024];         // longest allowable message
};

void ReadFlowFile(Im32FC1 &u, Im32FC1 &v, const std::string &_filename)
{
	const char *filename = _filename.c_str();

	if (filename == NULL)
		throw CError("ReadFlowFile: empty filename");

	const char *dot = strrchr(filename, '.');
	if (strcmp(dot, ".flo") != 0){
		printf("%s\n",filename);
		throw CError("ReadFlowFile (%s): extension .flo expected", filename);}

	FILE *stream = fopen(filename, "rb");
	if (stream == 0)
		throw CError("ReadFlowFile: could not open %s", filename);

	int width, height;
	float tag;

	if ((int)fread(&tag,    sizeof(float), 1, stream) != 1 ||
		(int)fread(&width,  sizeof(int),   1, stream) != 1 ||
		(int)fread(&height, sizeof(int),   1, stream) != 1)
		throw CError("ReadFlowFile: problem reading file %s", filename);

	if (tag != TAG_FLOAT) // simple test for correct endian-ness
		throw CError("ReadFlowFile(%s): wrong tag (possibly due to big-endian machine?)", filename);

	// another sanity check to see that integers were read correctly (99999 should do the trick...)
	if (width < 1 || width > 99999)
		throw CError("ReadFlowFile(%s): illegal width %d", filename, width);

	if (height < 1 || height > 99999)
		throw CError("ReadFlowFile(%s): illegal height %d", filename, height);

	int nBands = 2;
	//CShape sh(width, height, nBands);
	//img.ReAllocate(sh);

	Mat w=Mat::zeros(height, width, CV_32FC2);

	//printf("reading %d x %d x 2 = %d floats\n", width, height, width*height*2);
	int n = nBands * width;
	for (int y = 0; y < height; y++) {
		//float* ptr = &img.Pixel(0, y, 0);
		float* ptr = (float*)w.ptr(y);
		if ((int)fread(ptr, sizeof(float), n, stream) != n)
			throw CError("ReadFlowFile(%s): file is too short", filename);
	}
	if (fgetc(stream) != EOF)
		throw CError("ReadFlowFile(%s): file is too long", filename);

	fclose(stream);

	std::vector <cv::Mat> channels;
	cv::split(w, channels);

	u=channels[0];//  x方向的偏移
	v=channels[1];//  y方向的偏移


	////保存flow数据，速度慢
	//ofstream flowdata("flowsrc-tar.txt");
	//for(int i=0;i<u.size().height;i++) {//height  row行数； width  col列数
	//	for(int j=0;j<u.size().width;j++){

	//		flowdata << u[i][j] << " "<< v[i][j] << endl;
	//	
	//	}
	//}

}

// write a 2-band image into flow file
void WriteFlowFile(const Im32FC1 &u, const Im32FC1 &v, const std::string &_filename)
{
	const char *filename = _filename.c_str();

	std::vector <Im32FC1> channels(2);
	channels[0]=u;
	channels[1]=v;

	Mat w;
	merge(channels, w);

	if (filename == NULL)
		throw CError("WriteFlowFile: empty filename");

	const char *dot = strrchr(filename, '.');
	if (dot == NULL)
		throw CError("WriteFlowFile: extension required in filename '%s'", filename);

	if (strcmp(dot, ".flo") != 0)
		throw CError("WriteFlowFile: filename '%s' should have extension '.flo'", filename);

	//CShape sh = img.Shape();
	int width = u.cols, height = u.rows, nBands = w.channels();

	if (nBands != 2)
		throw CError("WriteFlowFile(%s): image must have 2 bands", filename);

	FILE *stream = fopen(filename, "wb");
	if (stream == 0)
		throw CError("WriteFlowFile: could not open %s", filename);

	// write the header
	fprintf(stream, TAG_STRING);
	if ((int)fwrite(&width,  sizeof(int),   1, stream) != 1 ||
		(int)fwrite(&height, sizeof(int),   1, stream) != 1)
		throw CError("WriteFlowFile(%s): problem writing header", filename);

	// write the rows
	int n = nBands * width;
	for (int y = 0; y < height; y++) {
		float* ptr = (float*)w.ptr(y);
		if ((int)fwrite(ptr, sizeof(float), n, stream) != n)
			throw CError("WriteFlowFile(%s): problem writing data", filename);
	}

	fclose(stream);
}

#endif



