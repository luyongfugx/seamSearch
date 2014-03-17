#include "Document.h"
Field::Field(){
	
}
Field::~ Field(){

}
Document::Document()
{
}

Document::~Document()
{
	for(std::map<std::string,Field *>::iterator it=m_fieldMap.begin();it!=m_fieldMap.end();){
		    delete it->second;
			m_fieldMap.erase(it++);
	}
}

void Document::addField(std::string str,Field *field){
	m_fieldMap[str]=field;
}
int32_t Document::getIntField(std::string str){
	std::map<std::string,Field *>::iterator it=m_fieldMap.find(str);
	int32_t re=-1;
	if(it!=m_fieldMap.end()){
		Field *newField=(*it).second;
		re=atoi(newField->data.c_str());
	}
	return re;
}

int64_t Document::getLongField(std::string str){
	std::map<std::string,Field *>::iterator it=m_fieldMap.find(str);
	int64_t re=-1;
	if(it!=m_fieldMap.end()){
		Field *newField=(*it).second;
		re=atoi(newField->data.c_str());
	}
	return re;
}
std::string Document::getStringField(std::string str){
	std::map<std::string,Field *>::iterator it=m_fieldMap.find(str);
	std::string  re="";
	if(it!=m_fieldMap.end()){
		Field *newField=(*it).second;
		re=newField->data;
	}
	return re;
}
bool Document::containField(std::string fileName){
	std::map<std::string,Field *>::iterator it=m_fieldMap.find(fileName);
	if(it!=m_fieldMap.end())
		return true;
	else
		return false;
}

