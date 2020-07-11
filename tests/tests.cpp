#include <iostream>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include "gtest/gtest.h"

#include "../src/expression.h"
#include "../src/operation.h"
#include "../src/executorExceptions.h"

using Operator = std::function<double(double, double)>;

TEST(test_operation, test_eval)
{
    std::shared_ptr<std::string[]> inputIdentifers = std::make_shared<std::string[]>(2);
    
    inputIdentifers[0] = "A";
    inputIdentifers[1] = "B";
    Operation operation([](double l, double r) -> double {return l+r;}, inputIdentifers, 2, "Y");
    std::unordered_map<std::string, double> terms;
    terms.insert({"A", 7});
    terms.insert({"B", 10});
    operation.perform(terms);
    EXPECT_DOUBLE_EQ(17, terms["Y"]); 
}

TEST(test_operation, test_eval_too_much_inputs)
{
    std::shared_ptr<std::string[]> inputIdentifers = std::make_shared<std::string[]>(3);
    inputIdentifers[0] = "A";
    inputIdentifers[1] = "B";
    inputIdentifers[2] = "C";
    try{
        Operation operation([](double l, double r) -> double {return l+r;}, inputIdentifers, 3, "Y");
	    FAIL() << "Expected UnsupportedModel";
    }catch(UnsupportedModel &err){
        EXPECT_EQ(err.what(),std::string("Only binary operators are supported"));
    }
}

TEST(test_expression, test_from_istream)
{
    std::istringstream stream("");
    try{
        std::unique_ptr<Expression> expression(Expression::fromIstream(stream, onnx::ModelProto(), std::unordered_map<std::string, Operator>()));
	    FAIL() << "Expected UnsupportedModel";
    }catch(UnsupportedModel &err){
        EXPECT_EQ(err.what(),std::string("Only models with single output are supported"));
    }
}

TEST(test_expression, test_constructor_unsupported_operator)
{
    onnx::GraphProto graph;
    onnx::NodeProto* node = graph.add_node();
    node->add_input("A");
    node->add_input("B");
    node->add_output("C");
    node->set_op_type("Mul");
    graph.add_output();

    try{
        std::unique_ptr<Expression> expression(Expression::fromOnnxGraph(graph, std::unordered_map<std::string, Operator>()));
	    FAIL() << "Expected UnsupportedModel";
    }catch(UnsupportedModel &err){
        EXPECT_EQ(err.what(),std::string("Operator Mul is not supported"));
    }
}

TEST(test_expression, test_complex_flow)
{
    onnx::GraphProto graph;
    onnx::NodeProto* addNode = graph.add_node();
    onnx::NodeProto* divNode = graph.add_node();
    addNode->add_input("A");
    addNode->add_input("B");
    addNode->add_output("Z");

    divNode->add_input("Z");
    divNode->add_input("C");
    divNode->add_output("Y");

    addNode->set_op_type("Add");
    divNode->set_op_type("Div");
    graph.add_output();
    graph.add_input();
    graph.add_input();
    graph.add_input();
    graph.mutable_output()[0][0].set_name("Y");
    graph.mutable_input()[0][0].set_name("A");
    graph.mutable_input()[0][1].set_name("B");
    graph.mutable_input()[0][2].set_name("C");
    
    std::unordered_map<std::string, Operator> operators;
    operators.insert({"Add", [](double l, double r) -> double {return l+r;}});
    operators.insert({"Div", [](double l, double r) -> double {return l/r;}});
    std::unique_ptr<Expression> expression(Expression::fromOnnxGraph(graph, operators));
    std::shared_ptr<double[]> inputs = std::make_shared<double[]>(3);
    inputs[0] = 3.14159;
    inputs[1] = 2.718;
    inputs[2] = -6.02;
    double val = expression->calculate(inputs, 3);
    EXPECT_DOUBLE_EQ(-0.97335382059800668, val); 

}

TEST(test_expression, test_bad_output_name)
{
    onnx::GraphProto graph;
    onnx::NodeProto* node = graph.add_node();
    node->add_input("A");
    node->add_input("B");
    node->add_output("C");
    node->set_op_type("Add");
    graph.add_output();
    graph.add_input();
    graph.add_input();
    graph.mutable_output()[0][0].set_name("BAD");
    graph.mutable_input()[0][0].set_name("A");
    graph.mutable_input()[0][1].set_name("B");
    
    std::unordered_map<std::string, Operator> operators;
    operators.insert({"Add", [](double l, double r) -> double {return l+r;}});
    std::unique_ptr<Expression> expression(Expression::fromOnnxGraph(graph, operators));
    std::shared_ptr<double[]> inputs = std::make_shared<double[]>(2);
    inputs[0] = 5;
    inputs[1] = 3.5;
    try{
        expression->calculate(inputs, 2);
	    FAIL() << "Expected UnsupportedModel";
    }catch(UnsupportedModel &err){
        EXPECT_EQ(err.what(),std::string("The output value of the model has not been calculated - invalid model"));
    }
}

TEST(test_expression, test_simple_flow)
{
    onnx::GraphProto graph;
    onnx::NodeProto* node = graph.add_node();
    node->add_input("A");
    node->add_input("B");
    node->add_output("C");
    node->set_op_type("Add");
    graph.add_output();
    graph.add_input();
    graph.add_input();
    graph.mutable_output()[0][0].set_name("C");
    graph.mutable_input()[0][0].set_name("A");
    graph.mutable_input()[0][1].set_name("B");
    
    std::unordered_map<std::string, Operator> operators;
    operators.insert({"Add", [](double l, double r) -> double {return l+r;}});
    std::unique_ptr<Expression> expression(Expression::fromOnnxGraph(graph, operators));
    std::shared_ptr<double[]> inputs = std::make_shared<double[]>(2);
    inputs[0] = 5;
    inputs[1] = 3.5;
    double val = expression->calculate(inputs, 2);
    EXPECT_DOUBLE_EQ(8.5, val); 

    inputs[0] = 0.2424;
    inputs[1] = -1.323244;

    val = expression->calculate(inputs, 2);
    EXPECT_DOUBLE_EQ(-1.0808440000000001, val); 
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
