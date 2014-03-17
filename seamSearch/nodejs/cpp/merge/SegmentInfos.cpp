#include "SegmentInfos.h"

SegmentInfos::SegmentInfos()
{
}

SegmentInfos::~SegmentInfos()
{
	delete []p_segment;
}
std::string  SegmentInfos::getNextSegName(){
	return nextSegName;
}
void SegmentInfos::load(char *filepath){
	int len=strlen(filepath)+5;
	p_segment=new char[len+20];
	strcpy(p_segment,filepath);
	strcat(p_segment,"segments");
	IndexInput segInput(p_segment);
	int32_t segCount=segInput.readInt();
	int64_t start=0;
	int64_t docCount=0;
	m_docCount=0;
	for(int i=0;i<segCount;i++){
		Segment seg;
		segInput.readString(seg.segName);
		if(segInput.readVint()!=SEGSTATUS_CLOSE)
			continue;
		char fdi[len+20];
		strcpy(fdi,filepath);
		strcat(fdi,seg.segName.c_str());
		strcat(fdi,".fdi");
		start+=docCount;
		seg.startNum=start;
		IndexInput  fdiOutput(fdi);
		docCount=fdiOutput.readLong();
		m_docCount+=docCount;
		seg.docCount=docCount;
		m_segMap[seg.segName]=seg;
		if(i==(segCount-1)){
			int32_t temName=atoi(seg.segName.c_str())+1;
			char temp[5];
			sprintf(temp,"%d",temName);
			nextSegName.assign(temp);;
		}
	}
	
}
