#include "LandscapeGenerator.h"
#include <time.h>
#include <random>

//Singleton instance declaration
LandscapeGenerator* LandscapeGenerator::m_instance = NULL;

LandscapeGenerator::LandscapeGenerator()
{
	//TODO: move this to somewhere more sensible
	//create random seed for later
	srand((unsigned)time(0));
}

LandscapeGenerator::~LandscapeGenerator()
{}

ReturnCode LandscapeGenerator::GenerateTestLandscape(Landscape* &pobjLandscape)
{
	//Assume everything went well until something actually goes wrong
	ReturnCode rc = RC_OK;

	//Generate Landscape Model
	pobjLandscape = new Landscape(LANDSCAPE_GEN_TEST_WIDTH, LANDSCAPE_GEN_TEST_LENGTH);
	for(int i=0; i<LANDSCAPE_GEN_TEST_WIDTH; ++i)
	{
		for(int j=0; j<LANDSCAPE_GEN_TEST_LENGTH; ++j)
		{
			//Randomly generate height
			int iBase = 0;
			int iHeight = (LANDSCAPE_GEN_TEST_MIN_HEIGHT + 1) + rand() % LANDSCAPE_GEN_TEST_MAX_HEIGHT; //1 - 4

			//Create GridUnit.  If fails, we'll continue trying to create, let someone else handle deallocation
			GridUnit *pobjUnit = new GridUnit(iBase, iHeight, i, j, GrassTerrain);
			if(RC_FAILED(pobjLandscape->setGridUnit(i,j, pobjUnit)))
				rc = RC_ERR_GENERAL;
		}
	}

	//Create Landscape Controller
	LandscapeController* pobjLandscapeController = new LandscapeController();
	pobjLandscape->setLandscapeController(pobjLandscapeController);
	
	//Return success/failure status
	return rc;
}

ReturnCode LandscapeGenerator::DestroyLandscape(Landscape* &landscape)
{
	//Assume everything went well until something actually goes wrong
	ReturnCode rc = RC_OK;

	if(landscape)
	{
		//Delete each individual grid unit
		for(int i=0; i<landscape->getGridWidth(); ++i)
		{
			for(int j=0; j<landscape->getGridLength(); ++j)
			{
				//TODO: handle deletion of GridUnit columns when implemented
				GridUnit *pobjUnit = NULL;
				if(RC_FAILED(landscape->getGridUnit(i, j, pobjUnit)))
					rc = RC_ERR_GENERAL;

				if(pobjUnit)
				{
					delete pobjUnit;
					pobjUnit = NULL;
				}
			}
		}

		//Delete the landscape itself
		delete landscape;
		landscape = NULL;
	}

	return rc;
}

LandscapeGenerator* LandscapeGenerator::getInstance()
{
	if(m_instance)
		return m_instance;
	return m_instance = new LandscapeGenerator();
}

void LandscapeGenerator::destroyInstance()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}