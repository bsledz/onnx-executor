#include <string>
#include <memory>
#include <stdexcept>
#include "operation.h"
#include "executorExceptions.h"

using Operator = std::function<double(double, double)>;

void Operation::perform(std::unordered_map<std::string, double>& terms)
{
    double l_val = terms[inputsIdentifers[0]];
    double r_val = terms[inputsIdentifers[1]];

    terms.insert({this->outputIdentifer, this->op(l_val, r_val)});
}

Operation::Operation(Operator op, std::shared_ptr<std::string[]> inputsIdentifers, int inputsCount, std::string outputIdentifer)
: op(op), outputIdentifer(outputIdentifer), inputsCount(inputsCount), inputsIdentifers(inputsIdentifers)
{
    if (inputsCount != 2) throw UnsupportedModel("Only binary operators are supported");
}