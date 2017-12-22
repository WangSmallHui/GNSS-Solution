#pragma once
#include <ostream>
#include <iomanip>
#include "DataStruct.h"

extern double UTC2JD(const Time &time);

//�����ļ����
extern bool CheckDatas(OEpochData &oDatas);
extern int MinOfNums(const vector<double>& nums);
//����ϵת��
extern BLHPoint PointToBLHPoint(const Point& point);
extern bool XYZToENU(const Point& station, const SatPoint& satpoint, ENUPoint &ENU);
//���������
extern double Trop(const Point &station, const SatPoint &satpoint);
extern bool Elevation(const Point &station, const SatPoint &satpoint, double elvation);
//�������
extern double Chebyshev(double x, int n);