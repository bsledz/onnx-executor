#include <memory>
#include <string>
#include <stdexcept>
#include "operation.h"
#include "expression.h"
#include "onnx.pb.h"
#include "executorExceptions.h"

Expression::Expression(const onnx::GraphProto& graph, std::unordered_map<std::string, Operator> binaryOperators) 
: inputsCount(graph.input_size()), binaryOperators(binaryOperators)
{
    if (graph.output_size() != 1) {
        throw UnsupportedModel("Only models with single output are supported");
    }
    outputIdentifer = graph.output()[0].name();
    inputsIdentifers = std::shared_ptr<std::string[]>(new std::string[inputsCount]);

    for (int inputIndex = 0; inputIndex < graph.input_size(); inputIndex++) {
        inputsIdentifers[inputIndex] = graph.input()[inputIndex].name();
    }

    operations.reserve(graph.node_size());
    for (int nodeIndex = 0; nodeIndex < graph.node_size(); nodeIndex++) {
        auto node = graph.node()[nodeIndex];
        if (binaryOperators.find(node.op_type()) == binaryOperators.end()) throw UnsupportedModel(("Operator "+node.op_type()+" is not supported").c_str());
        auto nodeInputsIdentifers = std::shared_ptr<std::string[]>(new std::string[node.input_size()]);
        for (int nodeInputIndex = 0; nodeInputIndex < node.input_size(); nodeInputIndex++) {
            nodeInputsIdentifers[nodeInputIndex] = node.input()[nodeInputIndex];
        }
        
        auto operatorForOperation = binaryOperators[node.op_type()];
        operations.push_back(Operation(operatorForOperation, nodeInputsIdentifers, node.input_size(), node.output()[0]));
    }
}

double Expression::calculate(std::shared_ptr<double[]> inputs, int inputsCount)
{
    std::unordered_map<std::string, double> terms;
    if (inputsCount != this->inputsCount) {
        std::string msg = "The model requires " + std::to_string(this->inputsCount) + " inputs - given: " + std::to_string(inputsCount);
        throw InvalidInput(msg.c_str());
    }

    for (int inputIndex = 0; inputIndex < inputsCount; inputIndex++) {
        std::string termName = inputsIdentifers[inputIndex];
        terms.insert({ termName, inputs[inputIndex] });
    }

    for (int operationindex = 0; operationindex < operations.size(); operationindex++) {
        operations[operationindex].perform(terms);
    }
    if (terms.find(outputIdentifer) == terms.end()) throw UnsupportedModel("The output value of the model has not been calculated - invalid model");
    return terms[outputIdentifer];
}

std::unique_ptr<Expression> Expression::fromIstream(std::istream &in, onnx::ModelProto onnxModel, std::unordered_map<std::string, Operator> binaryOperators)
{
    if(!onnxModel.ParseFromIstream(&in)){
        throw InvalidInput("Unable to load onnx model from specified file");
    }

    return std::unique_ptr<Expression>(new Expression(onnxModel.graph(), binaryOperators));
}

std::unique_ptr<Expression> Expression::fromOnnxGraph(onnx::GraphProto onnxGraph, std::unordered_map<std::string, Operator> binaryOperators)
{
    return std::unique_ptr<Expression>(new Expression(onnxGraph, binaryOperators));
}
