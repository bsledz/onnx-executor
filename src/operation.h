#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

using Operator = std::function<double(double, double)>;

class Operation {
    std::shared_ptr<std::string[]> inputsIdentifers;
    int inputsCount;

    std::string outputIdentifer;
    Operator op;

public:
    void perform(std::unordered_map<std::string, double>& terms);

    Operation(Operator op, std::shared_ptr<std::string[]> inputsIdentifers, int inputsCount, std::string outputIdentifer);
};
