#include <string>
#include <memory>
#include <stdexcept>
#include "operation.h"


double Operation::performBinaryOperation(std::string type, double l_val, double r_val)
{
    if (type == "Add")
        return l_val + r_val;
    if (type == "Div")
        return l_val / r_val;
    throw std::logic_error("Unsupported operator: " + type);
}

void Operation::perform(std::unordered_map<std::string, double>& terms)
{
    if (inputsCount != 2) {
        throw std::logic_error("Only binary operations are supported");
    }

    double l_val = terms[inputsIdentifers[0]];
    double r_val = terms[inputsIdentifers[1]];

    terms.insert({ outputIdentifer, Operation::performBinaryOperation(type, l_val, r_val) });
}
