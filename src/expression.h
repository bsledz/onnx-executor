#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

#include "onnx.pb.h"

using Operator = std::function<double(double, double)>;

class Operation;

class Expression {
    std::shared_ptr<std::string[]> inputsIdentifers;
    int inputsCount;

    std::vector<Operation> operations;

    std::string outputIdentifer;

    std::unordered_map<std::string, Operator> binaryOperators;

    Expression(const onnx::GraphProto& graph, std::unordered_map<std::string, Operator> binaryOperators);

public:

    double calculate(std::shared_ptr<double[]>, int inputsCount);

    static std::unique_ptr<Expression> fromIstream(std::istream &in, onnx::ModelProto onnxModel, std::unordered_map<std::string, Operator> binaryOperators);
    static std::unique_ptr<Expression> fromOnnxGraph(onnx::GraphProto onnxGraph, std::unordered_map<std::string, Operator> binaryOperators);
};
