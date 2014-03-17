#ifndef INDEXINPUT_H_
#define INDEXINPUT_H_
#include  <stdio.h>
#include   <iostream>  
#include  <string>
#include  "../comm/stdHeader.h"
using namespace std;
class IndexInput
{	
protected:
            char*	  m_buffer;
			int		   m_buffersize;
			long	  m_bufferStart;		// m_buffer 在文件中的偏移量
			int		  m_bufferPos;	// m_buffer 中的位置
			FILE   *fp;
			char * m_fileName;
public:
	IndexInput();
	IndexInput(char *file);
	void open(char *m_file);
	void close();
	byte  readByte();
	int32_t readInt();
	int32_t readVint();
	int64_t readLong();
	int64_t readVlong();
	void  readChars(char *buffer,int64_t start ,int64_t   len);
	void  readString(std::string &str);
	void  seek(int64_t pos);
	int64_t getFilePointer();
	virtual ~IndexInput();
};

#endif /*INDEXINPUT_H_*/
