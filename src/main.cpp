#include <iostream>
#include <vector>
#include <algorithm>

#include "CovidRecord.h"
#include "CsvReader.h"

int main()
{
    // 1. Create vector to store COVID records
    std::vector<CovidRecord> records;

    int invalidRows = 0;

    try
    {
        // 2. Read CSV file
        readCovidCsv(
            "data/covid19_sg.csv",
            records,
            invalidRows
        );

        // 3. Print summary statistics
        std::cout << "\n";
        std::cout << "================================"
                  << std::endl;

        std::cout << " SINGAPORE COVID-19 DATA SUMMARY"
                  << std::endl;

        std::cout << "================================"
                  << std::endl;

        std::cout << "Valid records: "
                  << records.size()
                  << std::endl;

        std::cout << "Invalid records: "
                  << invalidRows
                  << std::endl;


        // 4. Display first and last dates
        if (!records.empty())
        {
            std::cout << "First date: "
                      << records.front().date
                      << std::endl;

            std::cout << "Last date: "
                      << records.back().date
                      << std::endl;
        }


        // 5. Find highest daily confirmed cases
        if (!records.empty())
        {
            auto highestCases = std::max_element(
                records.begin(),
                records.end(),
                [](const CovidRecord& a,
                   const CovidRecord& b)
                {
                    return a.dailyConfirmed <
                           b.dailyConfirmed;
                }
            );

            std::cout << "Highest daily cases: "
                      << highestCases->dailyConfirmed
                      << std::endl;

            std::cout << "Date of highest cases: "
                      << highestCases->date
                      << std::endl;
        }


        // 6. Count records with missing vaccine data
        int missingVaccineData = 0;

        for (const CovidRecord& record : records)
        {
            if (!record.cumulativeVaccineDoses.has_value())
            {
                missingVaccineData++;
            }
        }

        std::cout << "Missing vaccine records: "
                  << missingVaccineData
                  << std::endl;


        // 7. Display first 5 records
        std::cout << "\n===== FIRST 5 RECORDS ====="
                  << std::endl;

        for (size_t i = 0;
             i < records.size() && i < 5;
             i++)
        {
            std::cout << records[i].date
                      << " | Cases: "
                      << records[i].dailyConfirmed
                      << " | Deaths: "
                      << records[i].dailyDeaths
                      << " | ICU: "
                      << records[i].icuCases
                      << std::endl;
        }

        std::cout << "\nProgram completed successfully!"
                  << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Program error: "
                  << e.what()
                  << std::endl;

        return 1;
    }

    return 0;
}