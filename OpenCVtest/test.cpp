#include<opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "Encode.h"
#include "Decode.h"
#include "PicAndVideo.h"

using namespace cv;
using namespace std;

int main()
{

    cout << "�������������Ķ������ļ�������test.bin����" << endl;
    string encodeBinFileName;
    cin >> encodeBinFileName;

    //��ȡ�������ļ�
    string str;//��¼��ȡ���ַ���
    bool flag = readBinFile(str, encodeBinFileName);
    if (flag == false) {
        exit(0);
    }

    //ת��Ϊ01�ַ�������data
    string data;//�洢�����Ʊ���
    int img_cnt;
    string01(data, str, img_cnt);

    //cout << img_cnt << endl;
    //cout << data.substr(0, 10) << endl;

    //��д��ά��
    //ÿ�ν�ȡ9184��λ��дΪһ�Ŷ�ά��ͼƬ���������9184λ��ֻ��ʣ�µĴ����ȥ
    vector<Mat>img;//����ͼƬ
    for (int i = 0; i < img_cnt; i++) {
        int begin = i * 9184;
        int end = 9184;
        if (begin + end > data.size()) {
            end = data.size() - begin + 1;
        }
        drawCode(data.substr(begin, end), img);
    }

    //ͼƬת��Ƶ
    Get_Video(img);

    //��ƵתͼƬ
    cout << "����������������Ƶ������Video.mp4����" << endl;
    string decodeVideoName;
    cin >> decodeVideoName;

    cout << "����������������Ķ������ļ�������res.bin����" << endl;
    string resBinFileName;
    cin >> resBinFileName;

    vector<Mat> picFromVideo;//�������Ƶ��ȡ����ͼƬ
    VideoToPic(picFromVideo, decodeVideoName);//�õ�ͼƬ

    //ͼƬ���룺
    //ͼƬ������ȡ�Ķ�ά��浽resPic
    vector<Mat> resPic;
    handlePicFromVideo(resPic, picFromVideo);
    
    //��resPic�Ķ�ά����뵽resCode�ַ���
    string resCode = "";
    for (vector<Mat>::iterator it = resPic.begin(); it != resPic.end(); it++) {
        string tem = Decode(*it);//�õ�ÿ��ͼƬ�ı���
        if (tem.empty()) {
            continue;
        }
        resCode += tem;
    }

    //������ת�ַ�
    string text = "";
    BinToText(text, resCode);

    //д���ļ�
    ofstream ofs(resBinFileName, ios::binary | ios::out);
    if (!ofs.is_open()) {
        cout << "fail to open resBinFile" << endl;
    }
    ofs << text;
    cout << "д��ɹ���" << endl;

    return 0;
}
