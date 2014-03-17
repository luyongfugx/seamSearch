//
//  nodeSearch.c
//  seamSearch
//
//  Created by waynelu on 14-3-13.
//  Copyright (c) 2014年 waynelu. All rights reserved.
//


#include "../store/IndexOutput.h"
#include "../store/IndexInput.h"
#include "../document/Document.h"
#include "../writer/IndexWriter.h"
#include "../reader/IndexReader.h"
#include "../reader/HitDoc.h"
#include "../comm/stdHeader.h"
#include "../search/IndexSearch.h"
#include "../merge/SegmentInfos.h"
#include "../merge/IndexMerge.h"
#include "../search/PriQueue.h"
#include  <stdio.h>
#include <string>
#include   <iostream>
#include <node.h>
#include <v8.h>
CDict iDict;
using namespace v8;
using namespace std;
Handle<Value> Search(const Arguments& args) {
    HandleScope scope;
    Local<Object> resObject = Object::New ();
    String::Utf8Value dictFile(args[1]->ToString());
    std::string dictDir(*dictFile);
    iDict.OpenDict(dictDir);
    IndexSearch indexSearch;
    String::Utf8Value indexDir(args[0]->ToString());
	indexSearch.open(*indexDir);
	std::vector<HitDoc> hitVo;
    String::Utf8Value keyword(args[2]->ToString());
	std::string term(*keyword);
    
    Local<Array> jsonObject=Local<Array>::Cast(args[3]);
    Array * jsonArray(*jsonObject);
	indexSearch.SearchOrString(term,hitVo);
    resObject->Set (String::New ("hits"), Integer::New (indexSearch.hitCounts));
    resObject->Set (String::New ("datas"), Array::New ());
    Local<Object> jobs = resObject->Get (String::New ("datas"))->ToObject ();
	for(int i=0;i<indexSearch.hitCounts;i++){
        Local<Object> data= Object::New ();
		HitDoc  hdoc=hitVo[i];
		Document doc;
		int64_t id= hdoc.getDocId();
        data->Set (String::New ("id"),Integer::New (id));
        indexSearch.doc(id,doc);
        for(int x=0;x<jsonArray->Length();x++){
            Array *fieldArray(*(Local<Array>::Cast(jsonArray->Get(x))));
            String::Utf8Value fieldn(fieldArray->Get(0)->ToString());
            std::string fieldname(*fieldn);
            String::Utf8Value fieldt(fieldArray->Get(1)->ToString());
            std::string fieldtype(*fieldt);
            if(fieldtype=="long"){
                data->Set (String::New (fieldname.c_str()),Integer::New (doc.getLongField(std::string(fieldname))));
            }
            else{
                data->Set (String::New (fieldname.c_str()),String::New (doc.getStringField(std::string(fieldname)).c_str()));
            }

        }
        //data->Set (String::New ("t"),Integer::New (doc.getLongField(std::string("t"))));
      
        jobs->Set (i, data);
	}
	hitVo.clear();
    return scope.Close(resObject);
}

Handle<Value> Index(const Arguments& args) {
    HandleScope scope;
    IndexWriter *indexWriter=new IndexWriter();
    String::Utf8Value dictFile(args[0]->ToString());
    std::string dictDir(*dictFile);
    indexWriter->open(dictDir.c_str(),false);
    indexWriter->setBufferDoc(2);
    Local<Array> jsonObject=Local<Array>::Cast(args[1]);
    Array * jsonArray(*jsonObject);
    for(int i=0;i<jsonArray->Length();i++){
        Array *dataArray(*(Local<Array>::Cast(jsonArray->Get(i))));
        Document *doc=new Document();
        for(int j=0;j<dataArray->Length();j++){
            Local<Object> dataObject(*(Local<Object>::Cast(dataArray->Get(j))));
            String::Utf8Value nameChar(dataObject->Get(String::New ("name"))->ToString());
            String::Utf8Value typeChar(dataObject->Get(String::New ("type"))->ToString());
            std::string typeStr(*typeChar);
            String::Utf8Value valueChar(dataObject->Get(String::New ("value"))->ToString());
            Field *t=new Field();
            if(typeStr=="long"){
                t->type= STORE_LONG;
            }
            else{
                t->type=STORE_STRING;
            }
            t->is_store=STORE_YES;
            t->token=STORE_KEYWORD;
            t->name=(*nameChar);
            t->data=(*valueChar);
            std::string nn=(*nameChar);
            doc->addField(nn,t);
       
        }
        indexWriter->add(*doc);
        delete doc;
    }
    
    delete indexWriter;
    Local<Object> resObject = Object::New ();
    resObject->Set (String::New ("status"), String::New ("succ"));
   return scope.Close(resObject);
}
void init(Handle<Object> target) {
    NODE_SET_METHOD(target, "search", Search);
    NODE_SET_METHOD(target, "index", Index);
}

NODE_MODULE(seamSearch, init);