dyn.load("C:\\Rtools\\MySources\\random_pivot.dll")

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

  res <- .C("rp_augment",
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

random.pivot(c("r1", "r2", "r2", "r3"), c("c1", "c2", "c2", "c3"))*1
