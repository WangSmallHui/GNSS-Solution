#pragma once
#include "DataStruct.h"
#include "ReadFile.h"

extern SatPoint SatPosition(const Time& obsTime, const Point& stationPoint, double pL, const NFileRecord& nData, double &SatCLK, double LEAPSeconds, double Rr);
//��������PRN��Ѱ�ҹ۲�ʱ���������ο�ʱ��TOE�����С�����ǵ�������
extern NFileRecord GetNFileRecordByObsTime(const GTime& GobsTime, const vector<NFileRecord>& nDatas, string PRN);
extern bool CalculationPostion(Point PXYZ, OEpochData &oData, Point &Position, const vector<NFileRecord>& nDatas, double LeapSeconds, double &Rr, map<string, double> &SatCLKs, double elvation);
extern bool OutputResult(ReadFile read, string output,double elevation);
extern bool ChebyShev_SatPosition(const Time& obsTime, string PRN, const map<string, ChebyCoeff> &m, SatPoint& satpoint);
extern bool Lagrange_SatPosition(const Time& obsTime, string PRN, const map<string, vector<LargnageCoeff>>& L, SatPoint& satpoint);
//���ݹ㲥������Ҫ��ʱ�������������
extern void BroadcastSatPosition(const GTime& time, const NFileRecord& nData, SatPoint& satpoint);
//���
extern bool CoefficientofChebyShev(const vector<NFileRecord>& nDatas, map<string, ChebyCoeff>& m);
extern bool LagrangeCoeff(const vector<NFileRecord>& nDatas, map<string, vector<LargnageCoeff>>& L);
