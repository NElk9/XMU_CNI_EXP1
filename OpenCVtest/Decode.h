#pragma once
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace cv;
using namespace std;

/*此文件实现：二维码解码为二进制文件*/
void Help_Decode(Scalar& color, string& code);

string Decode(Mat& img);

//二进制转字符
void BinToText(string& text, string& resCode);