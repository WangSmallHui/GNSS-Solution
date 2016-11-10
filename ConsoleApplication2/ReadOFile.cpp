#include "ReadOFile.h"
#include <iostream>
#include <stdlib.h>
#include "TString.h"
#include <fstream>

using std::ifstream;
using std::getline;
ReadOFile::ReadOFile(string ofile)
{
	_ofile = ofile;
}
ReadOFile::ReadOFile()
{
}
ReadOFile::~ReadOFile()
{
}
//��ȡO�ļ�ͷ
OHeader ReadOFile::ReadHeader()
{
	OHeader header;
	ifstream fread(_ofile);
	if (!fread.is_open())
	{
		std::cout << "�ļ���ʧ��\n";
		exit(1);
	}
	string line;
	while (getline(fread, line))
	{
		//�����!=string::npos
		//find���Ҳ���ʱ���᷵��-1�����Ƿ���һ����������ص�unsigened int
		//���ֱ��if��find��Խ�������ȥ
		if (line.find("END OF HEADER") != string::npos) break;
		if (line.find("COMMENT") != string::npos) continue;
		if (line.find("RINEX VERSION / TYPE") != string::npos)
		{
			header.version = line.substr(5, 4);
			header.type = line.substr(20, 1)[0];
			header.sattype = line.substr(40, 1)[0];
		}
		if (line.find("MARKER NAME") != string::npos)
		{
			header.markername = EString::Trim(line.substr(0, 60));
		}
		if (line.find("MARKER NUMBER") != string::npos)
		{
			header.markernumber = EString::TrimRight(line.substr(0, 60));
		}
		if (line.find("OBSERVER / AGENCY") != string::npos)
		{
			header.observer = EString::Trim(line.substr(0, 20));
			header.agency = EString::Trim(line.substr(20, 40));
		}
		if (line.find("ANT # / TYPE") != string::npos)
		{
			header.ant = EString::Trim(line.substr(0, 20));
			header.anttype = EString::Trim(line.substr(20, 20));
		}
		if (line.find("REC # / TYPE / VERS") != string::npos)
		{
			header.rec = EString::TrimRight(line.substr(0, 20));
			header.rectype = EString::TrimRight(line.substr(20, 20));
			header.recvers = EString::TrimRight(line.substr(40, 20));
		}
		if (line.find("APPROX POSITION XYZ") != string::npos)
		{
			header.PXYZ.x = stod(line.substr(0, 14));
			header.PXYZ.y = stod(line.substr(14, 14));
			header.PXYZ.z = stod(line.substr(28, 14));
		}
		if (line.find("ANTENNA: DELTA H/E/N") != string::npos)
		{
			//α�ݵ��㶨λ����Ҫ��������ƫ����������ֻ��Ҫ���߸�
			header.antenna_h = stof(line.substr(0, 14));
		}
		if (line.find("# / TYPES OF OBSERV") != string::npos)
		{
			header.sigsnums = atoi(line.substr(0, 16).c_str());
			if (header.sigsnums <= 9)   //���ź����ͳ���9ʱ���м�¼
			{
				for (int i = 0; i < header.sigsnums; i++)
				{
					header.sigtypes.push_back(EString::Trim(line.substr(10 + 6 * i, 2)));
				}
			}
			else
			{
				for (int i = 0; i < 9; i++)
				{
					header.sigtypes.push_back(EString::Trim(line.substr(10 + 6 * i, 2)));
				}
				getline(fread, line);
				for (int i = 0; i < header.sigsnums - 9; i++)
				{
					header.sigtypes.push_back(EString::Trim(line.substr(10 + 6 * i, 2)));
				}
			}
		}
		if (line.find("TIME OF FIRST OBS") != string::npos)
		{
			//�����ʱ��ָ��ΪGPSʱ��
			header.gtime.year = atoi(line.substr(0, 6).c_str());
			header.gtime.month = atoi(line.substr(6, 6).c_str());
			header.gtime.day = atoi(line.substr(12, 6).c_str());
			header.gtime.hour = atoi(line.substr(18, 6).c_str());
			header.gtime.minute = atoi(line.substr(24, 6).c_str());
			header.gtime.second = atof(line.substr(30, 13).c_str());
		}
		if (line.find("INTERVAL") != string::npos)
		{
			header.interval = stof(line.substr(0, 10));
		}
	}
	fread.close();
	return header;
}
//��ȡO�ļ��۲�ֵ
vector<OEpochData> ReadOFile::ReadData()
{
	vector<OEpochData> epochdatas;
	ifstream fread(_ofile);
	if (!fread.is_open())
	{
		std::cout << "�ļ���ʧ��\n";
	}
	OHeader header = ReadHeader();
	//�۲�ֵ���ͳ���5�������м�¼
	bool isMultiply = header.sigsnums > 5 ? true : false;
	//�����ļ�ͷ
	string line;
	while (getline(fread, line))
	{
		if (line.find(("END OF HEADER")) != string::npos)
			break;
	}
	//��ȡ�۲�����
	while (getline(fread, line))
	{
		if (line.substr(0, 80).size() == 0) break;
		OEpochData epochdata;
		vector<string> prns; //����PRN�б�
		//��Ԫʱ��
		int year = atoi(line.substr(1, 2).c_str());
		if (year >= 80 && year <= 99) year += 1900;
		else if (year >= 0 && year < 80) year += 2000;
		epochdata.gtime.year = year;
		epochdata.gtime.month = atoi(line.substr(4, 2).c_str());
		epochdata.gtime.day = atoi(line.substr(7, 2).c_str());
		epochdata.gtime.hour = atoi(line.substr(10, 2).c_str());
		epochdata.gtime.minute = atoi(line.substr(13, 2).c_str());
		epochdata.gtime.second = atof(line.substr(15, 11).c_str());
		epochdata.flag = atoi(line.substr(28, 1).c_str()); //��Ԫ��־
		epochdata.satsums = atoi(line.substr(29, 3).c_str()); //��������
		if (epochdata.flag == 1 || epochdata.flag == 4) //����Ԫ�����쳣��ȥ
		{
			for (int i = 0; i<epochdata.satsums; i++)
				getline(fread, line);
			continue;
		}
		string strPRN = EString::TrimRight(line).substr(32, line.size() - 32);
		if (epochdata.satsums > 12)  //����������12������
		{
			getline(fread, line);
			strPRN += EString::Trim(line);
		}
		//����PRN�б�
		for (string::size_type i = 0; i < strPRN.size(); i += 3)
		{
			string g_prn = strPRN.substr(i, 3);
			if (' ' == g_prn.at(1)) g_prn.at(1) = '0';
			prns.push_back(g_prn);
		}
		int c1Pos = -1, p1Pos = -1, p2Pos = -1, l1Pos = -1, l2Pos = -1;
		//C1�۲�ֵ��λ��,��0��ʼ
		for (vector<string>::size_type i = 0; i < header.sigtypes.size(); i++)
		{
			if (header.sigtypes.at(i) == "C1")
			{
				c1Pos = i;
				continue;
			}
			if (header.sigtypes.at(i) == "P1")
			{
				p1Pos = i;
				continue;
			}
			if (header.sigtypes.at(i) == "P2")
			{
				p2Pos = i;
				continue;
			}
			if (header.sigtypes.at(i) == "L1")
			{
				l1Pos = i;
				continue;
			}
			if (header.sigtypes.at(i) == "L2")
			{
				l2Pos = i;
				continue;
			}
		}
		//һ����Ԫ�������ǵ��������͵Ĺ۲�����
		for (int i = 0; i < epochdata.satsums; i++)
		{
			string strOrx;
			getline(fread, line);
			strOrx = line;
			strOrx += string(' ', 80 - line.size());
			if (isMultiply)
			{
				getline(fread, line);
				strOrx += line + string(' ', 80 - line.size());  //��ȫ��ʽ
			}
			//��ȡ�۲�ֵ
			if (c1Pos != -1 && strOrx.substr(c1Pos * 16, 14) != string(' ', 14))
			{
				ASatData asatdata;
				double obs = atof(strOrx.substr(c1Pos * 16, 14).c_str()) == 0 ? 0 : atof(strOrx.substr(c1Pos * 16, 14).c_str());
				asatdata.Obs = obs;
				asatdata.PRN = prns.at(i);
				epochdata.AllTypeDatas[C1].push_back(asatdata); //����C1�۲�ֵ
			}
			if (p1Pos != -1 && strOrx.substr(p1Pos * 16, 14) != string(' ', 14))
			{
				ASatData asatdata;
				double obs = atof(strOrx.substr(p1Pos * 16, 14).c_str()) == 0 ? 0 : atof(strOrx.substr(p1Pos * 16, 14).c_str());
				asatdata.Obs = obs;
				asatdata.PRN = prns.at(i);
				epochdata.AllTypeDatas[P1].push_back(asatdata); //����P1�۲�ֵ
			}
			if (p2Pos != -1 && strOrx.substr(p2Pos * 16, 14) != string(' ', 14))
			{
				ASatData asatdata;
				double obs = atof(strOrx.substr(p2Pos * 16, 14).c_str()) == 0 ? 0 : atof(strOrx.substr(p2Pos * 16, 14).c_str());
				asatdata.Obs = obs;
				asatdata.PRN = prns.at(i);
				epochdata.AllTypeDatas[P2].push_back(asatdata); //����P2�۲�ֵ
			}
			if (l1Pos != -1 && strOrx.substr(l1Pos * 16, 14) != string(' ', 14))
			{
				ASatData asatdata;
				double obs = atof(strOrx.substr(l1Pos * 16, 14).c_str()) == 0 ? 0 : atof(strOrx.substr(l1Pos * 16, 14).c_str());
				asatdata.Obs = obs;
				asatdata.PRN = prns.at(i);
				epochdata.AllTypeDatas[L1].push_back(asatdata); //����L1�۲�ֵ
			}
			if (l2Pos != -1 && strOrx.substr(l2Pos * 16, 14) != string(' ', 14))
			{
				ASatData asatdata;
				double obs = atof(strOrx.substr(l2Pos * 16, 14).c_str()) == 0 ? 0 : atof(strOrx.substr(l2Pos * 16, 14).c_str());
				asatdata.Obs = obs;
				asatdata.PRN = prns.at(i);
				epochdata.AllTypeDatas[L2].push_back(asatdata); //����L2�۲�ֵ
			}
		}
		//����
		//std::cout << epochdata.gtime.hour << " " << epochdata.gtime.minute << "  "<<epochdata.gtime.second << "\n";
		epochdatas.push_back(epochdata);
	}
	fread.close();
	return epochdatas;
}
