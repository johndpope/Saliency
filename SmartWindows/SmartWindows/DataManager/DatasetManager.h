//////////////////////////////////////////////////////////////////////////
// manage datasets
// jiefeng@2014-3-22
//////////////////////////////////////////////////////////////////////////


#pragma once

#include "DataManager/VOCDataManager.h"
#include "DataManager/Berkeley3DDataManager.h"


class DatasetManager
{
private:

	VOCDataManager voc07_man;

public:
	DatasetManager(void);



	//////////////////////////////////////////////////////////////////////////
	// database analysis
	//////////////////////////////////////////////////////////////////////////
	// generate positive and negative object windows
	bool GenerateWinSamps(DatasetName dname);

};

