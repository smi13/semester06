# Homework for mathematical statistics according to 'calctask.txt' - Part II (hypothesis tests)

# Every digit result will be stored in "results/res.txt" file
# So 'results' folder must exist

# 'calc' function from Part I must be called previously

require(nortest)

kolmogorovTest <- function(sample)
{
  test_result = ks.test(sample, "pnorm", mean = main_sample_mu, sd = main_sample_sigma)
  
  cat(test_result$method)
  writeLines("\n\np-value: ")
  cat(test_result$p.value)  
}

pearsonTest <- function(sample)
{
  test_result = pearson.test(sample)
  writeLines("\n\n")
  cat(test_result$method)
  writeLines("\n\np-value: ")
  cat(test_result$p.value)  
}

kolmogorovSmirnovTest <- function(sample1, sample2)
{
  test_result = ks.test(sample1, sample2)
  writeLines("\n\n")
  cat(test_result$method)
  writeLines("\n\np-value: ")
  cat(test_result$p.value)  
}

wilcoxTest <- function(sample1, sample2)
{
  test_result = wilcox.test(sample1, sample2)
  writeLines("\n\n")
  cat(test_result$method)
  writeLines("\n\np-value: ")
  cat(test_result$p.value)  
}

fisherTest <- function(sample1, sample2)
{
  test_result = var.test(sample1, sample2)
  writeLines("\n\n")
  cat(test_result$method)
  writeLines("\n\np-value: ")
  cat(test_result$p.value)    
}

studentTest <- function(sample1, sample2)
{
  # Assuming that F test succeed
  test_result = t.test(sample1, sample1, var.equal = TRUE)

  writeLines("\n\n")
  cat(test_result$method)
  writeLines("\n\np-value: ")
  cat(test_result$p.value)    
}

chiSquareTest <- function(sample1, sample2)
{
  test_result = chisq.test(sample1, sample2)

  writeLines("\n\n")
  cat(test_result$method)
  writeLines("\n\np-value: ")
  cat(test_result$p.value)    
}

# Main program function.

calcTests <- function()
{
  sink("results/res_tests.txt", append = FALSE)

  # Distrubtion hypothesis tests
  # Assuming main_sample is from N(main_sample_mu, main_sample_sigma)
  kolmogorovTest(main_sample)
  pearsonTest(main_sample)   

  # Other tests are two-sample. Form second sample
 
  # Currently second sample is second part of original sample

  sample_a = main_sample[1:as.integer(length(main_sample) / 2)]
  sample_b = main_sample[as.integer(length(main_sample) / 2):length(main_sample)]

  #sample_a = theor_sample[1:as.integer(length(theor_sample) / 2)]
  #sample_b = theor_sample[as.integer(length(theor_sample) / 2):length(theor_sample)]

  #sample_a = orig_sample[1:as.integer(length(orig_sample) / 2)]
  #sample_b = orig_sample[as.integer(length(orig_sample) / 2):length(orig_sample)]

  # Homogeneity hypothesis tests     
  kolmogorovSmirnovTest(sample_a, sample_b)
  wilcoxTest(sample_a, sample_b)
  fisherTest(sample_a, sample_b)
  studentTest(sample_a, sample_b)

  # Independence hypothesis test
  while(length(sample_b) > length(sample_a))
    sample_b = sample_b[-1]

  chiSquareTest(sample_a, sample_b)

  sink()
}