#include "store/IndexOutput.h"
#include "store/IndexInput.h"
#include "document/Document.h"
#include "writer/IndexWriter.h"
#include "reader/IndexReader.h"
#include "reader/HitDoc.h"
#include "comm/stdHeader.h"
#include "search/IndexSearch.h"
#include "merge/SegmentInfos.h"
#include "merge/IndexMerge.h"
#include "search/PriQueue.h"
#include  <stdio.h>
#include <string>
#include   <iostream>
using namespace std;
CDict iDict;
int main(int argc, char* argv[]){
    
    	     IndexWriter *indexWriter=new IndexWriter();
    	     indexWriter->open("/Users/waynelu/nginxhtmls/luyfsearch/index/",false);
    	     indexWriter->setBufferDoc(2);
    		Document doc;
    		Field *t=new Field();
    	   t->type= STORE_LONG;
    	   t->is_store=STORE_YES;
    	   t->token=STORE_KEYWORD;
    	   t->name="t";
    	   t->data="22";
    	   std::string nn="t";
    	   doc.addField(nn,t);
    
    		Field *tx=new Field();
    	  tx->type= STORE_STRING;
    	  tx->is_store=STORE_YES;
    	  tx->token=STORE_KEYWORD;
    	  tx->name="tx";
    	  tx->data="you and me 2222222222" ;
    	  std::string nnx="tx";
    	  doc.addField(nnx,tx);
    
    		Document doc1;
    		Field *t1=new Field();
    	   t1->type= STORE_LONG;
    	   t1->is_store=STORE_YES;
    	   t1->token=STORE_KEYWORD;
    	   t1->name="t";
    	   t1->data="22";
    	   doc1.addField(nn,t1);
    		Field *tx1=new Field();
    	  tx1->type= STORE_STRING;
    	  tx1->is_store=STORE_YES;
    	  tx1->token=STORE_KEYWORD;
    	  tx1->name="tx";
    	  tx1->data="you and you  22222222 ";
    	  doc1.addField(nnx,tx1);
    	 indexWriter->add( doc);
    	 indexWriter->add( doc1);
        
    	  delete indexWriter;
        delete t;
        delete tx;
        delete t1;
        delete tx1;
    
	
    //
    //		IndexReader *reader=new IndexReader();
    //		reader->open("/home/luyf/index/");
    //		reader->del(6);
    //		delete reader;
    //
    //	IndexReader *reader1=new IndexReader();
    //	reader1->open("/home/luyf/index/");
    //	for(int i=0;i<reader1->getNumDoc();i++){
    //	//	int i=23;
    //	Document doc;
    //	cout<<i<<endl;
    //	reader1->doc(i,doc);
    //	std::string fieldName="t";
    //     cout<<doc.getLongField(fieldName)<<endl;
    // 	 fieldName="tx";
    // 	std::string fieldValue;
    //     cout<<doc.getStringField(fieldName)<<endl;
    //	}
    //
    //	delete reader1;
	
	IndexSearch indexSearch;
	indexSearch.open("/Users/waynelu/nginxhtmls/luyfsearch/index/");
	std::vector<HitDoc> hitVo;
	std::string term="you me";
	indexSearch.SearchOrString(term,hitVo);
    cout<<"------SearchOrString----------"<<endl;
	for(int i=0;i<indexSearch.hitCounts;i++){
		HitDoc  hdoc=hitVo[i];
		Document doc;
		int64_t id= hdoc.getDocId();
	    cout<<id<<":"<<hdoc.getHits()<<endl;
        indexSearch.doc(id,doc);
        cout<<doc.getLongField(std::string("t"))<<endl;
        cout<<doc.getStringField(std::string("tx"))<<endl;
	}
	hitVo.clear();
    cout<<"-------.SearchAndString---------"<<endl;
	indexSearch.SearchAndString(term,hitVo);
	for(int i=0;i<indexSearch.hitCounts;i++){
		HitDoc  hdoc=hitVo[i];
		Document doc;
		int64_t id= hdoc.getDocId();
	    cout<<id<<":"<<hdoc.getHits()<<endl;
        indexSearch.doc(id,doc);
        cout<<doc.getLongField(std::string("t"))<<endl;
        cout<<doc.getStringField(std::string("tx"))<<endl;
	}
    
    
    return 0;
}




