#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <map>
#include "MTime.h"

#define GM 3.986005E14 //������������G���������M֮�˻�
#define C  2.99792458E8 //����
#define PI atan(1)*4
#define We 7.292115E-5 //������ת���ٶ�

#define WGS84A  6378137.0     //WGS84����ϵ������
#define WGS84f  1/298.257223565 //WGS84����ϵƫ����
#define NMAX    11   //�б�ѩ����Ͻ���+1
#define LMAX    40   //����������Ͻ���

using std::string;
using std::vector;
using std::map;
enum ObsType{
	C1,
	P1,
	P2,
	L1,
	L2 = 4,
};
struct Point  //�ռ�����ϵ
{
	double x = 0;
	double y = 0;
	double z = 0;
};
struct SatPoint  //��������
{
	Point point;
	string PRN = "";
};
struct BLHPoint  //�������ϵ,��λΪ����
{
	double B;
	double L;
	double H;
	BLHPoint()
	{
		B = 0;
		L = 0;
		H = 0;
	}
};
struct ENUPoint  //վ�ĵ�ƽ����ϵ
{
	double E;
	double N;
	double U;
	ENUPoint()
	{
		E = 0;
		N = 0;
		U = 0;
	}
};

struct OHeader{  //O�ļ��ļ�ͷ
	string version;
	char type; //�ļ�����
	char sattype; //��������
	string markername; //��վ��
	string markernumber;//��վ���
	string observer;//�۲�������
	string agency;//�۲ⵥλ
	string rec;//���ջ����к�
	string rectype;//���ջ��ͺ�
	string recvers;//���ջ��汾��
	float antenna_h;//���߸�
	float interval;//��Ԫ���
	string ant;//�������к�
	string anttype;//�����ͺ�
	Point PXYZ; //��վ��������
	int sigsnums;//GPS���ջ��ź������������Ｔ����M������Ҳֻ����GPS�ź�����
	vector<string> sigtypes; //�ź��б�
	Time gtime;//��һ���۲��¼ʱ�̣�GPSʱ
};
struct ASatData //ÿ�����ǵĹ۲�ֵ�Լ����
{
	string PRN; //���Ǳ��
	double Obs; //�۲�ֵ
};
struct OEpochData //ÿһ����Ԫ�Ĺ۲�ֵ
{
	Time gtime; //��Ԫʱ��
	int satsums;//��������
	int flag;//��Ԫ��־��0��ʾ������1��ʾ��ǰ��Ԫ����һ��Ԫ�����쳣
	map<ObsType, vector<ASatData>> AllTypeDatas;  //�������������͵Ĺ۲�ֵ
};
struct NFileHeader        //N�ļ��ļ�ͷ
{
	string version;  //RINEX��ʽ�İ汾��
	char type;       //�ļ�����
	char system;     //�ļ���������ϵͳ
	double IonAlpha[4];  //�����еĵ�������A1-A4
	double IonBeta[4];   //�����еĵ�������B2-B4
	double DeltaUtaA[2]; //���ڼ���UTCʱ������ʱ�Ķ���ʽϵ��A1,A2
	double DeltaUtcT;   //���ڼ���UTCʱ������ʱ�Ĳο�ʱ��
	double DeltaUtcW;  //���ڼ���UTCʱ������ʱ �ο�����
	double LeapSeconds;  //���� ����UTC��GPSʱ�Ĳ���
};
struct NFileRecord  //PRN��ŵ����ǵĹ㲥����
{
	string PRN;     //  ���ǵ�Prn��	
	Time TOC;   //�����ӵĲο�ʱ��
	double ClkBias; //  �����ӵ�ƫ��s
	double ClkDrift;  //  �����ӵ�Ư�� s/s
	double ClkDriftRate;  //  �����ӵ�Ư���ٶ�s/(s2)
	//�㲥���1
	double IODE;
	double Crs;
	double DetaN; //�㶯����
	double M0;
	//�㲥���2
	double Cuc;
	double e; //���ƫ����
	double Cus;
	double sqrtA;
	//�㲥���3
	double TOE;  //�����ο�ʱ�̣�GPS���ڵ�����
	double Cic;
	double OMEGA;
	double Cis;
	//�㲥���4
	double i0;
	double Crc;
	double omega;
	double OMEGA_DOT;
	//�㲥���5
	double IDOT; //i�ı仯��
	double L2;
	int GPSWeek; //GPS��������TOEһͬ��ʾ�����Ĳο�ʱ��
	double L2_P;
	//�㲥���6
	double SatAccuracy;
	double SatHealthy;
	double TGD;
	double IODC;
	//�㲥���7
	double TransmissionTimeofMessage;
	double FitInterval;
	double Reamrk1;
	double Remark2;
};
//xCoffee yCoffee zCoffee: x,y,z���ϵ��
struct ChebyCoeff
{
	double XCoeff[NMAX + 2];
	double YCoeff[NMAX + 2];
	double ZCoeff[NMAX + 2];
	ChebyCoeff()
	{
		for (int i = 0; i < NMAX + 2; i++)
		{
			XCoeff[i] = 0;
			YCoeff[i] = 0;
			ZCoeff[i] = 0;
		}
	}
};
struct LargnageCoeff
{
	GTime gtime;
	Point point;
};
#endif