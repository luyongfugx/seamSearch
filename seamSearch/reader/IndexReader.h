#ifndef INDEXREADER_H_
#define INDEXREADER_H_
#include <string>
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include "HitDoc.h"
#include  "../comm/stdHeader.h"
#include  "../store/IndexOutput.h"
#include  "../store/IndexInput.h"
#include "../document/Document.h"
#include "HitDoc.h"
class IndexReader
{
public :
	   char  *p_tii;
	   char *p_fdi;
	   char *p_fdt;
	   char *p_fmn;
	   char *p_pro;
	   char *p_del;
		char *path;
		char * indexName;
		std::string segName;
		std::string m_strPath;
		int64_t docCount;
		int64_t delCount;
		int64_t termCount;
		 IndexInput *p_tiiInput;
		 IndexInput *p_fdiInput;
		 IndexInput *p_fdtInput;
		 IndexInput *p_fmnInput;
		 IndexInput *p_proInput;
		 IndexInput *p_delInput;
		 IndexOutput *p_delOutput;
		  Document m_doc;
		 std::map<std::string,int64_t> m_termMap;
		  std::map<int64_t, int64_t > m_delMap;
public:
	
	IndexReader();
	IndexReader(char * indexPath);
	int64_t getNumDoc();
	int64_t  getTermCount();
	std::vector<HitDoc> getTermDocs(std::string &term);
	void getTermDocs(std::string &term,std::vector<HitDoc> &docVo);
	void  doc(int64_t docId,Document & doc);
	void  del(int64_t docId);
	void open(char * indexPath);
	virtual ~IndexReader();
private:
	void loadFmn();
	void loadTerm();
	void loadDel();
	std::string getSegName();
};

#endif /*INDEXREADER_H_*/
