#include "MathUtil.h"


/**
*	@brief Converts the number of events per second to the number of seconds between each event.
*	@param [IN] fFreqInSeconds - The number of events per second.  Must not be <= 0.
*	@param [OUT] fSecondsPer - The number of seconds between each event
*	@return RC_OK on success.  RC_ERR_INVALID_ARGS is fFreqInSeconds <= 0.
*/
ReturnCode MathUtil::FrequencyToSeconds(float fFreqInSeconds, float &fSecondsPer)
{
	//Check for bad input
	if(fFreqInSeconds <= 0.0f)
		return RC_ERR_INVALID_ARGS;

	//Make conversion and return successfully
	fSecondsPer = 1.0f/fFreqInSeconds;
	return RC_OK;
}

/**
*	@brief Converts the number of events per milisecond to the number of miliseconds between each event.
*	@param [IN] fFreqInMilis - The number of events per milisecond.  Must not be <= 0.
*	@param [OUT] fMilisPer - The number of miliseconds between each event
*	@return RC_OK on success.  RC_ERR_INVALID_ARGS is fFreqInMilis <= 0.
*/
ReturnCode MathUtil::FrequencyToMiliseconds(float fFreqInMilis, float &fMilisPer)
{
	RET_IF_FAILED(FrequencyToSeconds(1000.0f*fFreqInMilis, fMilisPer));
	fMilisPer *= 1000.0f;
	return RC_OK;
}