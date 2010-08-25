# Homework for mathematical statistics according to 'calctask.txt' - Part I (until tests)

# Every digit result will be stored in "results/res.txt" file
# So 'results' folder must exist

require(moments)
require(graphics)

# Read data function.
# Parameters: (see Calc() comment)
readData <- function(fileName, clipCount)
{
  #Read table from file - will contain two columns
  my_sample <<- read.table(file = fileName, header = FALSE)

  #Reorganize table - now 'my_sample' is simply vector
  my_sample <<- my_sample$V1

  #Calculate logarithmic increments
  tmp_sample <<- c(0,my_sample)
  main_sample <<- tmp_sample / my_sample
  main_sample <<- main_sample[-1]
  main_sample <<- log(main_sample)

  #DEBUG output - store sample plot
  png("results/sample.png")
  plot(main_sample)
  dev.off()
  #

  #Clip series from both ends
  for(i in 1:clipCount) main_sample <<- main_sample[-which(main_sample == max(main_sample))]
  for(i in 1:clipCount) main_sample <<- main_sample[-which(main_sample == min(main_sample))]
}

# Build histogram & store picture in file function.
buildHistogram <- function(sample, fileName)
{  
  png(fileName)
  # Use Freedman-Diaconis rule (2 * IQR(x) * n^(-1/3)) to select number of histogram bins
  my_hist <<- hist(sample, breaks = "FD", plot = TRUE, freq = FALSE)  
  dev.off()
}

# Calculate distributions center factors: mode of sample, median, mean function.
calcDistrCenterFactors <- function(sample)
{  
  writeLines("Mean:\n")
  cat(mean(sample))

  writeLines("\n\nMedian:\n")
  cat(median(sample))

  writeLines("\n\nMode of sample:\n")
  cat(as.numeric(names(sort(-table(sample)))[1]))
}

# Calculate variation factors: standard deviation & coefficient of variation
calcVariationFactors <- function(sample)
{
  writeLines("\n\nStandard deviation:\n")
  cat(sd(sample))

  writeLines("\n\nCoefficient of variation:\n")
  cat(sd(sample) / mean(sample))
}

# Calculate histogram form factors: skewness & kurtosis
calcHistogramFormFactors <- function(sample)
{
  writeLines("\n\nSkewness:\n")
  cat(skewness(sample))
 
  writeLines("\n\nKurtosis:\n")
  cat(kurtosis(sample))
}

# Check "68-95-99.7 rule" function.
checkSigmaRule <- function(sample)
{
  b1 = 0
  b2 = 0
  b3 = 0
  mu = mean(sample)
  sigma = sd(sample)
  
  for(i in 1:length(sample))
  {
    if(sample[i] < mu + sigma && sample[i] > mu - sigma) {
      b1 = b1 + 1;
    }
    
    if(sample[i] < mu + 2 * sigma && sample[i] > mu - 2 * sigma) {
      b2 = b2 + 1;
    }

    if(sample[i] < mu + 3 * sigma && sample[i] > mu - 3 * sigma) {
      b3 = b3 + 1;
    }
  }

  writeLines("\n\nSigma rule:\n")
  cat(b1 / length(sample) * 100)
  writeLines("% ");
  cat(b2 / length(sample) * 100)
  writeLines("% ");
  cat(b3 / length(sample) * 100)
  writeLines("%\n");
}

# Main program function.
# Arguments:
#   fileName - name of file, which contains sample (one column of numbers)
#   clipCount - number of elements of sample to be deleted from the end & begin of variational series
calc <- function(fileName,  clipCount = 25) 
{
  readData(fileName, clipCount)

  sink("results/res.txt", append = FALSE)

  writeLines("Results for loaded sample.\n\n")

  calcDistrCenterFactors(main_sample)  
  calcVariationFactors(main_sample)
  calcHistogramFormFactors(main_sample)
  checkSigmaRule(main_sample)

  # Apply method of moments,
  # assuming that sample is from normal distribution
  m1 = moment(main_sample, order = 1)
  m2 = moment(main_sample, order = 2)

  main_sample_sigma <<- sqrt(m2 - m1 * m1)
  main_sample_mu <<- m1

  # Generate sample from normal distribution (main_sample_mu, main_sample_sigma)

  theor_sample <<- rnorm(length(main_sample), sd = main_sample_sigma, mean = main_sample_mu)

  writeLines("\n\n\nResults for theoretical sample.\n\n")

  calcDistrCenterFactors(theor_sample)  
  calcVariationFactors(theor_sample)
  calcHistogramFormFactors(theor_sample)
  checkSigmaRule(theor_sample)
  
  sink()

  buildHistogram(main_sample, "results/histogram_main.png")    
  buildHistogram(theor_sample, "results/histogram_theor.png")    
}