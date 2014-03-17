#ifndef INDEXWRITER_H_
#define INDEXWRITER_H_
#include <string>
#include <stdio.h>
#include <iostream>
#include  "../comm/stdHeader.h"
#include  "../store/IndexOutput.h"
#include  "../store/IndexInput.h"
#include "../document/Document.h"
#include "../seg/CHzSeg.h"
#include "../merge/IndexMerge.h"
#include "../seg/Dict.h"
#include <map>
using namespace std;
class IndexWriter
{
private:
	   char  *p_tii;
	   char *p_pro;
	   char *p_fdi;
	   char *p_fdt;
	   char *p_fmn;
	   char *p_del;
		char *path;
		char *p_segment;
		char * indexName;
		std::string m_strPath;
		int64_t docCount;
		int32_t fieldCount;
		int64_t termCount;
		int64_t curBufferDoc;
		int64_t bufferDoc;
		bool isCreate;
	   IndexOutput *p_tiiOutput;
	   IndexOutput  *p_proOutput;
	   IndexOutput *p_fdiOutput;
	   IndexOutput *p_fdtOutput;
	   IndexOutput *p_fmnOutput;
	   IndexOutput * p_delOutput;
	   //  IndexOutput *p_segmentOutput;
	   Document m_doc;

	   std::map<std::string,std::map<int64_t,int32_t> > m_termMap;

public:
	IndexWriter();
	void open(const char * filepath,bool create);
	void close();
	void add(Document &doc);
	void flush();
	void setBufferDoc(int64_t num);
	virtual ~IndexWriter();
private:
	void initSegment(bool create);
	void  loadfmn(char *fmn);
	void loadfdi(char *fdi);
	void loadtii(char *tii);
	void loadDel(char *del);
	void loadsegments(std::map<std::string,int32_t> &m_segmentMap);
	void  getSegName(char *name);
	void writeSegment();
};

#endif /*INDEXWRITER_H_*/
