#ifndef SEGMENTINFOS_H_
#define SEGMENTINFOS_H_
#include <string>
#include <stdio.h>
#include <iostream>
#include  "../comm/stdHeader.h"
#include  "../store/IndexOutput.h"
#include  "../store/IndexInput.h"
#include "../document/Document.h"
#include "../seg/CHzSeg.h"
#include "../seg/Dict.h"
#include <map>
using namespace std;
struct Segment{
	int64_t startNum;
	int64_t docCount;
	std::string segName;
};
class SegmentInfos
{
public:
	std::map<std::string,Segment> m_segMap;
	int64_t m_docCount;
	std::string nextSegName;
	char *p_segment;
public:
	void load(char *path);
	std::string getNextSegName();
	SegmentInfos();
	virtual ~SegmentInfos();
};

#endif /*SEGMENTINFOS_H_*/
