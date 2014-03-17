#include "IndexSearch.h"
extern CDict iDict;
IndexSearch::IndexSearch()
{
}

IndexSearch::~IndexSearch()
{
	delete reader;
}
void IndexSearch::open(char *indexPath){
	path=string(indexPath);
	hitCounts=0;
	reader=new IndexReader();
	reader->open((char *)path.c_str());
}
void  IndexSearch::doc(int64_t docId,Document & doc){
	reader->doc(docId,doc);
}
void  IndexSearch::SearchTerm(std::string term,std::vector<HitDoc> &hitVo){
	reader->getTermDocs(term,hitVo);
	this->hitCounts=hitVo.size();
}
void  IndexSearch::SearchAndString(std::string str,std::vector<HitDoc> &hitVo){
	//std::priority_queue<int64_t,std::vector<HitDoc>> Pqueue;
	
	PriQueue pQueue;
	CHzSeg iHzSeg;
	iHzSeg.SegmentSentenceMM(iDict, str);
	if(iHzSeg.termMap.size()==1){
		std::string term(iHzSeg.termMap.begin()->first);
		SearchTerm(term,hitVo);
		return;
	}
	for (std::map<std::string,int32_t>::iterator it=iHzSeg.termMap.begin(); it
			!=iHzSeg.termMap.end(); it++) {
		std::string term(it->first);
		std::vector<HitDoc> tempHits;
		reader->getTermDocs(term,tempHits);
		pQueue.put(tempHits);
	}
	pQueue.getResult(hitVo);
	this->hitCounts=hitVo.size();
}
void  IndexSearch::SearchOrString(std::string str,std::vector<HitDoc> &hitVo){
	CHzSeg iHzSeg;
	iHzSeg.SegmentSentenceMM(iDict, str);
	if(iHzSeg.termMap.size()==1){
		std::string term(iHzSeg.termMap.begin()->first);
		SearchTerm(term,hitVo);
		return;
	}
	std::vector<HitDoc> tempHits;
	std::map<int64_t,HitDoc> hitMap;
	for (std::map<std::string,int32_t>::iterator it=iHzSeg.termMap.begin(); it
			!=iHzSeg.termMap.end(); it++) {
		std::string term(it->first);
		reader->getTermDocs(term,tempHits);
		for(std::vector<HitDoc>::iterator vit=tempHits.begin();vit!=tempHits.end();vit++){
				HitDoc  hdoc=(* vit);
				std::map<int64_t,HitDoc>::iterator mapit=hitMap.find(hdoc.getDocId());
				if(mapit!=hitMap.end()){
					(mapit->second).setHits((mapit->second).getHits()+hdoc.getHits());
				}
				else{
					hitMap[hdoc.getDocId()]=hdoc;
				}
		}
		tempHits.clear();
	}
	std::map<int32_t,HitDoc,std::greater<int32_t> > tempMap;
	std::priority_queue<int64_t,std::vector<HitDoc> > Pqueue;
	for(std::map<int64_t,HitDoc>::iterator mit=hitMap.begin();mit!=hitMap.end();mit++){// 排序
		Pqueue.push(mit->second);
	}
		 int size=Pqueue.size();
		for(int i=0;i<size;i++){
			HitDoc  hdocx=Pqueue.top();
			hitVo.push_back(hdocx);
			Pqueue.pop();
		}
	this->hitCounts=hitVo.size();
}
