#include <fstream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <functional>

#include "onnx.pb.h"
#include "executorExceptions.h"
#include "expression.h"
#include "operation.h"

using Operator = std::function<double(double, double)>;

void parseAndValidateArguments(int argc, char** argv, std::shared_ptr<double[]> &inputs, int& inputsCount)
{
    if(argc < 2){
    	throw InvalidInput("Missing model path.");
    }

    std::ifstream modelFile(argv[1]);
    if(!modelFile.good()) {
        std::string modelPath(argv[1]);
        throw InvalidInput("The model file does not exist under the specified path: ");
    }
    inputsCount = argc - 2;
    inputs = std::make_shared<double[]>(inputsCount);
    for(int indexInput = 0; indexInput < inputsCount; indexInput++){
    	char* isDoubleTest;
    	strtof(argv[2+indexInput], &isDoubleTest);
    	if((*isDoubleTest) != '\0') throw InvalidInput("One of inputs is not a floating point number type");
    	inputs[indexInput] = atof(argv[2+indexInput]);
    }
}

std::unordered_map<std::string, Operator> createSupportedOperators()
{
    std::unordered_map<std::string, Operator> operators;
    operators.insert({"Add", [](double l, double r) -> double {return l+r;}});
    operators.insert({"Div", [](double l, double r) -> double {return l/r;}});
    return operators;
}

int main(int argc, char** argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    int inputsCount = 0;
    char** modelPath;
    std::shared_ptr<double[]> inputs;
    try{
        parseAndValidateArguments(argc, argv, inputs, inputsCount);
        std::ifstream in(argv[1], std::ios_base::binary);
        std::unique_ptr<Expression> expression(Expression::fromIstream(in, onnx::ModelProto(), createSupportedOperators()));
        in.close();
        std::cout << expression->calculate(inputs, inputsCount) << std::endl;
    }catch(UnsupportedModel &exc){
        std::cout << "Unsupported model: " << exc.what() << std::endl;
    }catch(InvalidInput &exc){
        std::cout << "Invalid input: " << exc.what() << std::endl;
        std::cout << "USAGE: onnx_executor <model_path> inputs.." << std::endl;
    }
    return 0;
}
