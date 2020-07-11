#include <iostream>
#include <string>
#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <unordered_map>
#include "../src/operation.h"
#include "../src/executorExceptions.h"

TEST(test_operation, test_eval)
{
    std::shared_ptr<std::string[]> inputIdentifers(new std::string[2]);
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
    std::shared_ptr<std::string[]> inputIdentifers(new std::string[3]);
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
