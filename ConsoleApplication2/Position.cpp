#include "Position.h"
#include "Tools.h"
#include "Matrix.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <map>

//CLK�����Ӳ�(s)
//SendTime���ǽ��յ��ź�ʱ���ǵ��źŷ���ʱ��
//LEAPSeconds ����
//stationPoinst ��վ���꣬��ʼֵΪ(0,0,0)
//Rr���ջ��Ӳ�
SatPoint SatPosition(const Time& obsTime, const Point& stationPoint, double pL, const NFileRecord& nData, double &SatCLK, double LEAPSeconds, double Rr)
{
	SatPoint satpoint;
	satpoint.PRN = nData.PRN;
	/********************�źŴ����ǵ����ջ�����ʱ��******************************/
	double TransTime0 = 0, TransTime1 = 0;
	GTime GobsTime = UTC2GTime(obsTime);
	//���������źŴ���ʱ��
	TransTime1 = pL / C - Rr + SatCLK;
	//�����źŴ���ʱ����Ҫ����
	//��һ�μ����վ����Ϊ(0,0,0)�����;Ͳ��˼����룬��Ȼ��������
	while (abs(TransTime0 - TransTime1) > 1.0e-8)
	{
		TransTime0 = TransTime1;
		//1.���������˶���ƽ�����ٶ�
		double n0 = sqrt(GM) / pow(nData.sqrtA, 3); //�ο�ʱ��TOE��ƽ�����ٶ�
		double n = n0 + nData.DetaN;
		//���ջ�����ʱ��-�źŴ���ʱ��=�������Ƿ����źŵ�ʱ��
		GTime SendTime;
		if ((GobsTime.seconds - TransTime1) < 0)
		{
			SendTime.week = GobsTime.week - 1;
			SendTime.seconds = GobsTime.seconds - TransTime1 + 7 * 86400;
		}
		else
		{
			SendTime.week = GobsTime.week;
			SendTime.seconds = GobsTime.seconds - TransTime1;
		}
		//2.����۲�˲�����ǵ�ƽ�����M0,����ע�����ϸ� �۲�˲��-�ο�ʱ��
		double TransTime = (SendTime.week - nData.GPSWeek) * 7 * 86400 + (SendTime.seconds - nData.TOE);// +LEAPSeconds;
		//�˼�һ�ܣ�604800����ʼ�����
		if (TransTime > 302400)   TransTime -= 604800;
		if (TransTime < -302400)  TransTime += 604800;
		double M = nData.M0 + n*TransTime;
		//3.����ƫ�����
		double E = 0, E0 = M;
		while (abs(E - E0) > 1.0e-14)
		{
			E0 = E;
			E -= (M + nData.e*sin(E0) - E0) / (nData.e*cos(E0) - 1);
		}
		//4.����������
		double f = atan2(sqrt(1.0 - nData.e*nData.e)*sin(E), cos(E) - nData.e);
		//5.���������Ǿ�
		double u1 = nData.omega + f;
		//6.�����㶯������
		double kesi_u = nData.Cuc*cos(2 * u1) + nData.Cus*sin(2 * u1);
		double kesi_r = nData.Crc*cos(2 * u1) + nData.Crs*sin(2 * u1);
		double kesi_i = nData.Cic*cos(2 * u1) + nData.Cis*sin(2 * u1);
		//7.�� u' r' i'�����㶯����
		double u = u1 + kesi_u;
		double r = nData.sqrtA*nData.sqrtA *(1 - nData.e*cos(E)) + kesi_r;
		double i = nData.i0 + kesi_i + nData.IDOT*TransTime;
		//8.���������ڹ��������ϵ�е�λ��
		double sat_x = r*cos(u);
		double sat_y = r*sin(u);
		//9.����۲�˲��������ľ���L
		double L = nData.OMEGA + (nData.OMEGA_DOT - We)*TransTime - We*nData.TOE;
		//10.����������˲ʱ��������ϵ�е�λ��
		satpoint.point.x = sat_x*cos(L) - sat_y*cos(i)*sin(L);
		satpoint.point.y = sat_x*sin(L) + sat_y*cos(i)*cos(L);
		satpoint.point.z = sat_y*sin(i);
		//11.������ת����,������λ�ù黯�������ź�ʱ�̵�λ��
		double deta_a = We*TransTime1;
		double earth_x = satpoint.point.x, earth_y = satpoint.point.y;
		satpoint.point.x = earth_x * cos(deta_a) + earth_y  * sin(deta_a);
		satpoint.point.y = earth_y * cos(deta_a) - earth_x * sin(deta_a);
		satpoint.point.z = satpoint.point.z;
		//12.�����Ӳ����
		//���������ЧӦ
		//�������Ƿ�Բ�ι������������ЧӦ������
		double deta_tr = -2 * sqrt(GM)*nData.sqrtA / pow(C, 2)*nData.e*sin(E);
		//�����������ݼ��������Ӳ�
		double deta_toc = (SendTime.week - UTC2GTime(nData.TOC).week) * 7 * 86400 + (SendTime.seconds - UTC2GTime(nData.TOC).seconds);
		SatCLK = nData.ClkBias + nData.ClkDrift*deta_toc + nData.ClkDriftRate*pow(deta_toc, 2) + deta_tr;
		//13.�ٴμ����źŴ���ʱ�䣬���������Ӳ�
		//���ǵ����ջ��۲����
		double sat2rec_x2 = pow((satpoint.point.x - stationPoint.x), 2);
		double sat2rec_y2 = pow((satpoint.point.y - stationPoint.y), 2);
		double sat2rec_z2 = pow((satpoint.point.z - stationPoint.z), 2);
		double pL1 = sqrt(sat2rec_x2 + sat2rec_y2 + sat2rec_z2);
		//���������Ӳ���ջ��Ӳ�
		TransTime1 = pL1 / C + SatCLK - Rr;
	}
	return satpoint;
}

