#include "onnx.pb.h"
#include <fstream>

void start() {
	onnx::ModelProto* model = new onnx::ModelProto;
	std::ifstream in("model.onnx", std::ios_base::binary);

	model->ParseFromIstream(&in);
	in.close();
	std::cout << model->graph().output_size() << "\n";
	delete model;
}

int main() {
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	start();
	return 0;
}