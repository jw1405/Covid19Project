#include "CovidRecord.h"

CovidRecord::CovidRecord(
    
    //Create variable to be used
    std::string _date,
    int _dailyConfirmed,
    int _cumulativeConfirmed,
    int _dailyImported,
    int _dailyLocal,
    int _stillHospitalised,
    int _dailyDeaths,
    int _cumulativeDeaths,
    int _icuCases,
    std::optional<std::string> _phase,
    std::optional<long long> _cumulativeVaccineDoses
)
{
    date = _date;

    dailyConfirmed = _dailyConfirmed;
    cumulativeConfirmed = _cumulativeConfirmed;

    dailyImported = _dailyImported;
    dailyLocal = _dailyLocal;

    stillHospitalised = _stillHospitalised;

    dailyDeaths = _dailyDeaths;
    cumulativeDeaths = _cumulativeDeaths;

    icuCases = _icuCases;

    phase = _phase;
    cumulativeVaccineDoses = _cumulativeVaccineDoses;
}