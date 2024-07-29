#include <atomic>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
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
    const size_t BUF_SIZE = 1024 * 1024 * 4;
    int fd = open("./data/measurements-10000000.txt", O_RDONLY);

    if (fd == -1) {
        return 1;
    }

    std::unordered_map<std::string, Temps> temps;

    char buf[BUF_SIZE];
    size_t curPos = 0;
    size_t startPos = 0;
    std::string city;
    double temp;
    size_t offset = 0;
    ssize_t end;
    while ((end = read(fd, buf + offset, BUF_SIZE - offset)) > 0)
    {
        if (end == BUF_SIZE - offset) {
            end = BUF_SIZE;
        } else {
            end += offset; // NOLINT(*-narrowing-conversions)
        }
        while (curPos < end) {
            if (buf[curPos] == ';') {
                buf[curPos] = 0;
                city = buf + startPos;
                startPos = curPos + 1;
            }
            if (buf[curPos] == '\n') {
                temp = std::strtod(buf + startPos, nullptr);
                temps[city].numberOfEntries++;
                temps[city].sum += temp;
                temps[city].min = std::min(temps[city].min, temp);
                temps[city].max = std::max(temps[city].max, temp);
                startPos = curPos + 1;
            }
            curPos++;
        }
        std::memmove(buf, buf + startPos, BUF_SIZE - startPos);
        curPos = BUF_SIZE - startPos;
        offset = curPos;
        startPos = 0;
    }

    for (const auto& elem : temps) {
        std::cout << elem.first << "=" << elem.second.min << '/' << (elem.second.sum / elem.second.numberOfEntries) << '/' << elem.second.max << '\n'; // NOLINT(*-narrowing-conversions)
    }

    return 0;
}
