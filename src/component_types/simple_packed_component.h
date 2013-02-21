/*
 * simple_packed_component.h
 *
 *  Created on: Feb 4, 2013
 *      Author: mthurley
 */

#ifndef SIMPLE_PACKED_COMPONENT_H_
#define SIMPLE_PACKED_COMPONENT_H_

#include "base_packed_component.h"
#include "component_archetype.h"
#include "component.h"


#include "../primitive_types.h"

class SimplePackedComponent : public BasePackedComponent {
public:

  SimplePackedComponent() {
  }

  inline SimplePackedComponent(Component &rComp);

//  SimplePackedComponent(Component &rComp, const mpz_class &model_count,
//      unsigned long time) :
//      SimplePackedComponent(rComp) {
//    model_count_ = model_count;
//    creation_time_ = time;
//  }

  unsigned num_variables() const{
     return (*data_) & variable_mask();
   }

};


SimplePackedComponent::SimplePackedComponent(Component &rComp) {
  unsigned data_size_vars = bits_per_variable() + rComp.num_variables() * bits_per_variable();
  unsigned data_size_clauses = rComp.numLongClauses() * bits_per_clause();
  unsigned data_size = (data_size_vars + data_size_clauses)/bits_per_block() + 1;

  data_size+=  ((data_size_vars + data_size_clauses) % bits_per_block())? 1 : 0;

  unsigned *p = data_ =  new unsigned[data_size];

  *p = rComp.num_variables();
  unsigned hashkey_vars = 0;
  unsigned int bitpos = bits_per_variable();

  for (auto it = rComp.varsBegin(); *it != varsSENTINEL; it++) {
    *p |= (*it) << bitpos;
    bitpos += bits_per_variable();
    hashkey_vars = (hashkey_vars *3) + *it;
    if (bitpos >= bits_per_block()) {
      bitpos -= bits_per_block();
      *(++p) = *it >> (bits_per_variable() - bitpos);
    }
  }

  unsigned hashkey_clauses = 0;

  if (*rComp.clsBegin()) {
    for (auto jt = rComp.clsBegin(); *jt != clsSENTINEL; jt++) {
      *p |= *jt << bitpos;
      bitpos += bits_per_clause();
      hashkey_clauses = (hashkey_clauses *3) + *jt;
      if (bitpos >= bits_per_block()) {
        bitpos -= bits_per_block();
        *(++p) = (*jt >> (bits_per_clause() - bitpos));
      }
    }
  }
  if (bitpos > 0)
    p++;
  *p = 0;
  hashkey_ = hashkey_vars + (((unsigned) hashkey_clauses) << 16);

  assert( p - data_ + 1 == data_size);
}

#endif /* SIMPLE_PACKED_COMPONENT_H_ */
