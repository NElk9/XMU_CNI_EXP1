#pragma once
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace cv;
using namespace std;

/*���ļ�ʵ�֣���ά�����Ϊ�������ļ�*/
void Help_Decode(Scalar& color, string& code);

string Decode(Mat& img);

//������ת�ַ�
void BinToText(string& text, string& resCode);