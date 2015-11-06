/**
 * Random pivot R module
 * Copyright (C) 2012,2015  Martin von Gagern
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <cstdlib>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#define MATHLIB_STANDALONE
#include <R.h>
#include <Rmath.h>

#ifdef ASSERT
#undefine ASSERT
#endif
#ifndef NDEBUG

namespace random_pivot {

  class AssertionError : public std::logic_error {
  public:
    AssertionError(const std::string& msg) : std::logic_error(msg) { }
  };

}

#define ASSERT(condition)				       \
  do { if (!(condition))				       \
      throw AssertionError                                     \
           ("Assertion " #condition " failed in " __FILE__);   \
  } while(0)
#else
#define ASSERT(condition)
#endif

namespace random_pivot {

  class Impossible : std::runtime_error {
  public:
    Impossible(const std::string& msg) : std::runtime_error(msg) { }
  };

  typedef std::vector<int> Numbers;

  class MatrixAugmentator {
  private:
    int nr, nc;
    int* rowDeficit;
    int* colDeficit;
    int* m;
    int deficit;
    int lastCol;

    int& v(int row, int col) { return m[col*nr + row]; }

    int chooseOne(const Numbers& values);
    int augmentRow(Numbers& inRows, const Numbers& outCols);
    int augmentCol(const Numbers& inRows, const Numbers& outCols);

  public:

    MatrixAugmentator
    (int numRows, int numCols,
     int* rowSums, int* colSums,
     int* matrix);

  };

  int MatrixAugmentator::chooseOne(const Numbers& values) {
    if (values.empty())
      throw Impossible("Empty set to choose from.");
    int index;
    index = int(double(values.size())*unif_rand());
    ASSERT(index >= 0);
    ASSERT(index < int(values.size()));
    return values[index];
  }

  inline int MatrixAugmentator::augmentCol
  (const Numbers& inRows, const Numbers& outCols)
  {
    // find all cols which have a 0 in one of the inRows
    Numbers zeroCols;
    Numbers::const_iterator io = outCols.begin(), eo = outCols.end();
    Numbers::const_iterator br = inRows.begin(), er = inRows.end();
    for (int c = 0; c != nc; ++c) {
      for (Numbers::const_iterator ir = br; ir != er; ++ir) {
        if (!v(*ir, c)) {
          if (io != eo && *io == c) {
            // We have found at least one augmenting path!
            // Now look for all zero columns matching inRows and outCols.
            zeroCols.clear();
            zeroCols.push_back(c);
            for (++io; io != eo; ++io) {
              for (ir = br; ir != er; ++ir) {
                if (!v(*ir, *io)) {
                  zeroCols.push_back(c);
                  break;
                }
              }
            }
            lastCol = chooseOne(zeroCols);
            return lastCol;
          }
          zeroCols.push_back(c);
          break;
        }
      }
      if (io != eo && *io == c)
        ++io;
    }

    // find all rows which have a 1 in one of the zeroCols
    Numbers oneRows;
    Numbers::iterator bc = zeroCols.begin(), ec = zeroCols.end();
    for (int r = 0; r != nr; ++r) {
      for (Numbers::iterator ic = bc; ic != ec; ++ic) {
        if (v(r, *ic)) {
          oneRows.push_back(r);
          break;
        }
      }
    }

    // recursively call augment
    int row = augmentRow(oneRows, outCols);

    // find the 1s in the selected row, restricted to zeroCols
    Numbers::size_type n = zeroCols.size();
    while (n != 0) {
      --n;
      if (!v(row, zeroCols[n])) {
        // not a 1, remove it from the list
        zeroCols[n] = zeroCols.back();
        zeroCols.pop_back();
      }
    }
    int col = chooseOne(zeroCols);
    ASSERT(v(row, col));
    v(row, col) = 0;
    return col;
  }

  int MatrixAugmentator::augmentRow
  (Numbers& inRows, const Numbers& outCols)
  {
    // choose a column where we want to go from our inRows
    int col = augmentCol(inRows, outCols);

    // restrict inRows to those which have a 0 in the selected column
    int* p = &v(0, col);
    Numbers::size_type n = inRows.size();
    while (n != 0) {
      --n;
      if (p[inRows[n]]) {
        // not a 0, remove it from the list
        inRows[n] = inRows.back();
        inRows.pop_back();
      }
    }
    int row = chooseOne(inRows);
    ASSERT(!v(row, col));
    v(row, col) = 1;
    return row;
  }

  MatrixAugmentator::MatrixAugmentator
  (int numRows, int numCols,
   int* rowSums, int* colSums,
   int* matrix)
    : nr(numRows), nc(numCols)
    , rowDeficit(rowSums), colDeficit(colSums)
    , m(matrix)
    , deficit(0)
  {
    int* p = m;
    for (int c = 0; c != nc; ++c) {
      for (int r = 0; r != nr; ++r) {
        rowDeficit[r] -= *p;
        colDeficit[c] -= *p;
        ++p;
      }
      deficit += colDeficit[c];
    }

    std::set<int> deficitRows;
    for (int r = 0; r != nr; ++r)
      if (rowDeficit[r])
        deficitRows.insert(deficitRows.end(), r);
    Numbers deficitCols;
    for (int c = 0; c != nc; ++c)
      if (colDeficit[c])
        deficitCols.push_back(c);

    while (deficit) {
      Numbers inRows(deficitRows.begin(), deficitRows.end());
      int row = augmentRow(inRows, deficitCols);
      if (--rowDeficit[row] == 0)
        deficitRows.erase(row);
      if (--colDeficit[lastCol] == 0) {
        Numbers::iterator pos = std::lower_bound
          (deficitCols.begin(), deficitCols.end(), lastCol);
        deficitCols.erase(pos);
      }
      --deficit;
    }
  }

} // namespace random_pivot

extern "C" {

  void random_pivot_augment
  (int* pNumRows, int* pNumCols,
   int* rowSums, int* colSums,
   int* matrix)
  {
    GetRNGstate();
    try {
      random_pivot::MatrixAugmentator
        (*pNumRows, *pNumCols, rowSums, colSums, matrix);
    }
    catch (std::exception &e) {
      error("%s", e.what());
    }
    PutRNGstate();
  }

}
