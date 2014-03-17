#include "IndexMerge.h"

IndexMerge::IndexMerge()
{
}
IndexMerge::IndexMerge(char *path)
{
	open(path);
}
IndexMerge::~IndexMerge()
{
}
void IndexMerge::open(char *path){
	m_path.assign(path);
	m_segmentInfos.load(path);
	loadfmn();
}
void IndexMerge::loadfmn(){
	int len=m_path.length();
	char fmn[len+10];
	strcpy(fmn,m_path.c_str());
	strcat(fmn,"seg.fmn");
	IndexInput *p_fmnInput=new IndexInput(fmn);
	int32_t fieldCount= p_fmnInput->readInt();
	for (int i=0; i<fieldCount; i++) {
		Field *deF=new Field();
		p_fmnInput->readString(deF->name);
		deF->is_store=p_fmnInput->readVint();
		deF->token=p_fmnInput->readVint();
		deF->type=p_fmnInput->readVint();
		m_doc.addField(deF->name, deF);
	}
	delete p_fmnInput;
}
void IndexMerge::copyDoc(IndexOutput &destFdt,IndexInput &input){
	for(std::map<std::string,Field *>::iterator it=m_doc.m_fieldMap.begin(); it!=m_doc.m_fieldMap.end();it++){
		if((it->second)-> is_store!=STORE_YES)
			continue;
		switch((it->second)->type){
				case  STORE_VINT:{
					destFdt.writeVint(input.readVint());
					break;
				}
				case   STORE_INT:{
					destFdt.writeInt(input.readInt());
					break;
				}
				case   STORE_LONG:{
					destFdt.writeLong(input.readLong());
					break;
				}
				case   STORE_VLONG:{
					destFdt.writeVlong(input.readVlong());
					break;
				}
				case   STORE_STRING:{
					std::string data;
					input.readString(data);
					destFdt.writeString(data);
					break;
				}
				default:
					break;
			}
		
	}
}
void IndexMerge::loadDel(IndexInput &delInput,std::map<std::string,std::vector<int64_t>  > &delMap,std::string segName){
	int64_t delCount=delInput.readLong();
	std::vector<int64_t> delVector;
	for(int64_t i=0;i<delCount;i++){
		delVector.push_back(delInput.readVlong());
	}
	delMap[segName]=delVector;
}
void IndexMerge::loadTii(IndexInput &tiiInput,std::map<std::string,std::map<std::string,int64_t> > &termMap,std::string segName){
	int64_t termCount=tiiInput.readLong();
	for(int64_t i=0;i<termCount;i++){
		std::string term;
		tiiInput.readString(term);//读取term
		std::map<std::string,std::map<std::string,int64_t> >::iterator it=termMap.find(term);
		if(it==termMap.end()){//如果没有这个 项
			map<std::string,int64_t> segTermMap;
			segTermMap[segName]=tiiInput.readVlong();
			termMap[term]=segTermMap;
		}
		else{//如果已经有这个项
				map<std::string,int64_t> segTermMap=it->second;
				segTermMap[segName]=tiiInput.readVlong();
				termMap[term]=segTermMap;
			}
	}
}
void IndexMerge::removeOldIndex(){
	std::map<std::string,Segment>::iterator it;
	for(it=m_segmentInfos.m_segMap.begin();it!=m_segmentInfos.m_segMap.end();it++){
		std::string segName=it->first;
		int len=m_path.length();
		char newPath[len+10];
		strcpy(newPath,m_path.c_str());
		strcat(newPath,segName.c_str());
		char tiiPath[len+10];
		strcpy(tiiPath,newPath);
		strcat(tiiPath,".tii");
		remove(tiiPath);
		
		char fdiPath[len+10];
		strcpy(fdiPath,newPath);
		strcat(fdiPath,".fdi");
		remove(fdiPath);
		
		char fdtPath[len+10];
		strcpy(fdtPath,newPath);
		strcat(fdtPath,".fdt");
		remove(fdtPath);
		
		char proPath[len+10];
		strcpy(proPath,newPath);
		strcat(proPath,".pro");
		remove(proPath);
		
		char delPath[len+10];
		strcpy(delPath,newPath);
		strcat(delPath,".del");
		remove(delPath);
	}
}
void IndexMerge::merge(){
	int len=m_path.length();
	char newPath[len+10];
	strcpy(newPath,m_path.c_str());
	strcat(newPath,m_segmentInfos.getNextSegName().c_str());
	char fdiPath[len+10];
	strcpy(fdiPath,newPath);
	strcat(fdiPath,".fdi");
	IndexOutput fdiOutput(fdiPath,true);//新的合并后的fdi
	char fdtPath[len+10];
	strcpy(fdtPath,newPath);
	strcat(fdtPath,".fdt");
	IndexOutput fdtOutput(fdtPath,true);//合并后的fdt
	fdiOutput.writeLong(m_segmentInfos.m_docCount);//写doc总数
	
	char tiiPath[len+10];
	strcpy(tiiPath,newPath);
	strcat(tiiPath,".tii");
	IndexOutput tiiOutput(tiiPath,true);//合并后的tii
	
	char delPath[len+10];
	strcpy(delPath,newPath);
	strcat(delPath,".del");
	IndexOutput delOutput(delPath,true);//合并后的del
	delOutput.writeVlong(0);
	
	char proPath[len+10];
	strcpy(proPath,newPath);
	strcat(proPath,".pro");
	IndexOutput proOutput(proPath,true);//合并后的tii
	proOutput.writeVlong(0);
	
	//首先合并fdt和fdi的各个段
	std::map<std::string,Segment>::iterator it;
	std::map<std::string,std::map<std::string,int64_t> > termMap;
	std::map<std::string,IndexInput *> indexMap;
	std::map<std::string,std::vector<int64_t>  > delMap;
	for(it=m_segmentInfos.m_segMap.begin();it!=m_segmentInfos.m_segMap.end();it++){
		std::string segName=it->first;
		Segment segment=it->second;
		char inProPath[len+10];
		strcpy(inProPath,m_path.c_str());
		strcat(inProPath,segName.c_str());
		strcat(inProPath,".pro");
		IndexInput *tProInput=new IndexInput(inProPath);
		
		indexMap[segName]=tProInput;
		
		char inTiiPath[len+10];
		strcpy(inTiiPath,m_path.c_str());
		strcat(inTiiPath,segName.c_str());
		strcat(inTiiPath,".tii");
		IndexInput tTiiInput(inTiiPath);
		loadTii(tTiiInput,termMap,segName);
		
		char inDelPath[len+10];
		strcpy(inDelPath,m_path.c_str());
		strcat(inDelPath,segName.c_str());
		strcat(inDelPath,".del");
		IndexInput tDelInput(inDelPath);
		loadDel(tDelInput,delMap,segName);
		
		
		
		char inFdtPath[len+10];
		strcpy(inFdtPath,m_path.c_str());
		strcat(inFdtPath,segName.c_str());
		strcat(inFdtPath,".fdt");
		IndexInput tfdtInput(inFdtPath);
		for(int64_t i=0;i<segment.docCount;i++){
			fdiOutput.writeLong(fdtOutput.getFilePointer());
			copyDoc(fdtOutput,tfdtInput);
		}
	}
	int64_t termCount=termMap.size();
	tiiOutput.writeLong(termCount);
	for(std::map<std::string,std::map<std::string,int64_t> >::iterator it=termMap.begin();it!=termMap.end();it++){
		std::string tm=string(it->first);
		tiiOutput.writeString(tm);
	//	cout<<tm<<endl;
		tiiOutput.writeVlong(proOutput.getFilePointer());
		std::map<std::string,int64_t> segMap=it->second;
		int32_t docC=0;
		//的到项的文章总数
		for(std::map<std::string,int64_t>::iterator segit=segMap.begin();segit!=segMap.end();segit++){
			IndexInput *in=indexMap[segit->first];
//			cout<<segit->first<<endl;
			in->seek(segit->second);
			docC+=in->readVint();
		}
		proOutput.writeVint(docC);
		for(std::map<std::string,int64_t>::iterator segit=segMap.begin();segit!=segMap.end();segit++){
			IndexInput *in=indexMap[segit->first];
			in->seek(segit->second);
			int32_t cou=in->readVint();//读取文章数
			Segment segment=m_segmentInfos.m_segMap[segit->first];//得到段信息
			for(int j=0;j<cou;j++){
			        int64_t docId=in->readVlong();//读取老文章号
			        docId+=segment.startNum;//新文章号
			        int32_t pro=in->readVint();//词频
			        proOutput.writeVlong(docId);
			        proOutput.writeVint(pro);
			}
		}
	}
	int64_t delCount=0;
	for(std::map<std::string,std::vector<int64_t>  >::iterator it=delMap.begin();it!=delMap.end();it++){
			std::string tm=string(it->first);
			Segment segment=m_segmentInfos.m_segMap[tm];//得到段信息
			std::vector<int64_t> &delVo =it->second;
			for(int i=0;i<delVo.size();i++){
				    int64_t delid=delVo[i];
				    delid=+ segment.startNum;
					delOutput.writeVlong(delid);
					delCount++;
				}
			}
	delOutput.seek(0);
	delOutput.writeVlong(delCount);
	for(std::map<std::string,IndexInput *>::iterator it= indexMap.begin();it!=indexMap.end();it++){
		delete it->second;
	}
	indexMap.clear();
	termMap.clear();
	removeOldIndex();
	char segPath[len+10];
	strcpy(segPath,m_path.c_str());
	strcat(segPath,"segments");
	IndexOutput segOutput(segPath,true);//合并后的tii
	segOutput.writeInt(1);
	std::string  newSeg=string(m_segmentInfos.getNextSegName());
	segOutput.writeString(newSeg);
	segOutput.writeVint(SEGSTATUS_CLOSE);
	
}
