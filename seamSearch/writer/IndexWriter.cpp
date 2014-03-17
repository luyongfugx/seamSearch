#include "IndexWriter.h"

extern CDict iDict;
IndexWriter::IndexWriter() {
	 curBufferDoc=0;
	 bufferDoc=100;
}

IndexWriter::~IndexWriter() {
	//写上总数
	flush();
	p_fdiOutput->seek(0);
	p_fdiOutput->writeLong(docCount);
	p_fmnOutput->seek(0);
	p_fmnOutput->writeInt(fieldCount);
	p_tiiOutput->seek(0);
	p_tiiOutput->writeLong(termCount);

	delete []p_segment;
	delete []path;
	delete []p_tii;
	delete []p_fdi;
	delete []p_fdt;
	delete []p_fmn;
	delete []p_pro;
	delete []p_del;

	delete []indexName;
	delete p_fdiOutput;
	delete p_proOutput;
	delete p_fdtOutput;
	delete p_fmnOutput;
	delete p_tiiOutput;
	delete p_delOutput;
//	IndexMerge *merge=new IndexMerge();
//	merge->open((char*)m_strPath.c_str());
//	merge->merge();
//    delete merge;
	
	IndexMerge merge;
	merge.open((char*)m_strPath.c_str());
	merge.merge();
    //delete merge;
}

