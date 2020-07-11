#include <memory>
#include <string>
#include <stdexcept>
#include "operation.h"
#include "expression.h"
#include "onnx.pb.h"
#include "executorExceptions.h"

Expression::Expression(const onnx::GraphProto& graph, std::unordered_map<std::string, Operator> binaryOperators) : inputsCount(graph.input_size()), binaryOperators(binaryOperators)
{
    if (graph.output_size() != 1) {
        throw UnsupportedModel("Only models with single output are supported");
    }
    outputIdentifer = graph.output()[0].name();
    inputsIdentifers = std::shared_ptr<std::string[]>(new std::string[inputsCount]);

    for (int i = 0; i < graph.input_size(); i++) {
        inputsIdentifers[i] = graph.input()[i].name();
    }

    operations.reserve(graph.node_size());
    for (int i = 0; i < graph.node_size(); i++) {
        auto node = graph.node()[i];
        auto inputsIdentifers = std::shared_ptr<std::string[]>(new std::string[node.input_size()]);
        for (int i = 0; i < node.input_size(); i++) {
            inputsIdentifers[i] = node.input()[i];
        }
        if (binaryOperators.find(node.op_type()) == binaryOperators.end()) throw UnsupportedModel(("Operator "+node.op_type()+" is not supported").c_str());
        auto operatorForOperation = binaryOperators[node.op_type()];
        operations.push_back(Operation(operatorForOperation, inputsIdentifers, node.input_size(), node.output()[0]));
    }
}

double Expression::calculate(std::shared_ptr<double[]> inputs, int inputsCount)
{
    if (inputsCount != this->inputsCount) {
        std::string msg = "The model requires " + std::to_string(this->inputsCount) + " inputs - given: " + std::to_string(inputsCount);
        throw InvalidInput(msg.c_str());
    }

    for (int i = 0; i < inputsCount; i++) {
        std::string termName = inputsIdentifers[i];
        terms.insert({ termName, inputs[i] });
    }

    for (int i = 0; i < operations.size(); i++) {
        operations[i].perform(terms);
    }

    return terms[outputIdentifer];
}

std::unique_ptr<Expression> Expression::fromIstream(std::istream& in, std::unordered_map<std::string, Operator> binaryOperators)
{
    onnx::ModelProto onnxModel;
    if(!onnxModel.ParseFromIstream(&in)){
         throw InvalidInput("Unable to load onnx model from specified file");
    }

    return std::unique_ptr<Expression>(new Expression(onnxModel.graph(), binaryOperators));
}
