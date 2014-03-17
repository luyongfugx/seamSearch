#ifndef DOCUMENT_H_
#define DOCUMENT_H_
#include <string>
#include <iostream>
#include <map>
#include  "../comm/stdHeader.h"
using namespace std;
class Field{
public:	
	int  type;//存储类型 
	int  is_store;//是否存储 
	int  token;//是否切词存储
	std::string name;//字段名字
	std::string data;//字段数据
public:
	          Field();
	          virtual ~ Field();
};
class Document
{
public:
	std::map<std::string,Field *> m_fieldMap;    
public:
	Document();
	void addField(std::string string,Field *field);
	int32_t getIntField(std::string fieldName);
	int64_t getLongField(std::string fieldName );
	std::string  getStringField(std::string fieldName);
	bool containField(std::string fileName);
	virtual ~Document();
};

#endif /*DOCUMENT_H_*/
