# Random pivot R module
# Copyright (C) 2012,2015  Martin von Gagern
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

random.pivot <- function(rows, cols, do.assertions=TRUE) {
  rf <- as.factor(rows)
  cf <- as.factor(cols)
  n <- length(rf)
  rl <- levels(rf)
  cl <- levels(cf)
  row.counts <- as.numeric(table(rf))
  col.counts <- as.numeric(table(cf))
  if (sum(row.counts) != sum(col.counts)) stop("count mismatch")

  # Initialize matrix by randomized subscription.  This might write
  # TRUE repeatedly to the same cell, so the result might still not
  # reach the required counts.
  m <- matrix(FALSE, length(rl), length(cl))
  m[matrix(c(sample(rf), sample(cf)), n, 2)] <- TRUE

  res <- .C(random_pivot_augment,
            as.integer(nrow(m)), as.integer(ncol(m)),
            as.integer(row.counts), as.integer(col.counts),
            m=as.logical(m))$m

  dim(res) <- dim(m)
  rownames(res) <- rl
  colnames(res) <- cl
  if (do.assertions) {
    stopifnot(all(rowSums(res) == row.counts), all(colSums(res) == col.counts))
  }
  return(res)
}
