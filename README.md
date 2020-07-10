# onnx-executor

### Build
```sh
$ git clone https://github.com/Bartosz12364/onnx-executor.git
$ cd onnx-executor
$ sudo docker build -t onnx-executor:1.0 .
```

### Usage
*It is most convenient to launch the docker in interactive mode*
```sh
$ sudo docker run -it onnx-executor:1.0 bash
```
Then select a model from the example_models directory (ie. *model.onnx*):

example use:
```sh
$ ./Release/onnx-executor example_models/model.onnx 3 2 1
5
```



