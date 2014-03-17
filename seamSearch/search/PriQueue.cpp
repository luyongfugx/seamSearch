#include "PriQueue.h"

PriQueue::PriQueue()
{

}

PriQueue::~PriQueue()
{
	reVo.clear();
}
void  PriQueue::put(std::vector<HitDoc> hitVo){
	if(hitVo.size()>0)
		reVo.push_back(hitVo);
}

void  PriQueue::getResult(std::vector<HitDoc> &hitVo){
	if(reVo.size()<=1)
		   return;
	std::priority_queue<int64_t,std::vector<HitDoc> > Pqueue;
	std::vector<HitDoc> firstVo=reVo.front();
	HitDoc hdoc=firstVo.front();
	
	std::vector<HitDoc> &inVo=reVo.front();//使用引用删除队列的头一个元素
	inVo.erase(inVo.begin());

	std::vector<HitDoc> firstVor=(*reVo.begin());

	
	bool isEnd=true;
	while(isEnd){
		bool isDoc=true;
		for(std::vector< std::vector<HitDoc> >::iterator it=(reVo.begin()+1);it!=reVo.end();it++){
			std::vector<HitDoc> &tempVo=(*it);//得到引用，必须加 &
			HitDoc tempdoc=tempVo.front();//取得第一个元素
			if(hdoc.getDocId()==tempdoc.getDocId()){//如果第一个doc相等
				hdoc.setHits(hdoc.getHits()+tempdoc.getHits());
				tempVo.erase(tempVo.begin());//删除相等元素
				if(tempVo.size()==0||reVo.front().size()==0){
					isEnd=false;
				}
			}
			else{
				isDoc=false;
				if(tempVo.size()==0||reVo.front().size()==0){
							isEnd=false;
				}
			}
		}
	
		if(isDoc){
			Pqueue.push(hdoc);
		//	hitVo.push_back(hdoc);
		}
		if(!isEnd)
			   break;
		sort();
		firstVo=(*reVo.begin());
		 hdoc=firstVo.front();
		std::vector<HitDoc> &inVo1=reVo.front();//使用引用删除队列的头一个元素
		inVo1.erase(inVo1.begin());
	}
	 int size=Pqueue.size();
	for(int i=0;i<size;i++){
		HitDoc  hdocx=Pqueue.top();
		hitVo.push_back( hdocx);
		Pqueue.pop();
	}
}
void  PriQueue::sort(){
	std::vector<HitDoc> firstVo=reVo.front();
	HitDoc hdoc=firstVo.front();
	reVo.erase(reVo.begin());
	std::vector< std::vector<HitDoc> > tempReVo;
	
	for(std::vector< std::vector<HitDoc> >::iterator it=(reVo.begin());it!=reVo.end();it++){
		std::vector<HitDoc> tempVo=(*it);
		HitDoc tempdoc=(*tempVo.begin());
		if(hdoc.getDocId()<tempdoc.getDocId()){
			tempReVo.push_back(tempVo);
		}
		else{
			hdoc=tempVo.front();
			std::vector<HitDoc> tempVo1=firstVo;
			firstVo=tempVo;
			tempVo=tempVo1;
			tempReVo.push_back(tempVo);
		}
	}
	reVo.clear();
	reVo.push_back(firstVo);
	for(std::vector< std::vector<HitDoc> >::iterator it=(tempReVo.begin());it!=tempReVo.end();it++){
		reVo.push_back((*it));
	}
	tempReVo.clear();
}
