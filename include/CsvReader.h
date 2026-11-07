#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>

#include "CovidRecord.h"

// Read CSV file and store records
void readCovidCsv(
    const std::string& filename,
    std::vector<CovidRecord>& records,
    int& invalidRows
);

#endif