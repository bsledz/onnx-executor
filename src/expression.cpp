#include <memory>
#include <string>
#include <stdexcept>
#include "operation.h"
#include "expression.h"
#include "onnx.pb.h"
using namespace std;

Expression::Expression(const onnx::GraphProto& graph) : inputsCount(graph.input_size())
{
    if (graph.output_size() != 1) {
        throw invalid_argument("Provided graph is not representing an expression. Output should");
    }
    outputIdentifer = graph.output()[0].name();
    inputsIdentifers = shared_ptr<string[]>(new string[inputsCount]);

    for (int i = 0; i < graph.input_size(); i++) {
        inputsIdentifers[i] = graph.input()[i].name();
    }

    operations.reserve(graph.node_size());
    for (int i = 0; i < graph.node_size(); i++) {
        auto node = graph.node()[i];
        auto inputsIdentifers = shared_ptr<string[]>(new string[node.input_size()]);
        for (int i = 0; i < node.input_size(); i++) {
            inputsIdentifers[i] = node.input()[i];
        }
        operations.push_back(Operation(node.op_type(), inputsIdentifers, node.input_size(), node.output()[0]));
    }
}

double Expression::calculate(std::shared_ptr<double[]> inputs, int inputsCount)
{
    if (inputsCount != this->inputsCount) {
        throw invalid_argument("Inputs mismatch");
    }

    for (int i = 0; i < inputsCount; i++) {
        string termName = inputsIdentifers[i];
        terms.insert({ termName, inputs[i] });
    }

    for (int i = 0; i < operations.size(); i++) {
        operations[i].perform(terms);
    }

    return terms[outputIdentifer];
}

std::shared_ptr<Expression> Expression::fromOnnx(std::istream& in)
{
onnx::ModelProto onnxModel;
onnxModel.ParseFromIstream(&in);

return std::shared_ptr<Expression>(new Expression(onnxModel.graph()));
}