void IndexWriter::open(const char * filepath, bool create) {
	m_strPath=string(filepath);
	docCount=0;
	fieldCount=0;
	termCount=0;
	isCreate=create;
	int len=strlen(filepath)+5;
	path=new char[len];
	strcpy(path, filepath);
	p_segment=new char[len+20];
	strcpy(p_segment,filepath);
	strcat(p_segment,"segments");
    FILE *fpDict;
	if ((fpDict = fopen(p_segment, "r")) == NULL&&create==false) {
		cout <<"create index!"<<endl;
        create=true;
		//exit(1);
	}
    fclose(fpDict);
   // delete fpDict;
//	if(!create)
//		loadsegments();
	
        
	indexName=new char[10];
	if(create)
		strcpy(indexName,"1");
	else
		getSegName(indexName);
	

	
	strcat(path, indexName);
	initSegment(create);
	
	p_tii=new char[len+20];
	strcpy(p_tii, path);
	strcat(p_tii, ".tii");
	p_tiiOutput=new IndexOutput(p_tii,true);
	p_tiiOutput->writeLong(0);

	
	p_del=new char[len+20];
	strcpy(p_del, path);
	strcat(p_del, ".del");
	p_delOutput=new IndexOutput(p_del,true);
	p_delOutput->writeLong(0);
	
	p_pro=new char[len+20];
	strcpy(p_pro, path);
	strcat(p_pro, ".pro");
	p_proOutput=new IndexOutput(p_pro,true);

	p_fdi=new char[len+20];
	strcpy(p_fdi, path);
	strcat(p_fdi, ".fdi");
//	if (!create)
//		loadfdi(p_fdi);
	p_fdiOutput=new IndexOutput(p_fdi,true);
	//if (create)
		p_fdiOutput->writeLong(0);//首先写总数,这个先写0,等写完后再回到开头之上覆盖

	p_fdt=new char[len+20];
	strcpy(p_fdt, path);
	strcat(p_fdt, ".fdt");
	p_fdtOutput=new IndexOutput(p_fdt,true);


	
	
	p_fmn=new char[len+20];
	strcpy(p_fmn, filepath);
	strcat(p_fmn, "seg.fmn");
	if (!create)
		loadfmn(p_fmn);
	p_fmnOutput=new IndexOutput(p_fmn,create);
    if (create)
		p_fmnOutput->writeInt(0);
}
void IndexWriter::initSegment(bool create){
	IndexOutput segmentOutput(p_segment,create);
	if(!create){//如果不是新建则，先加一个新段，并标志为在写
		IndexInput segInput(p_segment);
		int32_t segCount=segInput.readInt();
		segCount=segCount+1;
		std::string segName(indexName);
		segmentOutput.writeString(segName);
		segmentOutput.writeVint(SEGSTATUS_WRITEING);
		segmentOutput.seek(0);//写有几个段
		segmentOutput.writeInt(segCount);
	}
	else{//如果是新建索引，则以1为名字
	   segmentOutput.writeInt(1);
	   std::string segName("1");
	   segmentOutput.writeString(segName);
	   segmentOutput.writeVint(SEGSTATUS_WRITEING);
	}
}
void IndexWriter::loadsegments(std::map<std::string,int32_t> &m_segmentMap){
	IndexInput segInput(p_segment);
	int32_t segCount=segInput.readInt();
	for(int i=0;i<segCount;i++){
		std::string segName;
		segInput.readString(segName);
		int32_t status=segInput.readVint();
		m_segmentMap[segName]=status;
	}
}
void IndexWriter::writeSegment(){
	std::map<std::string,int32_t> m_segmentMap;
	loadsegments(m_segmentMap);
	IndexOutput segmentOutput(p_segment,true);
	segmentOutput.writeInt(m_segmentMap.size());
	std::map<std::string,int32_t>::iterator it;
	for(it=m_segmentMap.begin();it!=m_segmentMap.end();it++){
		std::string segName=it->first;
		int32_t status=it->second;
		segmentOutput.writeString(segName);
		if(strcmp(segName.c_str(),indexName)==0){
			  segmentOutput.writeVint(SEGSTATUS_CLOSE);
		}
		else{
			 segmentOutput.writeVint(status);
		}
	}
}
void IndexWriter::getSegName(char *name){
	IndexInput segInput(p_segment);
	int32_t segCount=segInput.readInt();
	for(int i=0;i<segCount;i++){
		std::string segName;
		segInput.readString(segName);
		int32_t status=segInput.readVint();
		if(i==(segCount-1)){
			int32_t  lastName=atoi(segName.c_str());
			lastName++;
			sprintf(name,"%d",lastName);
		}
			
	}
}
void IndexWriter::loadtii(char *tii) {

}
//void IndexWriter::loadDel(char *del) {
//	IndexInput *p_delInput=new IndexInput(del);
//	int64_t delCount=p_delInput->readLong();
//	for(int i=0;i<delCount;i++){
//		int64_t docid=p_delInput.readVlong();
//		m_delVo.push_back(docid);
//	}
//	delete p_delInput;
//}
void IndexWriter::loadfdi(char *fdi) {
	IndexInput *p_fdiInput=new IndexInput(fdi);
	docCount=p_fdiInput->readLong();
	delete p_fdiInput;
}
void IndexWriter::loadfmn(char *fmn) {
	IndexInput *p_fmnInput=new IndexInput(fmn);
	fieldCount= p_fmnInput->readInt();
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
void IndexWriter::close() {

}
void IndexWriter::add(Document &doc) {
	p_fdiOutput->writeLong(p_fdtOutput->getFilePointer());
	//遍历，写数据文件，并在索引文件写上位置
	std::string docStr;
	CHzSeg iHzSeg;
	for (std::map<std::string,Field *>::iterator it=doc.m_fieldMap.begin(); it
			!=doc.m_fieldMap.end(); it++) {
		Field *newField=(*it).second;
		if (!m_doc.containField(newField->name)) {
			Field *deF=new Field();
			deF->is_store=newField->is_store;
			deF->token=newField->token;
			deF->type=newField->type;
			deF->name=newField->name;
			p_fmnOutput->writeString(deF->name);
			p_fmnOutput->writeVint(deF->is_store);
			p_fmnOutput->writeVint(deF->token);
			p_fmnOutput->writeVint(deF->type);
			m_doc.addField(deF->name, deF);
			fieldCount++;
		}
		if((newField->is_store)==STORE_NO
				) {//如果不存储，则跳过
					continue;
				}
				switch (newField->type) {
				case STORE_VINT: {
					p_fdtOutput->writeVint(atoi(newField->data.c_str()));
//					char temp[32];
//					sprintf(temp, "%d", newField->data.intData);
					docStr+=string(newField->data);
					break;
				}
					case STORE_INT
					: {
						p_fdtOutput->writeInt(atoi(newField->data.c_str()));
//						char temp[32];
//						sprintf(temp,"%d",newField->data.intData);
						docStr+=newField->data;
						break;
					}
					case
					STORE_LONG
					: {
						p_fdtOutput->writeLong(atoi(newField->data.c_str()));
//						char temp[64];
//						sprintf(temp,"%d",newField->data.longData);
						docStr+=string(newField->data);
						break;
					}
					case
					STORE_VLONG
					:
					{
						p_fdtOutput->writeVlong(atoi(newField->data.c_str()));
//						char temp[64];
//						sprintf(temp,"%d",newField->data.longData);
						docStr+=newField->data;
						break;
					}
					case
					STORE_STRING
					: {
						//std::string str=string(newField->data);
						p_fdtOutput->writeString(newField->data);
						docStr+=newField->data;
						break;
					}
				default:
					break;
				}
			}
			iHzSeg.SegmentSentenceMM(iDict, docStr);
			for (std::map<std::string,int32_t>::iterator it=iHzSeg.termMap.begin(); it
					!=iHzSeg.termMap.end(); it++) {
				if (m_termMap.find(it->first)==m_termMap.end()) {
					std::map<int64_t,int32_t> docMap;
					docMap[docCount]=it->second;
					m_termMap[it->first]=docMap;
					termCount++;
				} else {
					m_termMap[it->first][docCount]=it->second;
				}
				//	cout << it->first<<":" <<it->second<< endl; 
			}
			curBufferDoc++;
	
			docCount++;
		}
		void IndexWriter::flush() {
		//	if (curBufferDoc>=bufferDoc) {
				for (std::map<std::string,std::map<int64_t,int32_t> >::iterator it=m_termMap.begin(); it!=m_termMap.end(); it++) {
					
					std::string s=string(it->first);
					std::map<int64_t,int32_t> tMap=it->second;
					p_tiiOutput->writeString(s);//项值
					p_tiiOutput->writeVlong(p_proOutput->getFilePointer());//写文件偏移指针
				//	cout<<it->first<<":"<<p_proOutput->getFilePointer()<<endl;
					p_proOutput->writeVint(tMap.size());//文章数
					for (std::map<int64_t,int32_t>::iterator tit=tMap.begin(); tit!=tMap.end(); tit++) {
						p_proOutput->writeVlong(tit->first);//写文章号
						p_proOutput->writeVint(tit->second);//词频
						//cout<<tit->first<<":"<<tit->second<<endl;
					}
				}
		//	}
			m_termMap.clear();
			writeSegment();
		}
		void IndexWriter::setBufferDoc(int64_t num) {
			bufferDoc=num;
		}
