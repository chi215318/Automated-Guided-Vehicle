#include "Quest4.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

#include "src/ward/Ward.h"

// tim gia tri nho nhat trong vector
int find_min_value(const std::vector<int>& sample){
    int min = INT_MAX;
    for (int v: sample)
        min = (v < min) ? v : min;
    return min;
}

// tim gia tri lon nhat trong vector
int find_max_value(const std::vector<int>& sample){
    int max = 0;
    for (int v: sample)
        max = (v < max) ? v : max;
    return max;
}

// Hàm tính giá trị trung bình của vector
double Quest4::calculateMean(const std::vector<int>& sample) {
    double sum = 0.0;
    for (double value : sample) {
        sum += value;
    }
    return sum / sample.size();
}

// Hàm tính độ lệch chuẩn của vector
double Quest4::calculateStdDev(const std::vector<int>& sample, double mean) {
    double variance = 0.0;
    for (int value : sample) {
        variance += pow((value - mean), 2);
    }
    return sqrt(variance / sample.size());
}

// sinh ra vector các int sao cho tổng int thỏa mãn yc và tuân theo phân phối chuẩn
std::vector<int> Quest4::Gauss_Distribution(int noOfWard, int triple, int single){
    int noOfPoint = triple * 3 + single;
    std::default_random_engine gen;
    std::vector<int> ListInt;
    bool isFine = false;

    while(!isFine){
        // sinh ra số lượng Int = noOfWard
        for(int i = 0; i < noOfWard; i++){
            std::uniform_int_distribution<int> dis(1, noOfPoint);
            int ran_int = dis(gen);
            noOfPoint -= ran_int;
            ListInt.push_back(ran_int);
        }
        if (ListInt.size() != noOfWard){
            noOfPoint = triple * 3 + single;
            continue;
        }

        // kiểm tra phân phối 
        double mean = calculateMean(ListInt);
        double stdDev = calculateStdDev(ListInt, mean);
        int min_value = find_min_value(ListInt);
        int max_value = find_max_value(ListInt); 

        // Tính p-value
        double z = (mean - ((min_value + max_value) / 2)) / (stdDev / sqrt(ListInt.size()));
        double p_value = 2 * (1 - 0.5 * (1 + erf(fabs(z) / sqrt(2))));

        // Đặt mức ý nghĩa thống kê (significance level) là 0.05
        // Nếu giá trị p_value nhỏ hơn mức ý nghĩa thống kê, ta bác bỏ giả thuyết không và kết luận mẫu dữ liệu không tuân theo phân bố chuẩn
        // Nếu giá trị p_value lớn hơn hoặc bằng mức ý nghĩa thống kê, ta không bác bỏ giả thuyết không và kết luận mẫu dữ liệu tuân theo phân bố chuẩn
        double alpha = 0.05; // Mức ý nghĩa thống kê
        if (p_value < alpha) {
            noOfPoint = triple * 3 + single;
            continue;
        } else isFine = true;
    }

    return ListInt;
}

// trả về vector chứa các pair <ward, int> như yêu cầu
std::vector<std::pair<Ward, int>> Quest4::OutListWardInt(std::vector<Ward> InListWard, int triple, int single){
    std::vector<std::pair<Ward, int>> Distribute_Ward;

    std::vector<int> Distribute_Int = Gauss_Distribution(InListWard.size(), triple, single);
    // gán lần lượt các int với ward
    for(auto v: Distribute_Ward){
        v.second = Distribute_Int.back();
        Distribute_Int.pop_back();
    }

    return Distribute_Ward;
}
