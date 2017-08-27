// Copyright (c) 2009, 2010, 2011 Object Computing, Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//    * Neither the name of Object Computing, Inc. nor the names of its
//      contributors may be used to endorse or promote products derived from this
//      software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <Common/QuickFASTPch.h>
#include "Profiler.h"
#include <math.h>

using namespace QuickFAST;

ProfileAccumulator * ProfileAccumulator::root_ = 0;

ProfileAccumulator::ProfileAccumulator(const char * name, const char * file, size_t line)
  : name_(name)
  , file_(file)
  , line_(line)
  , entries_(0)
  , exits_(0)
  , pauses_(0)
  , resumes_(0)
  , sum_(0.0)
  , sumOfSquares_(0.0)
  , recursions_(0)
  , recursiveSum_(0.0)
  , recursiveSumOfSquares_(0.0)

{
  next_ = root_;
  root_ = this;
}

void
ProfileAccumulator::write(std::ostream & out)
{
  const ProfileAccumulator * ac = root_;
  out << "name\tfile\tline\tentries\texits\tsum\tsum_of_squares"
    << "\trecursions\trecursive_sum\trecursive_sum_of_squares"
    // helpers
    << "\tnonRsum"
    << "\tnonRmean"
    << std::endl;
  while(ac != 0)
  {
    out << ac->name_
      << '\t' << ac->file_
      << '\t' << ac->line_
      << '\t' << ac->entries_
      << '\t' << ac->exits_
      << '\t' << ac->sum_
      << '\t' << ac->sumOfSquares_
      << '\t' << ac->recursions_
      << '\t' << ac->recursiveSum_
      << '\t' << ac->recursiveSumOfSquares_
      // helpers
      << '\t' << ac->sum_ - ac->recursiveSum_
      << '\t' << (ac->sum_ - ac->recursiveSum_) /double(ac->exits_ - ac->recursions_)
      << std::endl;
    ac = ac->next_;
  }
}

void
ProfileAccumulator::print(std::ostream & out)
{
  const ProfileAccumulator * ac = root_;
  out << "name\tcount\tsum\tmean\tstd_dev\trecursions\trsum\trmean\trstd_dev" << std::endl;
  while(ac != 0)
  {
    double count = double(ac->exits_ - ac->recursions_);
    double sum = ac->sum_ - ac->recursiveSum_;
    double sumsq = ac->sumOfSquares_ - ac->recursiveSumOfSquares_;

    out << ac->name_
      << '\t' << std::fixed << std::setprecision(0) << count
      << '\t' << std::fixed << std::setprecision(0) << sum;
    if(count > 1)
    {
      double mean = sum/ count;
      double stdDev = std::sqrt((sumsq - sum * mean) / (count - 1.0));
      out
        << '\t' << std::fixed << std::setprecision(3) << mean
        << '\t' << std::fixed << std::setprecision(3) << stdDev
        << '\t' << ac->recursions_;
      if(ac->recursions_ > 0)
      {
        double count = double(ac->recursions_);
        double sum = ac->recursiveSum_;
        double sumsq = ac->recursiveSumOfSquares_;
        double mean = sum/ count;
        double stdDev = std::sqrt((sumsq - sum * mean) / (count - 1.0));
        out << '\t' << std::fixed << std::setprecision(0) << sum
          << '\t' << std::fixed << std::setprecision(3) << mean
          << '\t' << std::fixed << std::setprecision(3) << stdDev;
      }
    }
    out << std::endl;
    ac = ac->next_;
  }
}


