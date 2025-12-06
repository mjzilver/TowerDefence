#pragma once

#include <string>
#include <vector>

static std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::string current;

    for (char c : text) {
        if (c == '\n') {
            lines.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }

    lines.push_back(current);
    return lines;
}
