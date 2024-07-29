#include <cstdio>
#include <fstream>
#include <iostream>
#include <unordered_map>

struct Temps {
    size_t numberOfEntries = 0;
    double sum = 0;
    double min = 10000.0;
    double max = -10000.0;
};

int main() {
    std::ifstream infile("./data/measurements-10000000.txt");

    if (!infile.is_open()) {
        return 1;
    }

    std::unordered_map<std::string, Temps> temps;

    std::string line;
    while (std::getline(infile, line))
    {
        std::string city;
        size_t i;
        for (i = 0;; i++) {
            if (line[i] == ';') {
                city = line.substr(0, i);
                break;
            }
        }
        double temp = std::stod(line.substr(i + 1));
        temps[city].numberOfEntries++;
        temps[city].sum += temp;
        temps[city].min = std::min(temps[city].min, temp);
        temps[city].max = std::max(temps[city].max, temp);
    }

    for (const auto& elem : temps) {
        std::cout << elem.first << "=" << elem.second.min << '/' << (elem.second.sum / elem.second.numberOfEntries) << '/' << elem.second.max << '\n'; // NOLINT(*-narrowing-conversions)
    }

    return 0;
}
