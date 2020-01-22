
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"

#include "CombFilterIf.h"
#include "CombFilter.h"

static const char*  kCMyProjectBuildDate             = __DATE__;


CCombFilterIf::CCombFilterIf () :
    m_bIsInitialized(false),
    m_pCCombFilter(0),
    m_fSampleRate(0)
{
    // this never hurts
    this->reset ();
}


CCombFilterIf::~CCombFilterIf ()
{
    this->reset ();
}

const int  CCombFilterIf::getVersion (const Version_t eVersionIdx)
{
    int iVersion = 0;

    switch (eVersionIdx)
    {
    case kMajor:
        iVersion    = MUSI6106_VERSION_MAJOR; 
        break;
    case kMinor:
        iVersion    = MUSI6106_VERSION_MINOR; 
        break;
    case kPatch:
        iVersion    = MUSI6106_VERSION_PATCH; 
        break;
    case kNumVersionInts:
        iVersion    = -1;
        break;
    }

    return iVersion;
}
const char*  CCombFilterIf::getBuildDate ()
{
    return kCMyProjectBuildDate;
}

Error_t CCombFilterIf::create( CCombFilterIf*& pCCombFilter)
{
    pCCombFilter = new CCombFilterIf();
    return kNoError;
}

Error_t CCombFilterIf::destroy (CCombFilterIf*& pCCombFilter)
{
    delete pCCombFilter;
    pCCombFilter = nullptr;
    return kNoError;
}

Error_t CCombFilterIf::init( CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels )
{
    m_fSampleRate = fSampleRateInHz;
    int delayLength = (int)(fMaxDelayLengthInS * fSampleRateInHz);
    if (eFilterType == kCombFIR) {
        m_pCCombFilter = new CCombFilterFIR(delayLength, iNumChannels);
    }
    else if (eFilterType == kCombIIR) {
        m_pCCombFilter = new CCombFilterIIR(delayLength, iNumChannels);
    }
    m_bIsInitialized = true;
    return kNoError;
}

Error_t CCombFilterIf::reset ()
{
    return kNoError;
}

Error_t CCombFilterIf::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    m_pCCombFilter->filter(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames);
    return kNoError;
}

Error_t CCombFilterIf::setParam( FilterParam_t eParam, float fParamValue )
{
    return kNoError;
}

float CCombFilterIf::getParam( FilterParam_t eParam ) const
{
    return 0;
}
