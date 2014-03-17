#ifndef PRIQUEUE_H_
#define PRIQUEUE_H_
#include "../reader/IndexReader.h"
#include "../seg/Dict.h"
#include "../seg/CHzSeg.h"
using namespace std;
class PriQueue
{
private:
	std::vector<std::vector<HitDoc> > reVo;//存放结果集
	void sort();
public:
	PriQueue();
	virtual ~PriQueue();
	void  put(std::vector<HitDoc> hitVo);
	void getResult(std::vector<HitDoc> &hitVo);
	
};

#endif /*PRIQUEUE_H_*/
