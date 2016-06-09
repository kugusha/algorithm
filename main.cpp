#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

struct MercuryStruckt {
    double weight;
    double weight_mg;
};

bool func(MercuryStruckt& one, MercuryStruckt& two) {
    return one.weight_mg > two.weight_mg;
}

int main() {
    long long int N, W;
    std::cin >> N >> W;
    std::vector<MercuryStruckt> cuprum;
    for (int ii = 0; ii < N; ++ii) {
        double p, w;
        std::cin >> p >> w;
        MercuryStruckt another_merc;
        another_merc.weight = w;
        if (w == 0) {
            another_merc.weight_mg = p;
        } else {
            another_merc.weight_mg = p / w;
        }
        if (p != 0) {
            cuprum.push_back(another_merc);
        }
    }
    double profit = 0;
    std::sort(cuprum.begin(), cuprum.end(), func);
    auto iter = cuprum.begin();
    while (iter != cuprum.end()) {
        if (W >= (*iter).weight) {
            if ((*iter).weight == 0) {
                profit = profit + (*iter).weight_mg;
            } else {
                profit = profit + (*iter).weight_mg * (*iter).weight;
            }
            W = W - (*iter).weight;
        } else if (W < (*iter).weight && W != 0) {
            profit = profit + W * (*iter).weight_mg;
            W = 0;
        }
        ++iter;
    }
    std::cout << std::fixed << std::setprecision(6) << profit;
}