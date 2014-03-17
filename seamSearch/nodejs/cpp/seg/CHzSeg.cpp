#include "CHzSeg.h"
#include "Dict.h"
#include <assert.h>
const unsigned int MAX_WORD_LENGTH = 12;
const unsigned int CHA_LENGTH =3;
const string SEPARATOR("/  "); // delimiter between words
CHzSeg::CHzSeg() {
}

CHzSeg::~CHzSeg() {
}

void  CHzSeg::SegmentHzStrMM(CDict &dict, std::string s1) {
	while (!s1.empty()) { 
		unsigned int len=s1.size();
		if (len>MAX_WORD_LENGTH) len=MAX_WORD_LENGTH;
		string w=s1.substr(0, len);// the candidate word
		bool isw=dict.IsWord(w);

		while (len> CHA_LENGTH && isw==false) {	// if not a word
			len-= CHA_LENGTH;		// cut a word
			w=w.substr(0, len);
			isw=dict.IsWord(w);
		}
		addTerm(w);
//		s2 += w + SEPARATOR;
		s1 = s1.substr(w.size()); 
	}
}
void CHzSeg::SegmentSentenceMM(CDict &dict, std::string s1) {
	
	unsigned int i,len;
	unsigned char c;

	while (!s1.empty()) 
	{		
		i = 0;
		len = s1.size();
		c=s1[i];
		while ( i<len && c<128 && !( (c>='a' && c<='z')||(c>='A' && c<='Z')||(c>='0' && c<='9') ) )
		{ 
			i++;
			c=s1[i];
		}
		if (i >= len)	//i has come to the end of s1
		{
			break;
		}
		if (i > 0)
		{
			s1 = s1.substr(i);
		}
		if(s1.empty())
			break;

		unsigned char ch=(unsigned char) s1[0];

		if(ch < 128) // deal with ASCII
		{ 		
			i = 0;
			len = s1.size();
			c=s1[i];
			unsigned char cPre=c;
			while(i<len && c<128 && 
				( ((c>='a' && c<='z')||(c>='A' && c<='Z')) 
					&& ((cPre>='a' && cPre<='z')||(cPre>='A' && cPre<='Z')) )
				|| ((c>='0' && c<='9')&&(cPre>='0' && cPre<='9')) 
				)
			{
				i++;
				c=s1[i];
			}
            addTerm(s1.substr(0, i));
			if (i < len)
			{
				s1 = s1.substr(i);
				continue;
			}
			else
			{
				break;
			}

		} 
		else if (ch < 176)
		{ 
			i = 0;
			len = s1.length();
			while(i<len && ((unsigned char)s1[i]<176) && ((unsigned char)s1[i]>=161)
			&& (!((unsigned char)s1[i]==161 && ((unsigned char)s1[i+1]>=162 && (unsigned char)s1[i+1]<=168)))
			&& (!((unsigned char)s1[i]==161 && ((unsigned char)s1[i+1]>=171 && (unsigned char)s1[i+1]<=191)))
			&& (!((unsigned char)s1[i]==163 && ((unsigned char)s1[i+1]==172 || (unsigned char)s1[i+1]==161) 
			|| (unsigned char)s1[i+1]==168 || (unsigned char)s1[i+1]==169 || (unsigned char)s1[i+1]==186
			|| (unsigned char)s1[i+1]==187 || (unsigned char)s1[i+1]==191))) { 
				i=i+ CHA_LENGTH; 
			}

			if (i==0) i=i+ CHA_LENGTH;


			if (!(ch==161 && (unsigned char)s1[1]==161)) 
			{ 
				if (i <= s1.size())
	
	            addTerm(s1.substr(0, i));
				else break; 
			}

			if (i <= s1.size())
				s1=s1.substr(i);
			else break;
			continue;
		}
    



		i =  CHA_LENGTH;
		len = s1.length();
		while(i<len && (unsigned char)s1[i]>=176) 
			i+= CHA_LENGTH;

	    SegmentHzStrMM(dict, s1.substr(0,i));
		if (i < len)
			s1=s1.substr(i);
		else break;
	}
}
void  CHzSeg::addTerm(std::string term){
	std::map<std::string,int>::iterator it=termMap.find(term);
	if(it!=termMap.end()){
		termMap[term]=(it->second+1);
	}
	else{
		termMap[term]=1;
	}
}
