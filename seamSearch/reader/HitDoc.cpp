#include "HitDoc.h"

HitDoc::HitDoc()
{
}

HitDoc::~HitDoc()
{
}
bool HitDoc::operator> (const HitDoc &doc) const{
	return docId<doc.docId;
}
bool HitDoc::operator<( const HitDoc &doc) const{
	return docId>doc.docId;
}

