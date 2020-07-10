#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "onnx.pb.h"

class Operation;

class Expression {
    std::shared_ptr<std::string[]> inputsIdentifers;
    int inputsCount;

    std::vector<Operation> operations;

    std::string outputIdentifer;
    std::unordered_map<std::string, double> terms;

    Expression(const onnx::GraphProto& graph);

public:
    double calculate(std::shared_ptr<double[]>, int inputsCount);

    static std::shared_ptr<Expression> fromOnnx(std::istream& in);
};
