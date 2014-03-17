#ifndef STDHEADER_H_
#define STDHEADER_H_
#include <sys/types.h>
#include <wchar.h>
#include <queue>
#include <vector>
#include  <stdio.h>  
	typedef unsigned char		byte;
	//存储的数据类型
	#define  STORE_VINT 1
	#define   STORE_INT 2 
	#define   STORE_LONG 3
	#define   STORE_VLONG 4
	#define   STORE_STRING 5
	//数据是否存储
	#define   STORE_YES 1//存储
	#define   STORE_NO 2//不存储
	//数据是否切词
	#define  STORE_NO_TOKEN 1//不切词
	#define   STORE_TOKEN 2//切词
	#define   STORE_KEYWORD 3//不切词，直接当作关键词
	
    #define   SEGSTATUS_WRITEING 1//索引段正在写
    #define   SEGSTATUS_CLOSE 2 //索引写完可用
#endif /*STDHEADER_H_*/
