install.packages("Rcpp")

library(Rcpp) # load the Rcpp package

# write your code into a string
cpp_code <- '
#include "Rcpp.h"
using namespace Rcpp;

#include <string>

std::string example_1(const std::string& arg) 
{
   std::string message {"hello"};
   return message + " " + arg;
}

RCPP_MODULE(stor601) 
{
function("rcpp_example_1", &example_1);
}
'

marshalling_code <- '
#include "Rcpp.h"
using namespace Rcpp;

#include <string>
#include <iostream>
#include <complex>


double marshall_double(const double& X)
{
    Rcout << X << std::endl;
    double Y {3.14};
    return Y;
}

int marshall_integer(const int& X)
{
    Rcout << X << std::endl;
    int Y {3};
    return Y;
}

bool marshall_logical(const bool& X)
{
    Rcout << X << std::endl;
    bool Y = false;
    return Y;
}

char marshall_character(const char& X)
{
    Rcout << X << std::endl;
    char Y = \'A\';
    return Y;
}

std::complex<double> marshall_complex(const std::complex<double>& X)
{
    Rcout << X <<  std::endl;
    std::complex<double> Y {3.14,7.2};
    return Y;
}

RCPP_MODULE(marshalling) 
{
function("rcpp_marshall_double", &marshall_double);
function("rcpp_marshall_integer", &marshall_integer);
function("rcpp_marshall_logical", &marshall_logical);
function("rcpp_marshall_character", &marshall_character);
function("rcpp_marshall_complex", &marshall_complex);
}
'
sourceCpp(code = marshalling_code)
# add your code to R
sourceCpp(code = cpp_code)

# test your code
rcpp_example_1("world")

X <- 3.14
Y <- rcpp_marshall_double(X)
Y <- rcpp_marshall_integer(X)
Y <- rcpp_marshall_logical(X)
Y <- rcpp_marshall_character(X) - fails
Y <- rcpp_marshall_complex(X)

X <- 7L
Y <- rcpp_marshall_double(X)
Y <- rcpp_marshall_integer(X)
Y <- rcpp_marshall_logical(X)
Y <- rcpp_marshall_character(X) - fails
Y <- rcpp_marshall_complex(X)




marshalling_code <- '
#include "Rcpp.h"
using namespace Rcpp;

#include<list> 

std::list<double> marshall_lists(const std::list<std::vector<double> > X)
{
    
    std::list<double> Y;
    for(const auto& x : X)
    {
       Y.push_back(3.14*x[0]);
    }
    return Y;
}


RCPP_MODULE(marshalling) 
{
function("rcpp_marshall_lists", &marshall_lists);
}
'

sourceCpp(code = marshalling_code)

X <- list()
X[[1]] <- 1.2
X[[2]] <- 2.2
X[[3]] <- 3.2
Y <- rcpp_marshall_lists(X)
print(Y)

X[[1]] <- c(1,2,3,4)
Y <- rcpp_marshall_lists(X)
print(Y)



library(Rcpp)
marshalling_code <- '
#include "Rcpp.h"
using namespace Rcpp;

#include <list> 
#include <string>
#include <unordered_map>

std::list<double> marshall_lists
  (const std::list<std::vector<double> > X){
    
    std::list<double> Y;
    for(const auto& x : X)
    {
       Y.push_back(3.14*x[0]);
    }
    return Y;
}

std::map<std::string,std::string> marshall_dictionaries(const std::string& X)
{
std::map<std::string,std::string> Y;
return Y;
}

RCPP_MODULE(marshalling) 
{
function("rcpp_marshall_lists", &marshall_lists);
function("rcpp_marshall_dictionaries", &marshall_dictionaries);
}
'

sourceCpp(code = marshalling_code)

X <- list()
X[[1]] <- 1.2
X[[2]] <- 2.2
X[[3]] <- 3.2
Y <- rcpp_marshall_lists(X)
print(Y)

X[[1]] <- c(1,2,3,4)
Y <- rcpp_marshall_lists(X)
print(Y)
