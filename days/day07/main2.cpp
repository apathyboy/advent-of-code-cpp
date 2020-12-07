#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>

struct Bag {
    std::string       name;
    std::vector<Bag*> contain;
    std::vector<int>  amount;

    Bag(std::string n) { name = n; }
};

Bag* GetBag(std::string name, std::map<std::string, Bag*>& map)
{
    if (map.find(name) == map.end()) {
        map[name] = new Bag(name);
    }

    return map[name];
}

bool CanContainBag(std::string name, Bag* headBag)
{
    if (headBag->name == name)
        return true;

    for (unsigned int i = 0; i < headBag->contain.size(); i++) {
        if (CanContainBag(name, headBag->contain[i]))
            return true;
    }

    return false;
}

int CountInternalBags(Bag* headBag)
{
    int n = 0;

    for (unsigned int i = 0; i < headBag->contain.size(); i++) {
        n += headBag->amount[i]
             + (headBag->amount[i] * CountInternalBags(headBag->contain[i]));
    }

    return n;
}

int main()
{
    std::map<std::string, Bag*> bagTable;

    std::ifstream ifs{"days/day07/input.txt"};
    std::string   text;
    while (getline(ifs, text)) {
        std::istringstream       iss(text);
        std::vector<std::string> results(
            (std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());

        // Get target bag
        Bag* b = GetBag(results[0] + " " + results[1], bagTable);

        // Get contained bags
        for (unsigned int i = 4; i < results.size(); i += 4) {
            int n = atoi(results[i + 0].c_str());

            if (n <= 0)
                continue;

            // std::cout << "Adding " << n << " " << results[i + 1] + " " + results[i + 2] << "
            // bags to " << results[0] + " " + results[1] << std::endl;

            Bag* c = GetBag(results[i + 1] + " " + results[i + 2], bagTable);

            // for (int j = 0; j < n; j++)
            {
                b->contain.push_back(c);
                b->amount.push_back(n);
            }
        }
    }

    int ShinyGoldBags = 0;
    for (auto it : bagTable) {
        if (it.first != "shiny gold" && CanContainBag("shiny gold", it.second))
            ShinyGoldBags++;
    }

    std::cout << "Shiny Gold bags can be contained in " << ShinyGoldBags << " bags."
              << std::endl;

    std::cout << "A single shiny gold bag must contain "
              << CountInternalBags(GetBag("shiny gold", bagTable)) << " bags" << std::endl;

    return 0;
}
