#ifndef ESTRING_H_
#define ESTRING_H_

#include <string>
using std::string;
namespace EString
{
	string TrimLeft(const string& str);
	string TrimRight(const string& str);
	string Trim(const string& str);
	//��Щ�������в�ʶ��D��ѧ������
	string D2E(const string& str);
}

#endif 

