#ifndef _HZSEG_H_040415_
#define _HZSEG_H_040415_

#include <iostream>
#include <string>
#include "Dict.h"
#include <map>
using namespace std;

class CHzSeg
{

public:
	std::map<std::string,int> termMap;
public:
	CHzSeg();
	~CHzSeg();
	void SegmentSentenceMM (CDict&, string) ;
	void SegmentHzStrMM (CDict&, string) ;
private:
	void  addTerm(std::string term);
};
	
#endif /* _HZSEG_H_040415_ */
