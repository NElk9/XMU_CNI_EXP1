#include "PicAndVideo.h"

//ͼƬ������Ƶ
void Get_Video(vector<Mat>& img) {
    cout << "���������뱣����Ƶ���ļ�������Video.mp4����" << endl;
    string videoName;
    cin >> videoName;

    int frame_rate = 10;//��Ƶ֡��Ĭ��Ϊ10
    cout << "��Ƶ֡��Ĭ��Ϊ10." << endl;
    VideoWriter video(videoName, CAP_ANY, frame_rate, Size(780, 780));//����Ƶ�����videoname
    for (size_t i = 0; i < img.size(); i++)//i:long long unsigned int
    {
        Mat image = img[i].clone();
        video << image;// ������������ͼƬ������Ƶ
    }
    cout << "��Ƶ���ɳɹ���" << endl;
}

//��ƵתͼƬ ���浽res
void VideoToPic(vector<Mat>& picFromVideo, string decodeVideoName) {
    VideoCapture capture(decodeVideoName);
    Mat frame;

    while (true) {//��ȡ��Ƶ֡
        capture >> frame;
        if (frame.empty()) break;//ֹͣ��ȡ
        else {
            resize(frame, frame, Size(720, 720), 0, 0, INTER_NEAREST);//������С��ʹ������ڲ�ֵ��
            picFromVideo.push_back(frame.clone());
            frame.release();//�ͷ���Ƶ��
        }
    }
}

//��ȡ��ά�뵽resPic
void handlePicFromVideo(vector<Mat>& resPic, vector<Mat>& picFromVideo) {
    for (int i = 0; i < picFromVideo.size(); i++) {
        Mat midImg, dstImg;

        //��ͼ������������ȥ����
        Handle_Img(picFromVideo[i], midImg);
		//��ȡͼƬ�еĶ�ά�����dstImage��
		bool flag = Get_ROI(midImg, picFromVideo[i], dstImg);//��ȡͼƬ�еĶ�ά�����dstImage��

		if (flag == true) {
			resPic.push_back(dstImg);
		}
    }
}

//��ͼ������������ȥ����
void Handle_Img(Mat& srcImg, Mat& midImg) {
    midImg = srcImg.clone();
    cvtColor(midImg, midImg, CV_RGB2GRAY);//��ɫת�Ҷ�
    for (int i = 0; i < 5; i++) {
        GaussianBlur(midImg, midImg, Size(3, 3), 0);//��˹�˲���ƽ��ͼ�񣬼���
    }
    threshold(midImg, midImg, 100, 255, THRESH_BINARY); //��ֵ������ֵ��
}

//��ȡͼƬ����
bool Get_ROI(Mat& midImg, Mat& srcImg, Mat& dstImg) {
    vector<vector<Point>> corner = Find_Corner(midImg); //�ҵ���ά���ĸ���λ

    if (corner.size() == NULL || int(corner.size()) != 4)  return false; //corner��ȡ�����򷵻�

    vector<Point2f> centerpoint(corner.size()); //�����ĸ���������ģ�2f�����ά����
    centerpoint = Get_Center(corner); //�õ����ε�����

    Point2f* P1;
    Point2f P2[4];//����͸�ӱ任��ĵ�λ
    P1 = Sort_Corner(centerpoint); //�ĸ����ΰ������ϣ����ϣ����£���������
    P2[0] = Point2f(52, 52); //��Ӧ͸�ӱ任��ĵ�λ
    P2[1] = Point2f(668, 52);
    P2[2] = Point2f(52, 668);
    P2[3] = Point2f(668, 668);

    //����任�������͸�ӱ任
    Mat matrix = getPerspectiveTransform(P1, P2);
    //��͸�ӱ任���ͼƬ������dstImg��
    warpPerspective(srcImg, dstImg, matrix, srcImg.size(), 1, 0, Scalar(255, 255, 255));

	return true;
}

//���ĸ���λ���ΰ�˳������
//ȱ�㣺�����ֻ�ֻ������������Ƶ
//�Ľ�������������λ�㣬���������ν��ж�λ��ͬʱ�����жϷ���
Point2f* Sort_Corner(vector<Point2f>& mc) {
	Point2f* p = new Point2f[4];//�洢����õľ������ĵ�
	float sum[4];
	for (int i = 0; i < mc.size(); i++) {
		sum[i] = mc[i].x + mc[i].y;//sum���鴢��x+y
	}
	int bottomright = max_element(sum, sum + 4) - sum;//������󣬼�sum�õ�����
	int topleft = min_element(sum, sum + 4) - sum;//������С����sum�õ�����
	int topright = 0, bottomleft = 0;
	//ȷ�����º�����
	for (int i = 0; i < mc.size(); i++) {
		if (i == bottomright || i == topleft)  continue;//�Ѿ�ȷ���ˣ���������
		if (mc[i].x > mc[i].y)
			topright = i;//���ϵ�x>y
		if (mc[i].x < mc[i].y)
			bottomleft = i;//���µ�y>x
	}
	p[0] = mc[topleft];//����
	p[1] = mc[topright];//����
	p[2] = mc[bottomleft];//����
	p[3] = mc[bottomright];//����
	return p;
}

//������ε�����
vector<Point2f> Get_Center(vector<vector<Point>>& corner) {
	vector<Moments> mu(corner.size());//����һ��vector��Ԫ�ظ���Ϊcorner.size()��
	//����ÿ�����������о�
	for (int i = 0; i < corner.size(); i++) {
		mu[i] = moments(corner[i], false);//opencv���������������������ߴ��������о�
		//Ĭ�Ͼ���false��true��Ҫ�ٴζ�ֵ��
	}
	//��������������
	vector<Point2f> mc(corner.size());//�洢���ĵľ���
	for (int i = 0; i < corner.size(); i++) {
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		// ���ĵ� X,Y ���꣺(m10/m00, m01/m00),m10 etc��moments�ڲ���Ա
	}
	return mc;//��������
}

//�ҵ���λ���Σ�������������
vector<vector<Point>> Find_Corner(Mat& midImg) {
	vector<vector<Point>> contours, corner;//contours��������������corner���涨λ����
	vector<Vec4i> hierarchy;
	//opencv�������洢���еľ��Σ��㼶���洢��Ĭ�ϴ���С��⿪ʼ
	findContours(midImg, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	int index = -1, mark = 0;
	for (int i = 0; i < contours.size(); i++)//�ҵ��ĸ���λ���ε������Ž�corner
	{
		if (contourArea(contours[i]) > 50000) continue;//���������µ����������50000���ǣ��˳�
		//-1��ʾû�ж�Ӧ�ĸ��������������������
		if (hierarchy[i][2] != -1 && mark == 0) {//��һ���ҵ���������
			index = i;//��λ
			mark++;
		}
		else if (hierarchy[i][2] != -1) {//�ҵ���������
			mark++;
		}
		else if (hierarchy[i][2] == -1) {//�ҵ���������
			mark = 0;//mark��0
			index = -1;//index����
		}
		//���ҵ�2����λ����Ϣ����ʱ��λ����ȷ��
		if (mark >= 2) {
			corner.push_back(contours[index]);//����λ���δ���corner��
			mark = 0;//ic��0
			index = -1;//index����
		}
	}
	return corner;
}