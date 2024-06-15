#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <vector>

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

// For exchanging data between C++ and Python
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

//////////////////////////////////////
///////// Integer Generators /////////
//////////////////////////////////////

std::vector<int> uniformDistribution(int lowerBound, int upperBound, int num)
{
    std::vector<int> vals;
    std::default_random_engine rand;
    std::uniform_int_distribution<int> range(lowerBound, upperBound);

    for (int i = 0; i < num; i++)
    {
        vals.push_back(range(rand));
    }

    return vals;
}

std::vector<int> normalDistribution(float mean, float stddev, int num)
{
    std::vector<int> vals;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> normal{mean, stddev};

    for (int i = 0; i < num; i++)
    {
        int generated_number = normal(gen);
        vals.push_back(static_cast<int>(generated_number));
    }

    return vals;
}

std::vector<int> integerSequence(int start, int end)
{
    std::vector<int> vals;

    for (int i = start; i < end; i++)
    {
        if (i != end - 1)
        {
            vals.push_back(i);
        };

        if (i == end - 1)
        {
            vals.push_back(i);
            vals.push_back(i + 1);
        };
    };

    return vals;
}

//////////////////////////////////////
////// Date and Time Generators //////
//////////////////////////////////////

// Q: Return array of datetime.datetime objects, or date-formatted strings?
// A: datetime.datetime allows the format to be handled dynamically in Python
// std::vector<std::chrono::time_point> randomDate(std::string start_date, std::string end_date, int num)
// {
// }

std::vector<std::string> randomDate(std::string start_date, std::string end_date, int num)
{

    std::map<int, int> days_per_month{
        {1, 31},
        {2, 28},
        {3, 31},
        {4, 30},
        {5, 31},
        {6, 30},
        {7, 31},
        {8, 31},
        {9, 30},
        {10, 31},
        {11, 30},
        {12, 31}};

    std::vector<std::string> vals;

    // Expects "yyyy/mm/dd"
    // Indices "0123/56/89"
    int start_year = stoi(start_date.substr(0, 4));
    int start_month = stoi(start_date.substr(5, 2));
    int start_day = stoi(start_date.substr(8, 2));

    int end_year = stoi(end_date.substr(0, 4));
    int end_month = stoi(end_date.substr(5, 2));
    int end_day = stoi(end_date.substr(8, 2));

    /*
    1. Get random year between start and end year
    2. If year = start_year or year = end_year, generate month between (start_month, 12) or (1, end_month)
    3.
    */

    std::default_random_engine rand;
    for (int i = 0; i < num; i++)
    {
        int int_month;
        int int_day;

        // Generate random year between start_year and end_year
        std::uniform_int_distribution<int> rand_year(start_year, end_year);
        int year = rand_year(rand);

        // Generate random month taking into account interaction between
        // start_year<->start_month and end_year<->end_month
        std::uniform_int_distribution<int> rand_month(1, 12);
        std::uniform_int_distribution<int> rand_start_month(start_month, 12);
        std::uniform_int_distribution<int> rand_end_month(1, end_month);
        std::uniform_int_distribution<int> rand_start_end_month(start_month, end_month);

        if (year == start_year && year == end_year)
        {
            int_month = rand_start_end_month(rand);
        }
        else if (year == start_year)
        {
            int_month = rand_start_month(rand);
        }
        else if (year == end_year)
        {
            int_month = rand_end_month(rand);
        }
        else
        {
            int_month = rand_month(rand);
        };

        std::string str_month = std::string(2 - std::to_string(int_month).length(), '0').append(std::to_string(int_month));

        // Generate random day

        std::uniform_int_distribution<int> rand_day(1, 31);
        std::uniform_int_distribution<int> rand_start_day(start_day, 31);
        std::uniform_int_distribution<int> rand_end_day(1, end_day);

        if (int_month == start_month)
        {
            int_day = rand_start_day(rand);
        }
        else if (int_month == end_month)
        {
            int_day = rand_end_day(rand);
        }
        else
        {
            int_day = rand_day(rand);
        };

        // Handle day values greater than number of days in the month
        // TODO: Leap years
        while (int_day > days_per_month[int_month])
        {
            --int_day;
        };

        std::string str_day = std::string(2 - std::to_string(int_day).length(), '0').append(std::to_string(int_day));

        vals.push_back(std::to_string(year) + "/" + str_month + "/" + str_day);
    }

    return vals;
}

// ChatGPT Solution
std::chrono::year_month_day parse_date(const std::string &date_str)
{
    std::istringstream iss(date_str);
    int year, month, day;
    char sep;
    iss >> year >> sep >> month >> sep >> day;
    return std::chrono::year_month_day{std::chrono::year{static_cast<int>(year)}, std::chrono::month{static_cast<unsigned int>(month)}, std::chrono::day{static_cast<unsigned int>(day)}};
}

std::string format_date(const std::chrono::year_month_day &ymd)
{
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << int(ymd.year()) << '/'
        << std::setw(2) << std::setfill('0') << unsigned(ymd.month()) << '/'
        << std::setw(2) << std::setfill('0') << unsigned(ymd.day());
    return oss.str();
}

std::string generate_random_date(const std::string &start_date_str, const std::string &end_date_str)
{
    auto start_date = parse_date(start_date_str);
    auto end_date = parse_date(end_date_str);

    std::chrono::sys_days start_days = start_date;
    std::chrono::sys_days end_days = end_date;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::chrono::days::rep> dist((end_days - start_days).count());

    std::chrono::sys_days random_days = start_days + std::chrono::days{dist(gen)};
    std::chrono::year_month_day random_date = random_days;

    return format_date(random_date);
}
///////////////////////////////////////
/////////////// Binding ///////////////
///////////////////////////////////////

namespace nb = nanobind;
using namespace nb::literals;

NB_MODULE(fablegen, m)
{
    m.def("uniform_distribution", &uniformDistribution,
          "lower_bound"_a, "upper_bound"_a, "num_items"_a,
          "Generates a uniform distribution between the provided upper and lower bound");
    m.def("normal_distribution", &normalDistribution);
    m.def("integer_sequence", &integerSequence);
    m.def("random_date", &randomDate);
    m.def("chatgpt_date", &generate_random_date);
}
