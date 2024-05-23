#ifndef QUEST4_H
#define QUEST4_H

#include <lib/nlohmann/json.hpp>

#include "src/ward/Ward.h"
#include "src/pedestrian/Pedestrian.h"
#include <vector>
#include <utility> // dung pair

using json = nlohmann::json;

class Quest4{
    protected:
        std::vector<Ward> InListWard;
        int triple;
        int single;
        
    public:
        Quest4();
        std::vector<std::pair<Ward, int>> OutListWardInt(std::vector<Ward> InListWard, int triple, int single);
        std::vector<int> Gauss_Distribution(int noOfWard, int triple, int single);
        std::vector<int> Distribution(int noOfWard, int triple, int single);
        double calculateMean(const std::vector<int>& sample);
        double calculateStdDev(const std::vector<int>& sample, double mean);
        int find_min_value(const std::vector<int>& sample);
        int find_max_value(const std::vector<int>& sample);
};

#endif
