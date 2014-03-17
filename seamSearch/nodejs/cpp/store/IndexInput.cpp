#include "IndexInput.h"

IndexInput::IndexInput()
{
}
IndexInput::IndexInput(char *file){
    //cout<<file<<endl;
	fp=fopen(file,"rb");
	m_buffersize=1024;
	m_bufferPos=0;
	m_bufferStart=0;
	m_buffer=new char[m_buffersize];
	 fread(m_buffer,m_buffersize,1,fp);
}
IndexInput::~IndexInput()
{
	delete []m_buffer;
	fclose(fp);
}
void  IndexInput::readString(std::string &str){
	int32_t len=readVint();
	char*  buf=new char[len+1];
	readChars(buf,0,len);
	buf[len]='\0';
	str=buf;
	delete []buf;
}
void IndexInput::readChars(char *buffer,int64_t start ,int64_t   len){
	size_t end = start + len;
	for (size_t i = start; i < end; i++)
	{
		byte b = readByte();
		if ((b & 0x80) == 0)
			buffer[i] = (char) (b & 0x7F);
		else if ((b & 0xE0) != 0xE0)
		{
			buffer[i] = (char) (((b & 0x1F) << 6) | (readByte() & 0x3F));
		}
		else
			buffer[i] = (char) (((b & 0x0F) << 12) | ((readByte() & 0x3F) << 6) | (readByte() & 0x3F));
	}
}
int64_t IndexInput::readVlong(){
	uint8_t b=readByte();
	uint64_t i=b&0x7f;
	for(int64_t j=7;(b&~0x7F)!=0;j+=7){
		b=readByte();
		i|=(b&0x7F)<<j;
	}
    return i;
}
int32_t  IndexInput::readVint(){
	uint8_t b=readByte();
	int32_t i=b&0x7f;
	for(int32_t j=7;(b&~0x7F)!=0;j+=7){
		b=readByte();
		i|=(b&0x7F)<<j;
	}
    return i;
}
int64_t IndexInput::readLong(){
	int32_t  i1=readInt();
	int32_t  i2=readInt();
	return (((int64_t)i1)<<32)|(i2&0xFFFFFFFFL);
}
int32_t IndexInput::readInt(){
		uint8_t b1 = readByte();
		uint8_t b2 = readByte();
		uint8_t b3 = readByte();
		uint8_t b4 = readByte();
		return ((b1 & 0xFF) << 24) | ((b2 & 0xFF) << 16) | ((b3 & 0xFF) <<  8)| (b4 & 0xFF);		
}

void  IndexInput::seek(int64_t pos){
	if(pos>=m_bufferStart&&pos<(m_bufferStart+m_buffersize))
		this->m_bufferPos=pos-m_bufferStart;
	else{
		m_bufferStart=pos;
		m_bufferPos=0;
		fseek(fp,pos,SEEK_SET);
		 fread(m_buffer,m_buffersize,1,fp);
	}
}
int64_t IndexInput::getFilePointer(){
	return (m_bufferStart+(int64_t)m_bufferPos);
}
byte IndexInput::readByte(){
	if(m_bufferPos==m_buffersize){
		  fread(m_buffer,m_buffersize,1,fp);
		  m_bufferPos=0;
		  m_bufferStart +=m_buffersize;
	}
	return m_buffer[m_bufferPos++];
}


