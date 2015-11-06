\encoding{UTF-8}
\name{random.pivot}
\alias{random.pivot}
\title{Randomized Binary Matrices Satisfying Given Marginal Sums}
\description{Creates randomized binary (i.e. logical) matrices
  with a given number of cells selected in each row and column.}
\usage{random.pivot(rows, cols, do.assertions=TRUE)}
\arguments{
  \item{rows}{
    A multiset indicating the number of set elements in each row.
    This argument will be converted to a factor using \code{\link{as.factor}}.
    The order of elements in this vector is irrelevant.
    The number of occurrences of each level indicates
    the desired row sum for that level.
    The rows of the resulting matrix will correspond
    to the levels of this factor.
  }
  \item{cols}{
    A multiset indicating the number of set elements in each column.
    The data is interpreted analogous to the \code{rows} argument.
  }
  \item{do.assertions}{
    Whether to perform some additional checks.
    Currently this checks whether the requiested marginal sums
    are indeed satisfied by the generated matrix.
  }
}
\value{
  The output will be a logical matrix,
  with rows and columns corresponding to the levels
  of the first and second argument.

  If you prefer the shorter numeric notation
  (i.e. using \code{1}/\code{0} instead of \code{TRUE}/\code{FALSE}),
  simply multiply the result with `1`.
}
\references{
  von Gagern, von Gagern and Schmitz Ornés (2015),
  \href{http://dx.doi.org/10.1016/j.actao.2015.10.003}{
    Problems with bins:
    A critical reassessment of Gotelli and Ulrich's
    Bayes approach using bird data},
  \emph{Acta Oecologica} 69, 137--145.
}
\seealso{
  The \code{tswap} method of \code{\link[vegan]{commsim}} from the
  \code{vegan} package can be used to post-process the resulting matrix,
  achieving results closer to uniform distribution.
}
\examples{
## We want reproducible examples
set.seed(1248163264)

## Generate a random 3x3 matrix with sums 1,2,1 for rows and columns
random.pivot(c("r1", "r2", "r2", "r3"), c("c1", "c2", "c2", "c3"))

## Generate another matrix following the same rules, but print it more compact
random.pivot(c("r1", "r2", "r2", "r3"), c("c1", "c2", "c2", "c3"))*1
}
