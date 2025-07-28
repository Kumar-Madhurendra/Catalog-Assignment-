#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric> // for gcd
#include "nlohmann/json.hpp"
#include <boost/multiprecision/cpp_int.hpp>



using namespace std;
using json = nlohmann::json;

// Converts base-N encoded string into a decimal integer
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

// Helper function to compute GCD
long long gcd(long long a, long long b) {
    return b ? gcd(b, a % b) : abs(a);
}

// Computes the Lagrange interpolation at x=0 using exact fraction arithmetic
long long lagrangeInterpolation(const vector<pair<int, long long>>& points) {
    long long numerator = 0;
    long long denominator = 1;

    for (int i = 0; i < points.size(); ++i) {
        long long xi = points[i].first;
        long long yi = points[i].second;

        long long num = yi;
        long long den = 1;

        for (int j = 0; j < points.size(); ++j) {
            if (i == j) continue;
            long long xj = points[j].first;
            num *= -xj;
            den *= (xi - xj);
        }

        // Add this term to total (cross-multiplication to handle fractions)
        numerator = numerator * den + num * denominator;
        denominator *= den;

        // Simplify fraction (optional but helps avoid overflow)
        long long g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
    }

    return numerator / denominator; // secret at x = 0
}

// Reads JSON and reconstructs the secret
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
