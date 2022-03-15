#include "PicAndVideo.h"

//图片生成视频
void Get_Video(vector<Mat>& img) {
    cout << "请输入您想保存视频的文件名（如Video.mp4）：" << endl;
    string videoName;
    cin >> videoName;

    int frame_rate = 10;//视频帧率默认为10
    cout << "视频帧率默认为10." << endl;
    VideoWriter video(videoName, CAP_ANY, frame_rate, Size(780, 780));//将视频保存成videoname
    for (size_t i = 0; i < img.size(); i++)//i:long long unsigned int
    {
        Mat image = img[i].clone();
        video << image;// 流操作符，把图片传入视频
    }
    cout << "视频生成成功！" << endl;
}

//视频转图片 保存到res
void VideoToPic(vector<Mat>& picFromVideo, string decodeVideoName) {
    VideoCapture capture(decodeVideoName);
    Mat frame;

    while (true) {//读取视频帧
        capture >> frame;
        if (frame.empty()) break;//停止读取
        else {
            resize(frame, frame, Size(720, 720), 0, 0, INTER_NEAREST);//调整大小（使用最近邻插值）
            picFromVideo.push_back(frame.clone());
            frame.release();//释放视频流
        }
    }
}

//提取二维码到resPic
void handlePicFromVideo(vector<Mat>& resPic, vector<Mat>& picFromVideo) {
    for (int i = 0; i < picFromVideo.size(); i++) {
        Mat midImg, dstImg;

        //简单图像处理（初步噪音去除）
        Handle_Img(picFromVideo[i], midImg);
		//提取图片中的二维码存入dstImage中
		bool flag = Get_ROI(midImg, picFromVideo[i], dstImg);//提取图片中的二维码存入dstImage中

		if (flag == true) {
			resPic.push_back(dstImg);
		}
    }
}

//简单图像处理（初步噪音去除）
void Handle_Img(Mat& srcImg, Mat& midImg) {
    midImg = srcImg.clone();
    cvtColor(midImg, midImg, CV_RGB2GRAY);//彩色转灰度
    for (int i = 0; i < 5; i++) {
        GaussianBlur(midImg, midImg, Size(3, 3), 0);//高斯滤波，平滑图像，减噪
    }
    threshold(midImg, midImg, 100, 255, THRESH_BINARY); //二值化（阈值）
}

//提取图片区域
bool Get_ROI(Mat& midImg, Mat& srcImg, Mat& dstImg) {
    vector<vector<Point>> corner = Find_Corner(midImg); //找到二维码四个定位

    if (corner.size() == NULL || int(corner.size()) != 4)  return false; //corner提取出错，则返回

    vector<Point2f> centerpoint(corner.size()); //储存四个矩阵的质心，2f代表二维浮点
    centerpoint = Get_Center(corner); //得到矩形的质心

    Point2f* P1;
    Point2f P2[4];//储存透视变换后的点位
    P1 = Sort_Corner(centerpoint); //四个矩形按照左上，右上，左下，右下排序
    P2[0] = Point2f(52, 52); //对应透视变换后的点位
    P2[1] = Point2f(668, 52);
    P2[2] = Point2f(52, 668);
    P2[3] = Point2f(668, 668);

    //计算变换矩阵进行透视变换
    Mat matrix = getPerspectiveTransform(P1, P2);
    //将透视变换后的图片储存在dstImg中
    warpPerspective(srcImg, dstImg, matrix, srcImg.size(), 1, 0, Scalar(255, 255, 255));

	return true;
}

//将四个定位矩形按顺序排列
//缺点：限制手机只能正着拍摄视频
//改进：设置三个定位点，利用三角形进行定位，同时可以判断方向
Point2f* Sort_Corner(vector<Point2f>& mc) {
	Point2f* p = new Point2f[4];//存储排序好的矩形中心点
	float sum[4];
	for (int i = 0; i < mc.size(); i++) {
		sum[i] = mc[i].x + mc[i].y;//sum数组储存x+y
	}
	int bottomright = max_element(sum, sum + 4) - sum;//右下最大，减sum得到编码
	int topleft = min_element(sum, sum + 4) - sum;//左上最小，减sum得到编码
	int topright = 0, bottomleft = 0;
	//确定左下和右上
	for (int i = 0; i < mc.size(); i++) {
		if (i == bottomright || i == topleft)  continue;//已经确定了，可以跳过
		if (mc[i].x > mc[i].y)
			topright = i;//右上的x>y
		if (mc[i].x < mc[i].y)
			bottomleft = i;//左下的y>x
	}
	p[0] = mc[topleft];//左上
	p[1] = mc[topright];//右上
	p[2] = mc[bottomleft];//左下
	p[3] = mc[bottomright];//右下
	return p;
}

//计算矩形的质心
vector<Point2f> Get_Center(vector<vector<Point>>& corner) {
	vector<Moments> mu(corner.size());//建立一个vector，元素个数为corner.size()个
	//计算每个轮廓的所有矩
	for (int i = 0; i < corner.size(); i++) {
		mu[i] = moments(corner[i], false);//opencv函数，获得轮廓的所有最高达三阶所有矩
		//默认就是false，true是要再次二值化
	}
	//计算轮廓的质心
	vector<Point2f> mc(corner.size());//存储质心的矩阵
	for (int i = 0; i < corner.size(); i++) {
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		// 质心的 X,Y 坐标：(m10/m00, m01/m00),m10 etc是moments内部成员
	}
	return mc;//返回质心
}

//找到定位矩形（有三个轮廓）
vector<vector<Point>> Find_Corner(Mat& midImg) {
	vector<vector<Point>> contours, corner;//contours储存所有轮廓，corner储存定位轮廓
	vector<Vec4i> hierarchy;
	//opencv函数，存储所有的矩形，层级树存储，默认从最小检测开始
	findContours(midImg, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	int index = -1, mark = 0;
	for (int i = 0; i < contours.size(); i++)//找到四个定位矩形的轮廓放进corner
	{
		if (contourArea(contours[i]) > 50000) continue;//计算轮廓下的面积，大于50000则不是，退出
		//-1表示没有对应的父轮廓，即是外面的轮廓
		if (hierarchy[i][2] != -1 && mark == 0) {//第一次找到最里轮廓
			index = i;//定位
			mark++;
		}
		else if (hierarchy[i][2] != -1) {//找到次里轮廓
			mark++;
		}
		else if (hierarchy[i][2] == -1) {//找到外面轮廓
			mark = 0;//mark清0
			index = -1;//index回置
		}
		//当找到2个定位点信息，此时定位矩阵确定
		if (mark >= 2) {
			corner.push_back(contours[index]);//将定位矩形存入corner中
			mark = 0;//ic清0
			index = -1;//index回置
		}
	}
	return corner;
}