//��������PRN��Ѱ�ҹ۲�ʱ���������ο�ʱ��TOE�����С�����ǵ�������
NFileRecord GetNFileRecordByObsTime(const Time& obsTime, const vector<NFileRecord>& nDatas, string &PRN)
{
	GTime GobsTime = UTC2GTime(obsTime);
	//�����б��ΪPRN������������������
	vector<double> timeInterval;
	vector<NFileRecord> waitGet;
	for (vector<NFileRecord>::size_type i = 0; i < nDatas.size(); i++)
	{
		if (nDatas.at(i).PRN == PRN)
		{
			double timebias = abs(nDatas.at(i).TOE - GobsTime.seconds) + abs(nDatas.at(i).GPSWeek - GobsTime.week) * 7 * 86400;
			timeInterval.push_back(timebias);
			waitGet.push_back(nDatas.at(i));
		}
	}
	return waitGet.at(MinOfNums(timeInterval));
}
//���㵥��Ԫ�Ĳ�վ����
//PXYZ ��վ��������
//SatCLK �����Ӳ�
//Rr ���ջ��Ӳ�
bool CalculationPostion(Point PXYZ, OEpochData &oData, Point &Position, const vector<NFileRecord>& nDatas, double LeapSeconds, double &Rr, double elvation)
{
	Point Position1;
	//�����еĲ�վ��������
	Position = PXYZ;
	//�������Ƿ����4
	if (oData.satsums < 4) return false;
	//��ʼ�������Ӳ�
	map<string, double> SatCLKs;
	for (vector<NFileRecord>::size_type i = 0; i < nDatas.size(); i++)
	{
		SatCLKs[nDatas[i].PRN] = 0.0;
	}
	//���۲�ֵ
	if (!CheckDatas(oData)) return false;
	//���������վ����,���Ƶ�������
	int iter_count = 0;
	while (abs(Position.x - Position1.x)>1e-10 || abs(Position.y - Position1.y) > 1e-10 || abs(Position.z - Position1.z) > 1e-10)
	{
		//��������ϵ��
		vector<double> vB;
		vector<double> vL;
		if (++iter_count > 50) break;

		Position1 = Position;
		for (int i = 0; i < oData.satsums; i++)
		{
			if (oData.AllTypeDatas[C1][i].PRN.substr(0, 1) != "G") continue;
			//���ݹ۲�ʱ��ѡ����ѹ������
			NFileRecord _nData = GetNFileRecordByObsTime(oData.gtime, nDatas, oData.AllTypeDatas[C1].at(i).PRN);
			//�����Ӳÿ�������Ӳ����һ�������Ҫ����PRN�����ϴμ��������������Ӳ�
			double &SatCLK = SatCLKs[_nData.PRN];
			double PObs = oData.AllTypeDatas[C1].at(i).Obs;
			//����λ��
			SatPoint satpoint = SatPosition(oData.gtime, Position, PObs, _nData, SatCLK, LeapSeconds, Rr);
			if (oData.AllTypeDatas[P1].at(i).Obs != 0)   satpoint = SatPosition(oData.gtime, Position, PObs, _nData, SatCLK, LeapSeconds, Rr);
			//���Ǹ߶Ƚ�
			double azel[2];
			if (!Elevation(Position, satpoint, elvation, azel)) continue;

			//�޵����ģ��+���������
			double trop = Trop(Position, satpoint, azel);
			/*PObs -= trop;
			if (oData.AllTypeDatas[P1].at(i).Obs != 0)
			{
			PObs = oData.AllTypeDatas[P1].at(i).Obs - trop;
			if (oData.AllTypeDatas[P2].at(i).Obs != 0)  PObs = 2.54573*PObs - 1.54573*(oData.AllTypeDatas[P2].at(i).Obs - trop);
			}*/

			//�޵����ģ��
			PObs = oData.AllTypeDatas[C1].at(i).Obs;
			if (oData.AllTypeDatas[P1].at(i).Obs != 0)
			{
				PObs = oData.AllTypeDatas[P1].at(i).Obs;
				if (oData.AllTypeDatas[P2].at(i).Obs != 0)  PObs = 2.54573*PObs - 1.54573*(oData.AllTypeDatas[P2].at(i).Obs);
			}
			double deta_x = satpoint.point.x - Position.x;
			double deta_y = satpoint.point.y - Position.y;
			double deta_z = satpoint.point.z - Position.z;
			double Rj = sqrt(deta_x*deta_x + deta_y*deta_y + deta_z*deta_z);
			double li = deta_x / Rj;
			double mi = deta_y / Rj;
			double ni = deta_z / Rj;
			double l = PObs - Rj + C*SatCLK - C*Rr;
			vB.push_back(-li);
			vB.push_back(-mi);
			vB.push_back(-ni);
			vB.push_back(1.0);
			vL.push_back(l);
		}
		if (vL.size() < 4)  return false;
		Matrix B(&vB[0], vB.size() / 4, 4);
		Matrix L(&vL[0], vL.size(), 1);
		Matrix detaX(4, 1);
		detaX = (B.Trans()*B).Inverse()*(B.Trans()*L);

		Position.x += detaX.get(0, 0);
		Position.y += detaX.get(1, 0);
		Position.z += detaX.get(2, 0);
		Rr += detaX.get(3, 0) / C;
	}
	return true;
}
//elevation �߶Ƚ�ֹ��
bool OutputResult(ReadFile read, string output, double elevation)
{
	using namespace std;
	//��ȡ�����͹۲�����
	OHeader oHeader = read._ofile.ReadHeader();
	vector<OEpochData> oDatas = read._ofile.ReadData();
	NFileHeader nHeader = read._nfile.ReadNHeader();
	vector<NFileRecord> nDatas = read._nfile.ReadNRecord();
	//���ջ��Ӳ�
	double Rr = 0;

	//���ÿ���۲���Ԫ�ļ�����
	ofstream ResFile(output);
	cout << "����Ԫ���� " << oDatas.size() << endl;
	if (!ResFile.is_open()) return false;
	ResFile << setw(16)<< "�۲���Ԫ" << setw(18) << "������" 
		    << setw(8) << "X" << setw(16) << "Y" << setw(12) << "Z" 
			<< setw(23) << "���ջ��Ӳ�\n";

	for (vector<OEpochData>::size_type i = 0; i < oDatas.size(); i++)
	{
		cout << "���ڴ��� " << i << endl;
		Point result;
		if (CalculationPostion(oHeader.PXYZ, oDatas[i], result, nDatas, nHeader.LeapSeconds, Rr, elevation))
		{
			ResFile << setw(4) << oDatas[i].gtime.year << setw(3) << oDatas[i].gtime.month << setw(3) << oDatas[i].gtime.day
				    << setw(3) << oDatas[i].gtime.hour << setw(3) << oDatas[i].gtime.minute << setw(11) << fixed << setprecision(7)
				    << oDatas[i].gtime.second;
			ResFile << setw(4) << oDatas[i].satsums;
			ResFile << setw(16) << fixed << setprecision(4) << result.x
				    << setw(15) << fixed << setprecision(4) << result.y
				    << setw(15) << fixed << setprecision(4) << result.z
				    << setw(16) << fixed << setprecision(10) << Rr << endl;
		}
		else
		{
			ResFile << "����Ԫ��������";
			ResFile << setw(4) << oDatas[i].gtime.year << setw(3) << oDatas[i].gtime.month << setw(3) << oDatas[i].gtime.day
				    << setw(3) << oDatas[i].gtime.hour << setw(3) << oDatas[i].gtime.minute << setw(11) << fixed << setprecision(7)
				    << oDatas[i].gtime.second << endl;
		}
	}
	ResFile.close();
	return true;
}