# A C++ stable marriage function integrated into an R package 
## Description 
This R package uses the Rcpp library, which exposes a C++ implementation of the Fundamental Algorithm described on page 9 of Stable Marriage and Its Relation to Other Combinatorial Problems. 
The package provides a stable_marriage function, which accepts inputs that represent the preference tables. This function returns a stable matching as defined in Stable Marriage and Its Relation to Other Combinatorial Problems.
<br><be>
## functions:
### C++ functions
fundermental_wrapper()<br> 
This function takes two preference tables as input parameters and produces a dataframe of a stable matching (list of engagements).<br> 
is_stable_check_wrapper()<br>
this function takes two preference tables as input parameters and outputs either "stable" or "unstable". This function is used to validate the stability of the fundermental algorithms output. 
### R functions
check_data_inputs <br> 
This function checks the preference tables are correctly structured. It checks the dimensions of the tables match, that there are unique column names, that there are unique entries within each column (no duplicates in people's preference lists), and that the table is of a class suitable for the C++ algorithms. <br> 
shuffle_function() <br> 
This function takes a dataframe and reorders the columns randomly. This can be useful as preference tables tend to favor the first person. Shuffling the table can produce different stable matches. 
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
The fundamental algorithm function and the is_stable algorithm require either data.frames or named lists as inputs that represent the preference tables. 
### DataFrame
The column names and the data should all be strings. 
The column names represent the names of the people who'd preferences are listed the the column below. Example:<br>
A <- c('a','b','d')<br>
B <- c('b','a','d')<br>
C <- c('d','a','b')<br>
df1 <- data.frame(A,B,C)<br>
![image](https://github.com/LaurenDurrell/CplusplusR/assets/158074633/4c019a00-627c-4fa5-9f13-af3a629d9323)<br>
<br><br>
### named lists
Should be similarly structured. Example: 
NamedList1 <- list(A = c('a','b','d'),<br>
                   B = c('b','a','d'),<br>
                   C = c('d','a','b'))<br>
## Example R code to run the stable marriage function and expected output 
More detailed examples can be found in the repository here: (https://github.com/LaurenDurrell/Interfacing-C-and-R-additional-resources-) <br>
Short R code example: <br>
install_github("LaurenDurrell/CplusplusR",force=TRUE)<br>
library(CplusplusR)<br>
A <- c('a','b','d')<br>
B <- c('b','a','d')<br>
C <- c('d','a','b')<br>
a <- c('C','A','B')<br>
b <- c('A','C','B')<br>
d <- c('B','C','A')<br>
df1 <- data.frame(A,B,C)<br>
df2 <- data.frame(a,b,d)<br>
output <- fundamental_wrapper(df1,df2)<br>
is_stable_check_wrapper(df1,df2) #A C++ function has also been provided to check the stability of the output DataFrame:<br>
<br>
Output: <br>
The code will return a DataFrame where the 'names' column gives the names of the people from preference table 1, and the 'values' column gives the names of the people they should be engaged in order to give a stable matching. <br>
![image](https://github.com/LaurenDurrell/CplusplusR/assets/158074633/75846d4d-74c0-4e71-9358-082156d30855)<br>
A C++ function has also been provided to check the stability of the output DataFrame:<br>
"STABLE"
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

