#include "Encode.h"

bool readBinFile(string& str, const string& encodeBinFileNme) {
    ifstream fpr(encodeBinFileNme, ios::in | ios::binary);
    if (!fpr.is_open())
    {
        cout << "Open bin_file error!" << endl;
        return false;
    }

    stringstream ss;

    ss << fpr.rdbuf();
    str = ss.str();

    return true;
}

//����ȡ���ַ���ת��Ϊ01�����������ַ���
void string01(string& data, string& str, int& img_count) {
    int length_char, length_bit;
    length_char = str.size();
    length_bit = length_char * 8;

    img_count = length_bit / 9184;
    if (length_bit % 9184 != 0) {
        img_count++;//�жϼ���ͼ
    }

    int binary_int[8] = { 0 };//��ʱ����ַ��Ķ����Ʊ���
    str += "\0";

    //����ʱ �ֽڴ�ˣ�λС��
    for (int i = 0; i < length_char; i++) {
        for (int j = 7; j >= 0; j--) {
            binary_int[j] = str[i] % 2;
            str[i] /= 2;
        }
        for (int j = 0; j < 8; j++) {
            if (binary_int[j] == 1) {
                data.append(1, '1');
            }
            else if (binary_int[j] == 0) {
                data.append(1, '0');
            }
        }
    }
    data += "\0";//������
    cout << "�����Ķ������ļ��ַ�����Ϊ��" << data.length() << endl;
    cout << "�����Ƶ�λ��Ϊ��" << data.length() * 8 << endl;

}

//��λ���λ��ƣ��úڰ׺�3�����ο����
void DrawBlock(Mat img, Point tr) {
    rectangle(img, Point(0, 0) + tr, Point(105, 105) + tr, Scalar(0, 0, 0), -1);//������ľ��ο�RGB����ɫ��
    rectangle(img, Point(15, 15) + tr, Point(90, 90) + tr, Scalar(255, 255, 255), -1);//�м�ľ��ο�,RGB����ɫ��
    rectangle(img, Point(30, 30) + tr, Point(75, 75) + tr, Scalar(0, 0, 0), -1);//������ľ��ο�,RGB����ɫ��
}

//ȷ��������ɫ
void DrawColor(string code, int& R, int& G, int& B, int& key) {
    if (code[key] == '0') { R = 0; G = 0; B = 0; }//��
    if (code[key] == '1') { R = 255; G = 255; B = 255; }//��
    key = key + 1;
}

//��ά��ͼ����
void drawCode(string code, vector<Mat>& img) {
    //cout << code.size() << endl;
    int key = 0;//code����ı������
    int R = 255, G = 255, B = 255;//�����ͨ����ֵ
    Mat src(780, 780, CV_8UC3, Scalar(255, 255, 255));//���ص����src���������ͣ���ɫ��
    Point recSize(10, 10);//���ش�С

    DrawBlock(src, Point(0 + 30, 0 + 30));//����
    DrawBlock(src, Point(615 + 30, 0 + 30));//����
    DrawBlock(src, Point(0 + 30, 615 + 30));//����
    DrawBlock(src, Point(615 + 30, 615 + 30));//����

    //�м䲿�֣�һ��һ��Ϳ
    for (int col = 0; col < 72; col++) {//�м�һ��72��
        for (int row = 0; row < 48; row++) {//�м�һ��48��
            Point pos(row * 10 + 120 + 30, col * 10 + 30);//ȷ������λ��,120����Ϊ��λ���Σ�30�Ƕ�ά�����沿��
            DrawColor(code, R, G, B, key);//����������ɫ
            rectangle(src, pos, pos + recSize, Scalar(B, G, R), -1);//��������
            //��ͼ�������һ��ʱ����src����img��
            if (key >= code.size()) {
                img.push_back(src);
                return;
            }
        }
    }

    //������Ĳ��֣�һ��һ��Ϳ
    for (int col = 0; col < 48; col++) {//���˶�λ����һ��48��
        for (int row = 48; row < 60; row++) {//һ��12��
            Point pos(row * 10 + 120 + 30, col * 10 + 120 + 30);//ֻ�ܴ�120��ʼ����ǰ���Ƕ�λ�ķ���
            DrawColor(code, R, G, B, key);//����������ɫ
            rectangle(src, pos, pos + recSize, Scalar(B, G, R), -1);//��������
            //��ͼ�������һ��ʱ����src����img��
            if (key >= code.size()) {
                img.push_back(src);
                return;
            }
        }
    }

    //������Ĳ��֣�һ��һ��Ϳ
    for (int col = 0; col < 48; col++) {
        for (int row = 0; row < 12; row++) {
            Point pos(row * 10 + 30, col * 10 + 120 + 30);//ֻ�ܴ�120��ʼ����ǰ���Ƕ�λ�ķ���
            DrawColor(code, R, G, B, key);//����������ɫ
            rectangle(src, pos, pos + recSize, Scalar(B, G, R), -1);//��������
            //��ͼ�������һ��ʱ����src����img��
            if (key >= code.size()) {
                img.push_back(src);
                return;
            }
        }
    }
    img.push_back(src);
}