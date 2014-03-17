#include "IndexReader.h"

IndexReader::IndexReader()
{
}
IndexReader::IndexReader(char *filepath)
{
	open(filepath);
}
IndexReader::~IndexReader()
{
	delete []path;
	delete []p_tii;
	delete []p_fdi;
	delete []p_fdt;
	delete []p_fmn;
	delete []p_pro;
	delete []p_del;
	delete []indexName;
	delete p_fdiInput;
	delete p_fdtInput;
	delete p_fmnInput;
	delete p_tiiInput;
	delete p_proInput;
	delete p_delInput;
	p_delOutput->seek(0);
	//cout<<"write: "<<delCount<<endl;
	p_delOutput->writeVlong(delCount);
	delete p_delOutput;
}
std::string IndexReader::getSegName(){
	int len=m_strPath.length()+5;
	char p_segment[len+20];
	strcpy(p_segment,m_strPath.c_str());
	strcat(p_segment,"segments");
	IndexInput segInput(p_segment);
	int32_t segCount=segInput.readInt();
	for(int i=0;i<segCount;i++){
		segInput.readString(segName);
		if(segInput.readVint()!=SEGSTATUS_CLOSE)
			continue;
		else
			break;
	}
	return segName;
}
void IndexReader::open(char *filepath){
	delCount=0;
	m_strPath=std::string(filepath);
	int len=strlen(filepath)+5;
    path=new char[len];
    strcpy(path,filepath);
    indexName=new char[10];
    strcpy(indexName,getSegName().c_str());
    strcat(path,indexName);
    
    p_tii=new char[len+20];
    strcpy(p_tii,path);
	strcat(p_tii,".tii");
   p_tiiInput=new IndexInput(p_tii);
   termCount=p_tiiInput->readLong();
   loadTerm();
   
   p_pro=new char[len+20];
   strcpy( p_pro,path);
   strcat( p_pro,".pro");
   p_proInput=new IndexInput(p_pro);
   
   p_fdi=new char[len+20];
    strcpy(p_fdi,path);
	strcat(p_fdi,".fdi");              
   p_fdiInput=new IndexInput(p_fdi);
	docCount=p_fdiInput->readLong();//首先写总数,这个先写0,等写完后再回到开头之上覆盖
   
   p_fdt=new char[len+20];
    strcpy(p_fdt,path);
	strcat(p_fdt,".fdt");              
	p_fdtInput=new IndexInput(p_fdt);
   
	   p_del=new char[len+20];
	    strcpy(p_del,path);
		strcat(p_del,".del");              
		p_delInput=new IndexInput(p_del);
		loadDel();
		p_delOutput=new IndexOutput(p_del,false);
		
	 p_fmn=new char[len+20];
	 strcpy(p_fmn,filepath);
	  strcat(p_fmn,"seg.fmn");              
	  p_fmnInput=new IndexInput(p_fmn);
	  loadFmn();
	  
//		p_del=new char[len+20];
//		strcpy(p_del, path);
//		strcat(p_del, ".del");
//		p_delOutput=new IndexOutput(p_del,create);
//		p_delOutput->writeLong(0);
}
void IndexReader::getTermDocs(std::string &term,std::vector<HitDoc> &docVo){
	std::map<std::string,int64_t>::iterator it=m_termMap.find(term);
	if(it!=m_termMap.end()){
		int64_t pos=m_termMap[term];
		p_proInput->seek(pos);
		int32_t count=p_proInput->readVint();
		for(int i=0;i<count;i++){
			HitDoc doc;
			int64_t docId=p_proInput->readVlong();
			std::map<int64_t,int64_t>::iterator delIt=m_delMap.find(docId);
			if(delIt==m_delMap.end()){
				int32_t hits=p_proInput->readVint();
				doc.setDocId(docId);
				doc.setHits(hits);
				docVo.push_back(doc);
			}
			else{
				p_proInput->readVint();
			}
		}
	}
}

void IndexReader::loadDel(){
	int64_t delC=p_delInput->readVlong();
	delCount=delC;
	for(int i=0;i<delC;i++){
		int64_t docid=p_delInput->readVlong();
		m_delMap[docid]=docid;
	}
}
void IndexReader::loadTerm(){
	for(int i=0;i<termCount;i++){
		std::string termStr;
		p_tiiInput->readString(termStr);
		int64_t proPointer=p_tiiInput->readVlong();
		m_termMap[termStr]=proPointer;
	}
}
int64_t  IndexReader::getTermCount(){
	return termCount;
}
void IndexReader::loadFmn(){
	int fieldCount= p_fmnInput->readInt();
	for(int i=0;i<fieldCount;i++){
		Field *deF=new Field();
		p_fmnInput->readString(deF->name);
		deF->is_store=p_fmnInput->readVint();
		deF->token=p_fmnInput->readVint();
		deF->type=p_fmnInput->readVint();
		m_doc.addField(deF->name,deF);
	}
}
int64_t IndexReader::getNumDoc(){
	return docCount;
}
void IndexReader::del(int64_t docid){
	p_delOutput->writeVlong(docid);
	delCount++;
	m_delMap[docid]=docid;
//	std::map<std::string, std::vector<int64_t> >::iterator it=m_delMap.find(segName);
//	if(it!=m_delMap.end()){
//		std::vector<int64_t> &delVo=(*it).second;
//		delVo.push_back(docid);
//	}
}
void  IndexReader::doc(int64_t docid, Document &doc){
	int64_t pos=8+docid*8;//
	p_fdiInput->seek(pos);
	p_fdtInput->seek(p_fdiInput->readLong());
	for(std::map<std::string,Field *>::iterator it=m_doc.m_fieldMap.begin(); it!=m_doc.m_fieldMap.end();it++){
		if((it->second)-> is_store!=STORE_YES)
			continue;
	    Field *newField=new Field();	
		switch((it->second)->type){
				case  STORE_VINT:{
					char temp[32];
					sprintf(temp,"%d",p_fdtInput->readVint());
					newField->data.assign(temp);
					break;
				}
				case   STORE_INT:{
					char temp[32];
					sprintf(temp,"%d",p_fdtInput->readInt());
					newField->data.assign(temp);
					break;
				}
				case   STORE_LONG:{
					char temp[32];
					sprintf(temp,"%d",p_fdtInput->readLong());
					newField->data.assign(temp);
					break;
				}
				case   STORE_VLONG:{
					char temp[32];
					sprintf(temp,"%d",p_fdtInput->readVlong());
					newField->data.assign(temp);
					break;
				}
				case   STORE_STRING:{
			//		std ::string s;
					p_fdtInput->readString(newField->data);
//					newField->data.stringData=new char[s.length()+1];
//					strcpy(newField->data.stringData,s.c_str());
					break;
				}
				default:
					break;
			}
		 doc.addField((it->second)->name,newField);
	}
}
