#ifndef LANDSCAPEGENERATOR_H
#define LANDSCAPEGENERATOR_H

#define LANDSCAPE_GEN_TEST_WIDTH 20
#define LANDSCAPE_GEN_TEST_LENGTH 20
#define LANDSCAPE_GEN_TEST_MIN_HEIGHT 0
#define LANDSCAPE_GEN_TEST_MAX_HEIGHT 4

#include "Landscape.h"
#include "TacticsErrors.h"

//TODO: Concurrence-proof this singleton instance
/**
 *	The <pre>LandscapeGenerator</pre> class randomly generates a test landscape.
 */
class LandscapeGenerator
{
public:
	static LandscapeGenerator* getInstance();
	static void destroyInstance();

	ReturnCode GenerateTestLandscape(Landscape* &pobjLandscape);
	ReturnCode DestroyLandscape(Landscape* &landscape);

private:
	//Singleton c'tor d'tor
	LandscapeGenerator();
	~LandscapeGenerator();

	//Singleton instance
	static LandscapeGenerator *m_instance;
};

#endif //#ifndef LANSCAPEGENERATOR_H