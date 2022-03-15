#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

/*此文件实现：
1. 读取二进制文件
2. 二进制文件编码*/

//读取用户输入的二进制文件
bool readBinFile(string& str, const string& encodeBinFileNme);

//将读取的字符串转化为01二进制数字字符串
void string01(string& data, string& str, int& img_count);

//二维码图绘制
void drawCode(string code, vector<Mat>& img);

//定位矩形绘制，用黑白黑3个矩形框叠加
void DrawBlock(Mat img, Point tr);

//确定像素颜色
void DrawColor(string code, int& R, int& G, int& B, int& key);
