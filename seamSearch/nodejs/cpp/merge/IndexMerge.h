#ifndef INDEXMERGE_H_
#define INDEXMERGE_H_
#include "SegmentInfos.h"
using namespace std;
class IndexMerge
{
private:
	SegmentInfos m_segmentInfos;
	std::string m_path;
	 Document m_doc;
public:
	IndexMerge();
	IndexMerge(char *path);
	virtual ~IndexMerge();
	void  open(char *path);
	void merge();
private:
	void loadfmn();
	void copyDoc(IndexOutput &destFdt,IndexInput &input);
	void loadTii(IndexInput &tiiInput,std::map<std::string,std::map<std::string,int64_t> > &termMap,std::string segName);
	void loadDel(IndexInput &delInput,std::map<std::string,std::vector<int64_t>  > &delMap,std::string segName);
	void removeOldIndex();
};

#endif /*INDEXMERGE_H_*/
