#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "nlohmann/json.hpp"   

using namespace std;
using json = nlohmann::json;

 
long long convertToDecimal(const string& val, int base) {
    long long result = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else digit = tolower(c) - 'a' + 10;
        result = result * base + digit;
    }
    return result;
}

 
long long lagrangeInterpolation(const vector<pair<int, long long>>& points) {
    long double secret = 0.0;
    int k = points.size();

    for (int i = 0; i < k; ++i) {
        long double xi = points[i].first;
        long double yi = points[i].second;
        long double term = yi;

        for (int j = 0; j < k; ++j) {
            if (i != j) {
                long double xj = points[j].first;
                term *= -xj / (xi - xj);
            }
        }
        secret += term;
    }

    return static_cast<long long>(round(secret));
}

 
long long processFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "❌ Failed to open file: " << filename << endl;
        exit(1);
    }

    json data;
    file >> data;

    int k = data["keys"]["k"];
    vector<pair<int, long long>> points;

    for (auto& [key, value] : data.items()) {
        if (key == "keys") continue;
        int x = stoi(key);
        int base = stoi(value["base"].get<string>());
        string encodedValue = value["value"];
        long long y = convertToDecimal(encodedValue, base);
        points.emplace_back(x, y);
        if (points.size() == k) break;
    }

    return lagrangeInterpolation(points);
}

int main() {
    cout << "✅ Secret from testcase1.json: " << processFile("testcase1.json") << endl;
    cout << "✅ Secret from testcase2.json: " << processFile("testcase2.json") << endl;
    return 0;
}
