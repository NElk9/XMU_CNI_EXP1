#pragma once
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

/*此文件实现：图片转视频，视频转图片，图片处理*/

//生成视频
void Get_Video(vector<Mat>& img);

//视频转图片 保存到res
void VideoToPic(vector<Mat>& picFromVideo, string decodeVideoName);

//图片处理
void handlePicFromVideo(vector<Mat>& resPic, vector<Mat>& picFromVideo);
void Handle_Img(Mat& srcImg, Mat& midImg);
bool Get_ROI(Mat& midImg, Mat& srcImg, Mat& dstImg);
//定位矩形
Point2f* Sort_Corner(vector<Point2f>& mc);
vector<Point2f> Get_Center(vector<vector<Point>>& corner);
vector<vector<Point>> Find_Corner(Mat& midImg);
