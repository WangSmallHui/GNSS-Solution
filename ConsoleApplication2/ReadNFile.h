#pragma once
#include <string>
#include "DataStruct.h"
class ReadNFile
{
public:
	ReadNFile(std::string filename);
	//��Ҫʹ��Ĭ�Ϲ��캯����
	ReadNFile();
	~ReadNFile();
	NFileHeader ReadNHeader();
	vector<NFileRecord> ReadNRecord();
private:
	std::string _filename;
};

