#include <iostream>
#include <stdlib.h>
#include "ReadFile.h"
#include "Matrix.h"
#include "Tools.h"
#include "Position.h"
using namespace std;
int main(){
	//�������ݶ�ȡ
	/*string o= "D:\\ϵͳ�ļ�\\����\\o�ļ�\\unb31090.09o";
	string n = "D:\\ϵͳ�ļ�\\����\\o�ļ�\\unb31090.09n";
	ReadFile read = ReadFile(o, n);
	vector<OEpochData> odata = read._ofile.ReadData();
	vector<NFileRecord> ndata = read._nfile.ReadNRecord();*/

	//���Ծ����
	/*double m[][3] = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 8.0 } };
	Matrix M1(*m, 3, 3);
	cout<<M1.Inverse();*/

	//����ʱ��ת��
	/*Time time;
	time.year = 2007;
	time.month = 10;
	time.day = 26;
	time.hour = 9;
	time.minute = 30;
	time.second = 0;
	cout.precision(16);
	cout << UTC2JD(time)<<endl;
	system("pause");*/

	string o = "d:\\ϵͳ�ļ�\\����\\o�ļ�\\site247j.01o";
	string n = "d:\\ϵͳ�ļ�\\����\\o�ļ�\\site247j.01n";
	//string o = "D:\\ϵͳ�ļ�\\����\\o�ļ�\\unb31090.09o";
	//string n = "D:\\ϵͳ�ļ�\\����\\o�ļ�\\unb31090.09n";
	string w = "D:\\ϵͳ�ļ�\\����\\spp.pos";
	ReadFile reader(o, n);
	if (OutputResult(reader,w))
	{
		cout << "������ϣ�" << endl;
	}
	system("pause");
	return 0;
}