#include <fstream>
#include <string>
#include <memory>
#include <stdlib.h>

#include "onnx.pb.h"

#include "expression.h"
#include "operation.h"

void parseAndValidateArguments(int argc, char** argv, std::shared_ptr<double[]> &inputs, int& inputsCount){
    if(argc < 2){
    	throw std::invalid_argument("Missing model path. USAGE: onnx_executor <model_path> inputs..");
    }

    std::ifstream modelFile(argv[1]);
    if(!modelFile.good()) throw std::invalid_argument("Model file does not exist");
    inputsCount = argc - 2;
    inputs = std::make_shared<double[]>(inputsCount);
    for(int indexInput = 0; indexInput < inputsCount; indexInput++){
    	char* isDoubleTest;
    	strtof(argv[2+indexInput], &isDoubleTest);
    	if((*isDoubleTest) != '\0') throw std::invalid_argument("One of inputs is not a Float");
    	inputs[indexInput] = atof(argv[2+indexInput]);
    }
}

int main(int argc, char** argv){
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    int inputsCount = 0;
    char** modelPath;
    std::shared_ptr<double[]> inputs;
    parseAndValidateArguments(argc, argv, inputs, inputsCount);
    std::ifstream in(argv[1], std::ios_base::binary);
    std::shared_ptr<Expression> expression(Expression::fromOnnx(in));
    in.close();
    std::cout << expression->calculate(inputs, inputsCount) << std::endl;
    return 0;
}
