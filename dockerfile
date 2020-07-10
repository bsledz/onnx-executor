FROM yaspl123/protobuf-ubuntu:1.0

WORKDIR ~ 

COPY src src
COPY proto proto
COPY tests tests

COPY CMakeLists.txt CMakeLists.txt

RUN apt-get update
RUN apt-get install -y cmake
RUN apt-get install -y libgtest-dev
RUN apt-get install -y libgmock-dev

RUN cmake .
RUN make tests

RUN ./Release/tests

RUN make

CMD ./Release/onnx-executor ../
