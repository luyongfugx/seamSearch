#ifndef _DICT_H_040401_
#define _DICT_H_040401_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;
class CDict
{
public:
	CDict();
	~CDict();
	  
	bool GetFreq(std::string&) const {return false;};
	bool IsWord(std::string&) const;
	void AddFreq(std::string&) {};
    void OpenDict(std::string&);	 
private:
	map<std::string, int> mapDict;
	 
};

#endif /* _DICT_H_040401_ */
