#include <string>
#include <memory>
#include <unordered_map>

class Operation {
    std::shared_ptr<std::string[]> inputsIdentifers;
    int inputsCount;

    std::string outputIdentifer;
    std::string type;

public:
    static double performBinaryOperation(std::string type, double l_val, double r_val);

    void perform(std::unordered_map<std::string, double>& terms);

    Operation(std::string type, std::shared_ptr<std::string[]> inputsIdentifers, int inputsCount, std::string outputIdentifer)
        : type(type)
        , outputIdentifer(outputIdentifer)
        , inputsCount(inputsCount)
        , inputsIdentifers(inputsIdentifers)
    {
    }
};
