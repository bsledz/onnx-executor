#include <stdexcept>
#include <string>

class UnsupportedModel : public std::runtime_error
{ 
public:
    UnsupportedModel(char const* const message) throw() : std::runtime_error(message) {}
    virtual char const* what() const throw() {
        return runtime_error::what(); 
    }
};

class InvalidInput : public std::runtime_error
{ 
public:
    InvalidInput(char const* const message) throw() : std::runtime_error(message) {}
    virtual char const* what() const throw(){
        return runtime_error::what(); 
    }
};