#ifndef _SENTENCE_H_
#define _SENTENCE_H_

#include <iostream>
#include <string>
#include <vector>
#include <ext/hash_map>

#include "mhs.h"    // random.h is included here
#include "util.h"   // namespace tr1 defined here
//#include "SGLexicon.h"
#include "Unigrams.h"   
#include "Base.h"   

using std::vector;
using std::string;

typedef vector<F> Fs;
typedef vector<U> Us;
typedef unsigned char B;
typedef vector<B> Bs;
typedef int I;
typedef vector<I> Is;
class Data;
class Scoring; 

class Sentence: public S {
public:
  typedef std::vector<S> Words;
  typedef Words::const_iterator Words_iterator;

  Sentence() {}
  Sentence(U start, U end, Bs possible_boundaries, 
	   Bs true_boundaries, const Data* d);
  Words get_segmented_words() const {return get_words(_boundaries);}
  Words get_reference_words() const {return get_words(_true_boundaries);}
  std::wostream&  print(std::wostream& os) const;
  void insert_words(Unigrams& lex) const; //add counts of whole sentence
  void erase_words(Unigrams& lex); //remove counts of whole sentence
  void insert_words(Bigrams& lex) const; //add counts of whole sentence
  void erase_words(Bigrams& lex); //remove counts of whole sentence
  U sample_by_flips(Unigrams& lex, F temperature);
  U sample_by_flips(Bigrams& lex, F temperature);
  void sample_one_flip(Unigrams& lex, F temperature, U boundary_within_sentence); // for DecayedMCMC
  void sample_one_flip(Bigrams& lex, F temperature, U boundary_within_sentence); // for DecayedMCMC
  void maximize(Unigrams& lex, U nsentences, F temperature, bool do_mbdp = 0);
  void maximize(Bigrams& lex, U nsentences, F temperature);
  void sample_tree(Unigrams& lex, U nsentences, F temperature, bool do_mbdp = 0);
  void sample_tree(Bigrams& lex, U nsentences, F temperature);
  void score(Scoring& scoring) const;
  friend std::wostream& operator<< (std::wostream& os, const Sentence& s);
  Us get_possible_boundaries(){return _possible_boundaries;};
  
  Bs _boundaries;
private:
  Us _possible_boundaries;
  Us _padded_possible; // for use with dynamic programming
  Bs _true_boundaries;
  const Data* _constants;
  S word_at(U left, U right) const {
    return S(left + begin_index(), right + begin_index());
  }
  Words get_words(const Bs& boundaries) const;
  void insert(U left, U right, Unigrams& lex) const;
  void erase(U left, U right, Unigrams& lex) const;
  void insert(U i0, U i1, U i2, Bigrams& lex) const;
  void erase(U i0, U i1, U i2, Bigrams& lex) const;
  F prob_boundary(U i1, U i, U i2, const Unigrams& lex, F temperature) const;
  F p_bigram(U i1, U i, U i2, const Bigrams& lex) const;
  F prob_boundary(U i0, U i1, U i, U i2, U i3, const Bigrams& lex, F temperature) const;
  void surrounding_boundaries(U i, U& i0, U& i1, U& i2, U& i3) const;
  F mbdp_prob(Unigrams& lex, const S& word, U nsentences) const;
};

typedef std::vector<Sentence> Sentences;



#endif
