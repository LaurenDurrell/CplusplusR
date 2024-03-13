#random permutations of the data.frame rows 
shuffle_function <- function(df) { 
  shuffled_df<- df[ ,sample(1:ncol(df))] 
  return (shuffled_df)
} 

#input data checks
check_data_inputs <- function(df1, df2) {
  if(sum(dim(df1))==sum(dim(df2))) {print("preference tables the same size")}
  unique_cols_df1 <- sapply(df1, function(col) length(unique(col)))
  if (all(unique_cols_df1 == sapply(df1, length))) {
    print("No duplicate entries within each column of preference table 1")}
  if(length(unique(names(df1)))==length(names(df1))) {print("no duplicates names in preference table names")}
  if(class(df1)=="data.frame"|class(df1)=="list") {print("df1 correct type")}
  if(class(df2)=="data.frame"|class(df2)=="list") {print("df2 correct type")} 
  if(sapply(df1, class)=="string"|sapply(df1, class)=="character") {print("df1 contains strings or characters as required")}
  if(unique(sapply(df2, class)=="string"|sapply(df2, class)=="character")) {print("df2 contains strings or characters as required")}
  else(print("errors in input data"))
}


