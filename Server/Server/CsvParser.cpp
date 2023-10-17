#include "StdAfx.h"

CSVParser::CSVParser(std::string pFileDir)
{
	if (!strcmp(pFileDir.c_str(), "\0"))
		return;

	parseCSVData_(pFileDir);
}

void CSVParser::parseCSVData_(std::string pFileDir)
{
	FILE* csvFile = fopen(pFileDir.c_str(), "r");

	if (NULL == csvFile)
		return;

	char line[MAX_LEN] = "\0";
	Parse endSymbol("\0");

	while (NULL != fgets(line, MAX_LEN, csvFile))
	{
		if (strlen(line) >= MAX_LEN || strlen(line) < 1)
			continue;

		if ('\n' == line[strlen(line) - 1])
			line[strlen(line) - 1] = '\0';

		if (!strcmp(line, "\0"))
			continue;

		char* csvLine = strtok(line, ",");

		if (!strcmp(csvLine, "\0"))
			continue;

		Parse csvKey(csvLine);

		dataVec_.push_back(csvKey);

		while (NULL != csvLine)
		{
			csvLine = strtok(NULL, ",");

			if (NULL == csvLine)
				break;

			if (!strcmp(csvLine, ""))
				continue;

			Parse csvValue(csvLine);

			dataVec_.push_back(csvValue);
		}
	}

	dataVec_.push_back(endSymbol);

	fclose(csvFile);
}

CSVParser::~CSVParser()
{
	dataVec_.clear();
}