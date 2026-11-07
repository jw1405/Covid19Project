#include "CsvReader.h"

#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept> 
#include <optional> //Allows a variable to contain a value or no value. (Data Files will have some empty values)

// 1. Tokenise CSV line
std::vector<std::string> tokeniseCSV(
    const std::string& line
)
{
    std::vector<std::string> tokens;

    std::string token;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.length(); i++)
    {
        char current = line[i];

        if (current == '"')
        {
            // Handle escaped double quotes
            if (insideQuotes &&
                i + 1 < line.length() &&
                line[i + 1] == '"')
            {
                token += '"';
                i++;
            }
            else
            {
                insideQuotes = !insideQuotes;
            }
        }
        else if (current == ',' && !insideQuotes)
        {
            tokens.push_back(token);
            token.clear();
        }
        else
        {
            token += current;
        }
    }

    if (insideQuotes)
    {
        throw std::invalid_argument(
            "Unclosed quotation mark"
        );
    }

    tokens.push_back(token);

    return tokens;
}


// 2. Convert string into integer
int convertToInt(const std::string& value)
{
    size_t position = 0;

    int result = std::stoi(value, &position);

    // Ensure the entire field was converted
    if (position != value.length())
    {
        throw std::invalid_argument(
            "Invalid integer: " + value
        );
    }

    return result;
}


// 3. Convert optional string into long long
std::optional<long long> convertOptionalLong(
    const std::string& value
)
{
    if (value.empty())
    {
        return std::nullopt;
    }

    size_t position = 0;

    long long result = std::stoll(value, &position);

    if (position != value.length())
    {
        throw std::invalid_argument(
            "Invalid number: " + value
        );
    }

    return result;
}


// 4. Read CSV file
void readCovidCsv(
    const std::string& filename,
    std::vector<CovidRecord>& records,
    int& invalidRows
)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Unable to open CSV file: " + filename
        );
    }

    std::string line;

    // 5. Read CSV header
    if (!std::getline(file, line))
    {
        throw std::runtime_error("CSV file is empty");
    }

    std::vector<std::string> headers = tokeniseCSV(line);

    // Remove UTF-8 BOM if present
    if (!headers.empty() &&
        headers[0].substr(0, 3) == "\xEF\xBB\xBF")
    {
        headers[0].erase(0, 3);
    }

    // 6. Map column names to their positions
    std::map<std::string, size_t> columnIndex;

    for (size_t i = 0; i < headers.size(); i++)
    {
        columnIndex[headers[i]] = i;
    }

    // Helper to find a required column
    auto getIndex = [&](const std::string& name)
    {
        auto it = columnIndex.find(name);

        if (it == columnIndex.end())
        {
            throw std::runtime_error(
                "Missing CSV column: " + name
            );
        }

        return it->second;
    };

    // 7. Find the columns we need
    size_t dateIndex =
        getIndex("Date");

    size_t dailyConfirmedIndex =
        getIndex("Daily Confirmed");

    size_t cumulativeConfirmedIndex =
        getIndex("Cumulative Confirmed");

    size_t dailyImportedIndex =
        getIndex("Daily Imported");

    size_t dailyLocalIndex =
        getIndex("Daily Local transmission");

    size_t hospitalisedIndex =
        getIndex("Still Hospitalised");

    size_t dailyDeathsIndex =
        getIndex("Daily Deaths");

    size_t cumulativeDeathsIndex =
        getIndex("Cumulative Deaths");

    size_t icuIndex =
        getIndex("Intensive Care Unit (ICU)");

    size_t phaseIndex =
        getIndex("Phase");

    size_t vaccineIndex =
        getIndex("Cumulative Vaccine Doses");


    // 8. Read each data row
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        try
        {
            // Tokenise all 36 columns
            std::vector<std::string> tokens =
                tokeniseCSV(line);

            // Check the number of columns
            if (tokens.size() != headers.size())
            {
                throw std::invalid_argument(
                    "Incorrect number of columns"
                );
            }

            // 9. Convert selected fields
            std::string date = tokens[dateIndex];

            if (date.empty())
            {
                throw std::invalid_argument(
                    "Missing date"
                );
            }

            int dailyConfirmed =
                convertToInt(tokens[dailyConfirmedIndex]);

            int cumulativeConfirmed =
                convertToInt(tokens[cumulativeConfirmedIndex]);

            int dailyImported =
                convertToInt(tokens[dailyImportedIndex]);

            int dailyLocal =
                convertToInt(tokens[dailyLocalIndex]);

            int hospitalised =
                convertToInt(tokens[hospitalisedIndex]);

            int dailyDeaths =
                convertToInt(tokens[dailyDeathsIndex]);

            int cumulativeDeaths =
                convertToInt(tokens[cumulativeDeathsIndex]);

            int icu =
                convertToInt(tokens[icuIndex]);


            // 10. Handle optional fields
            std::optional<std::string> phase;

            if (!tokens[phaseIndex].empty())
            {
                phase = tokens[phaseIndex];
            }

            std::optional<long long> vaccineDoses =
                convertOptionalLong(tokens[vaccineIndex]);


            // 11. Create CovidRecord object
            CovidRecord record(
                date,
                dailyConfirmed,
                cumulativeConfirmed,
                dailyImported,
                dailyLocal,
                hospitalised,
                dailyDeaths,
                cumulativeDeaths,
                icu,
                phase,
                vaccineDoses
            );


            // 12. Store object in vector
            records.push_back(record);
        }
        catch (const std::exception& e)
        {
            std::cout << "Invalid CSV row: "
                      << e.what()
                      << std::endl;

            invalidRows++;
        }
    }

    file.close();
}