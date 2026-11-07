#ifndef COVIDRECORD_H
#define COVIDRECORD_H

#include <string>
#include <optional>

class CovidRecord
{
public:

    // Data members
    std::string date;

    int dailyConfirmed;
    int cumulativeConfirmed;

    int dailyImported;
    int dailyLocal;

    int stillHospitalised;

    int dailyDeaths;
    int cumulativeDeaths;

    int icuCases;

    std::optional<std::string> phase;
    std::optional<long long> cumulativeVaccineDoses;

    // Constructor
    CovidRecord(
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
    );
};

#endif