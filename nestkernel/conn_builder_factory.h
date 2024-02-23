/*
 *  conn_builder_factory.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CONN_BUILDER_FACTORY_H
#define CONN_BUILDER_FACTORY_H

// C++ includes:
#include <map>

// Includes from nestkernel:
#include "conn_builder.h"

// Includes from sli:
#include "dictdatum.h"
#include "name.h"
#include "sharedptrdatum.h"

namespace nest
{
/**
 * Generic factory class for ConnBuilder objects.
 *
 * This factory allows for flexible registration
 * of ConnBuilder subclasses and object creation.
 *
 */
class GenericConnBuilderFactory
{
public:
  virtual ~GenericConnBuilderFactory()
  {
  }

  /**
   * Factory method for builders for bipartite connection rules (the default).
   */
  virtual BipartiteConnBuilder* create( NodeCollectionPTR,
    NodeCollectionPTR,
    ThirdOutBuilder*,
    const DictionaryDatum&,
    const std::vector< DictionaryDatum >& ) const = 0;
};

/**
 * Factory class for ConnBuilders
 */
template < typename ConnBuilderType >
class ConnBuilderFactory : public GenericConnBuilderFactory
{
public:
  BipartiteConnBuilder*
  create( NodeCollectionPTR sources,
    NodeCollectionPTR targets,
    ThirdOutBuilder* third_out,
    const DictionaryDatum& conn_spec,
    const std::vector< DictionaryDatum >& syn_specs ) const override
  {
    return new ConnBuilderType( sources, targets, third_out, conn_spec, syn_specs );
  }
};

class GenericThirdConnBuilderFactory
{
public:
  virtual ~GenericThirdConnBuilderFactory()
  {
  }

  /**
   * Factory method for builders for bipartite connection rules (the default).
   */
  virtual ThirdOutBuilder* create( NodeCollectionPTR,
    NodeCollectionPTR,
    const DictionaryDatum&,
    const std::vector< DictionaryDatum >& ) const = 0;
};

/**
 * Factory class for Third-factor ConnBuilders
 */
template < typename ThirdConnBuilderType >
class ThirdConnBuilderFactory : public GenericThirdConnBuilderFactory
{
public:
  ThirdOutBuilder*
  create( NodeCollectionPTR sources,
    NodeCollectionPTR targets,
    const DictionaryDatum& conn_spec,
    const std::vector< DictionaryDatum >& syn_specs ) const override
  {
    return new ThirdConnBuilderType( sources, targets, conn_spec, syn_specs );
  }
};


} // namespace nest

#endif
