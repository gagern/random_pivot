# Random pivot R module

The goal of this code is to compute a random binary matrix
satisfying given marginal sums.
In other words, you specify the number of `TRUE` values
in each row and column of the matrix,
and get a matrix satisfying these counts.

## Algorithm

The algorithm is based on an initial incomplete fill
using random pairings between rows and columns,
followed by a search for augmenting paths
in a network flow model of the matrix.
The first step is performed in R code,
the second in a C++ implementation called from R.
As a third step, a trial swap may be used to approach equidistribution.
Such a third step is not part of the code at hand.

The algorithm has been mentioned in [*Problems with bins: A critical
reassessment of Gotelli and Ulrich's Bayes approach using bird
data*](http://dx.doi.org/10.1016/j.actao.2015.10.003) by von Gagern,
von Gagern and Schmitz Ornés (Acta Oecologica 69 (2015) pp. 137–145).
It has been discussed in more detail in
Appendix A available as an online supplemental to that paper.
In the long run,
the description should become part of this repository as well.

## Installation

The code should eventually become available as a public R package,
which can be downloaded from [CRAN](https://cran.r-project.org/).
Or perhaps it might be integrated into existing packages which
already perform some kind of matrix randomization.
But that requires some more work with packaging and portability testing,
so at the moment this is source code only for those who want to read it,
or for those brave enough to try installing it from this current form.

## Usage

The function `random.pivot` takes two arguments
which are turned into factors using `as.factor`.
They are interpreted as multisets,
i.e. the order of elements is irrelevant.
The number of occurrences of each level indicates
the desired marginal sum for that level.
So the rows and columns of the final matrix will correspond
to the levels of the first and second argument.

The output will be a logical matrix.
If you prefer the shorter binary notation
(i.e. using `1`/`0` instead of `TRUE`/`FALSE`),
simply multiply the result with `1`.

### Example

```
> random.pivot(c("r1", "r2", "r2", "r3"), c("c1", "c2", "c2", "c3"))*1
   c1 c2 c3
r1  0  1  0
r2  1  1  0
r3  0  0  1
> random.pivot(c("r1", "r2", "r2", "r3"), c("c1", "c2", "c2", "c3"))*1
   c1 c2 c3
r1  0  1  0
r2  0  1  1
r3  1  0  0
```

## Naming

Turning a pair of factors into a pivot table
(as Excel and therefore many other users call it)
is a common operation (and can be achieved in many ways in R,
including `xtabs`, `reshape` and functions from various packages).
In this sense, the code here creates a pivot table after randomizing
one of the vectors in a suitable way.
Hence the name, `random.pivot`.
