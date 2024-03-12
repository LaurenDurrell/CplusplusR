# A C++ stable marriage function integrated into an R package 
## Description 
This R package uses the Rcpp library, which exposes a C++ implementation of the Fundamental Algorithm described on page 9 of Stable Marriage and Its Relation to Other Combinatorial Problems. 
The package provides a stable_marriage function, which accepts inputs that represent the preference tables. This function returns a stable matching as defined in Stable Marriage and Its Relation to Other Combinatorial Problems.
<br><br>
## Required packages and dependencies 
The C++ was built using a c++17 compiler 
### R packages: 
devtools<br>
library(Rcpp)
### C++ packages (these are called within the package and do not need to be installed separately):
"Rcpp.h"<br>
string<br>
iostream<br>
vector<br>
unordered_map<br>
algorithm<br>
fstream<br>
sstream<br>
string<br>
map<br>
list<br>
cctype<br>
chrono<br>
random
## Function parameters: input data structures 
### DataFrame
This function required two R DataFrames as input that represent the preference tables.
The column names and the data should all be strings. 
The column names represent the names of the people who'd preferences are listed the the column below. Example:<br>
A <- c('a','b','d')<br>
B <- c('b','a','d')<br>
C <- c('d','a','b')<br>
df1 <- data.frame(A,B,C)<br>
![image](https://github.com/LaurenDurrell/CplusplusR/assets/158074633/4c019a00-627c-4fa5-9f13-af3a629d9323)<br>
<br><br>
## Example R code to run the stable marriage function and expected output 
R code: <br>
install_github("LaurenDurrell/CplusplusR",force=TRUE)<br>
library(CplusplusR)<br>
output <- fundamental_wrapper(df1,df2)
<br><br>
Output: <br>
The code will return a DataFrame where the 'names' column gives the names of the people from preference table 1, and the 'values' column gives the names of the people they should be engaged in order to give a stable matching. <br>
![image](https://github.com/LaurenDurrell/CplusplusR/assets/158074633/75846d4d-74c0-4e71-9358-082156d30855)<br>
<br><br> 
A C++ function has also been provided to check the stability of the output DataFrame:<br>
is_stable_check_wrapper(df1,df2)<br>
The output of this is either "STABLE" or "UNSTABLE".
## Algorithm pseudo-code 
The algorithm is detailed on page 9 of Stable Marriage and Its Relation to Other Combinatorial Problems: <br>
![image](https://github.com/LaurenDurrell/CplusplusR/assets/158074633/a3dd52a6-4952-4cf0-accc-eea3853db185)<br>
[Knuth, Donald Ervin. Stable marriage and its relation to other combinatorial problems: An introduction to the mathematical analysis of algorithms. Vol. 10. American Mathematical Soc., 1997.]<br>
The following pages of the book break the algorithm down into more detail. 
<br><br>
## Installation instructions 
R-4.3.1 was used to install and run this package. <br>
Install and load the "devtools" package. <br> 
To install the package directly from GitHub, use the following commands: <br> 
install_github("LaurenDurrell/CplusplusR",force=TRUE)<br>
library(CplusplusR)<br> 

