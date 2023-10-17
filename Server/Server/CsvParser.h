#pragma once
#include <iterator>

#define MAX_LEN		1024

struct Parse;
typedef std::vector<Parse> CSVDATAVEC;

struct Parse
{
public:
	Parse(std::string pItem)
	{
		strItem_ = pItem;
	};

	int getInt(CSVDATAVEC::iterator& pPos) const
	{
		pPos++;
		return atoi(strItem_.c_str());
	}

	INT64 getInt64(CSVDATAVEC::iterator& pPos) const
	{
		pPos++;
		return _atoi64(strItem_.c_str());
	}

	double getDouble(CSVDATAVEC::iterator& pPos) const
	{
		pPos++;
		return atof(strItem_.c_str());
	}

	std::string getString(CSVDATAVEC::iterator& pPos) const
	{
		pPos++;
		return strItem_;
	}

	const char* getChar(CSVDATAVEC::iterator& pPos)
	{
		pPos++;
		return strItem_.c_str();
	}

	int parseItem(std::string pItem, CSVDATAVEC::iterator& pPos)
	{
		int result = strItem_.compare(pItem);

		if(!result)
			pPos++;

		return result;
	}

	int parseItemNoPos(std::string pItem)
	{
		return strItem_.compare(pItem);
	}

	bool operator==(const Parse& pParse)
	{
		return !strItem_.compare(pParse.getString_());
	}

private:
	std::string strItem_;
	std::string getString_() const {return strItem_;}
};

class CSVParser
{
public:
	CSVParser(std::string pFileDir);

	const CSVDATAVEC& getCSVDataVec() const { return dataVec_; };

	~CSVParser();

private:
	void parseCSVData_(std::string pFileDir);
	CSVDATAVEC dataVec_;
};

#define SWITCH(X)		for(CSVDATAVEC::iterator pos = X.begin(); pos != X.end();pos++)
#define CASE(X)			if(!pos->parseItem(#X, pos))
#define WHILE(X)		while(pos->parseItem(#X, pos))
#define WHILENOPOS(X)	while(pos->parseItemNoPos(#X))

#define GET_CHAR()		pos->getChar(pos)
#define GET_INT()		pos->getInt(pos)
#define GET_INT64()		pos->getInt64(pos)
#define GET_DOUBLE()	pos->getDouble(pos)
#define GET_STR()		pos->getString(pos)

#define SET_CHAR(X)		X = pos->getChar(pos)
#define SET_INT(X)		X = pos->getInt(pos)
#define SET_INT64(X)	X = pos->getInt64(pos)
#define SET_DOUBLE(X)	X = pos->getDouble(pos)
#define SET_STR(X)		X = pos->getString(pos)