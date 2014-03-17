#ifndef INDEXOUTPUT_H_
#define INDEXOUTPUT_H_
#include  <stdio.h>
#include   <iostream>  
#include  <string>
#include  "../comm/stdHeader.h"
using namespace std;
class IndexOutput
{
	protected:
            char*	  m_buffer;
			int		   m_buffersize;
			long	  m_bufferStart;		// m_buffer 在文件中的偏移量
			int		  m_bufferPos;	// m_buffer 中的位置
			FILE   *fp;
			bool   isClose;
private:
	char * m_fileName;
public:
	IndexOutput();
	IndexOutput(char * m_file,bool create);
	void  writeInt( int32_t  value);
	void  writeVint( int32_t  value);
	void  writeByte(byte  b);
	void  writeString(std::string  &str);
	void  writeLong(int64_t value);
	void  writeVlong(int64_t value);
	void  writeChars(const char * s ,int64_t start, int64_t len);
	int64_t getFilePointer();
	void close();
	void	flush();
    void  flushBuffer() ;
	virtual ~IndexOutput();
   void seek(int64_t pos);
};

#endif /*INDEXOUTPUT_H_*/
