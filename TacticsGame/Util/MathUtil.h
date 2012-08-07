#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "TacticsErrors.h"

class MathUtil
{
public:
	/**
	 *	@brief Converts the number of events per second to the number of seconds between each event.
	 *	@param [IN] fFreqInSeconds - The number of events per second.  Must not be <= 0.
	 *	@param [OUT] fSecondsPer - The number of seconds between each event
	 *	@return RC_OK on success.  RC_ERR_INVALID_ARGS is fFreqInSeconds <= 0.
	 */
	static ReturnCode FrequencyToSeconds(float fFreqInSeconds, float &fSecondsPer);

	/**
	*	@brief Converts the number of events per milisecond to the number of miliseconds between each event.
	*	@param [IN] fFreqInMilis - The number of events per milisecond.  Must not be <= 0.
	*	@param [OUT] fMilisPer - The number of miliseconds between each event
	*	@return RC_OK on success.  RC_ERR_INVALID_ARGS is fFreqInMilis <= 0.
	 */
	static ReturnCode FrequencyToMiliseconds(float fFreqInMilis, float &fMilisPer);

protected:

private:

};

#endif //#ifndef MATHUTIL_H