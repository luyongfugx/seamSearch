#ifndef INDEXSEARCH_H_
#define INDEXSEARCH_H_
#include "../reader/IndexReader.h"
#include "../seg/Dict.h"
#include "../seg/CHzSeg.h"
#include "PriQueue.h"
#include <map>
using namespace std;

extern CDict iDict;
class IndexSearch
{
private:
	std::string path;
	IndexReader *reader;
public:
	int64_t hitCounts;//文档数
	
public:
	IndexSearch();
	void open(char * indexPath);
	void SearchTerm(std::string term,std::vector<HitDoc> &hitVo);
	void SearchAndString(std::string str,std::vector<HitDoc> &hitVo);
	void SearchOrString(std::string str,std::vector<HitDoc> &hitVo);
	void  doc(int64_t docId,Document & doc);
	virtual ~IndexSearch();
};

#endif /*INDEXSEARCH_H_*/
