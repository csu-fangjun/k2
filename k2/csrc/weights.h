// k2/csrc/weights.h

// Copyright     2020  Daniel Povey

// See ../../COPYING for clarification regarding multiple authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
// WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABILITY OR NON-INFRINGEMENT.
// See the Apache 2 License for the specific language governing permissions and
// limitations under the License.

#include <vector>

#include "k2/csrc/fsa.h"
#include "k2/csrc/fsa_util.h"
#include "k2/csrc/fsa_properties.h"


#ifndef K2_CSRC_WEIGHTS_H_
#define K2_CSRC_WEIGHTS_H_

namespace k2 {

/*
  This header contains various utilities that operate on weights and costs.
  We use the term "weights" when we have a positive-is-better sign,
  as in logprobs, and "costs" when we have a negative-is-better sign,
  as in negative logprobs.

  Note: weights are stored separately from the FSA.
 */





/*
  Does the 'forward' computation; this is as in the tropical semiring
  but with the opposite sign, as in logprobs rather than negative logprobs.
  It's like shortest path but with the opposite sign.

   @param [in] fsa  The fsa we are doing the forward computation on.
                Must satisfy IsValid(fsa) and IsTopSorted(fsa).
   @param [out] state_weights  The per-state weights will be written to here.
                They will be 0 for the start-state (if fsa is
                nonempty), and for later states will be the
                largest (most positive) weight from the start-state
                to that state along any path, or +infinity if no such
                path exists.
 */
void ComputeForwardMaxWeights(
    const Fsa &fsa, float *state_weights);

/*
  Does the 'backward' computation; this is as in the tropical semiring
  but with the opposite sign, as in logprobs rather than negative logprobs.
  It's like shortest path but with the opposite sign.

   @param [in] fsa  The fsa we are doing the backward computation on.
                Must satisfy IsValid(fsa) and IsTopSorted(fsa).
   @param [out] state_weights  The per-state weights will be written to here.
                They will be 0 for the final-state (if fsa is
                nonempty), and for earlier states will be the
                largest (most positive) weight from that
                to the final state along any path, or +infinity if no such
                path exists.
 */
void ComputeBackwardMaxWeights(
    const Fsa &fsa, float *state_weights);




enum {
  kMaxWeight,
  kLogSumWeight
} FbWeightType;





struct WfsaWithFbWeights {
  const Fsa *fsa;
  const float *arc_weights;
  const float *forward_state_weights;
  const float *backward_state_weights;

  /*
    Constructor.
       @param [in] fsa  Pointer to an FSA; must satisfy
            IsValid(*fsa) and IsTopSorted(*fsa).
       @param [in]  arc_weights  Arc weights, indexed by arc in `fsa`.
                                 Usually logprobs.
       @param [in]  t   Type of operation used to get forward
                        weights.  kMaxWeight == Viterbi, i.e. get
                        path with most positive weight sequence.
                        kLogSumWeight == Baum Welch, i.e. sum probs
                        over paths, treating weights as log-probs.
   */
  WfsaWithFbWeights(const Fsa *fsa,
                    const float *arc_weights,
                    FbWeightType t);
 private:
  std::vector<float> mem_;
};

}  // namespace k2

#endif  // K2_CSRC_WEIGHTS_H_
