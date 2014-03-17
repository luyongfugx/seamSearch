#ifndef HITDOC_H_
#define HITDOC_H_
#include  "../comm/stdHeader.h"
class HitDoc
{
private:
	int64_t docId;
	int32_t hits;
public:
	HitDoc();
	virtual ~HitDoc();
	int64_t getDocId(){return docId;};
	void setDocId(int64_t id){docId=id;};
	void setHits(int32_t  hi){hits=hi;};
	int32_t getHits(){return hits;};
	bool operator>( const HitDoc  &doc) const;
	bool operator<(const HitDoc &doc) const;
};

#endif /*HITDOC_H_*/
