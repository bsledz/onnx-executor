#include <iostream>
#include <string>
#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <unordered_map>
#include "../src/operation.h"

TEST(test_operation, test_eval_add)
{
    std::shared_ptr<std::string[]> inputIdentifers(new std::string[2]);
    inputIdentifers[0] = "A";
    inputIdentifers[1] = "B";
    Operation operation("Add", inputIdentifers, 2, "Y");
    std::unordered_map<std::string, double> terms;
    terms.insert({"A", 7});
    terms.insert({"B", 10});
    operation.perform(terms);
    EXPECT_DOUBLE_EQ(17, terms["Y"]); 
}

TEST(test_operation, test_eval_div)
{
    std::shared_ptr<std::string[]> inputIdentifers(new std::string[2]);
    inputIdentifers[0] = "J";
    inputIdentifers[1] = "G";
    Operation operation("Div", inputIdentifers, 2, "W");
    std::unordered_map<std::string, double> terms;
    terms.insert({"J", 10});
    terms.insert({"G", 5});
    operation.perform(terms);
    EXPECT_DOUBLE_EQ(2, terms["W"]); 
}

TEST(test_operation, test_eval_too_much_inputs)
{
    std::shared_ptr<std::string[]> inputIdentifers(new std::string[3]);
    inputIdentifers[0] = "A";
    inputIdentifers[1] = "B";
    inputIdentifers[2] = "C";
    Operation operation("Div", inputIdentifers, 3, "Y");
    std::unordered_map<std::string, double> terms;
    try{
	operation.perform(terms);
	FAIL() << "Expected std::logic_error";
    }catch(std::logic_error &err){
        EXPECT_EQ(err.what(),std::string("Only binary operations are supported"));
    }
}

TEST(test_operator, test_add)
{
    EXPECT_DOUBLE_EQ(10, Operation::performBinaryOperation("Add", 5, 5));
    EXPECT_DOUBLE_EQ(-4508227, Operation::performBinaryOperation("Add", -4512452, 4225));
    EXPECT_DOUBLE_EQ(0, Operation::performBinaryOperation("Add", 0, 0));
    EXPECT_DOUBLE_EQ(0.040205400000000002, Operation::performBinaryOperation("Add", 0.0402044, 0.000001));
}

TEST(test_operator, test_div)
{
    EXPECT_DOUBLE_EQ(1, Operation::performBinaryOperation("Div", 5, 5));
    EXPECT_DOUBLE_EQ(-1068.0359763313609, Operation::performBinaryOperation("Div", -4512452, 4225));
    EXPECT_DOUBLE_EQ(0, Operation::performBinaryOperation("Div", 0, 7));
    EXPECT_DOUBLE_EQ(40204.400000000001, Operation::performBinaryOperation("Div", 0.0402044, 0.000001));
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
