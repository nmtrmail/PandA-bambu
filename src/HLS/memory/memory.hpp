/*
 *
 *                   _/_/_/    _/_/   _/    _/ _/_/_/    _/_/
 *                  _/   _/ _/    _/ _/_/  _/ _/   _/ _/    _/
 *                 _/_/_/  _/_/_/_/ _/  _/_/ _/   _/ _/_/_/_/
 *                _/      _/    _/ _/    _/ _/   _/ _/    _/
 *               _/      _/    _/ _/    _/ _/_/_/  _/    _/
 *
 *             ***********************************************
 *                              PandA Project 
 *                     URL: http://panda.dei.polimi.it
 *                       Politecnico di Milano - DEIB
 *                        System Architectures Group
 *             ***********************************************
 *              Copyright (c) 2004-2018 Politecnico di Milano
 *
 *   This file is part of the PandA framework.
 *
 *   The PandA framework is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
/**
 * @file memory.hpp
 * @brief Datastructure to represent memory information in high-level synthesis
 *
 * @author Christian Pilato <pilato@elet.polimi.it>
 * @author Fabrizio Ferrandi <fabrizio.ferrandi@polimi.it>
 * $Revision$
 * $Date$
 * Last modified by $Author$
 *
*/

#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include "refcount.hpp"
/**
 * @name forward declarations
 */
//@{
REF_FORWARD_DECL(application_manager);
REF_FORWARD_DECL(tree_manager);
REF_FORWARD_DECL(memory_symbol);
REF_FORWARD_DECL(structural_manager);
REF_FORWARD_DECL(structural_object);
//@}
class xml_element;

#include <set>
#include <map>

class memory
{
      ///datastructure containing tree information
      const tree_managerRef TreeM;

      ///set of variables allocated outside the top module
      std::map<unsigned int, memory_symbolRef> external;

      ///set of variables allocated internally to the cores, classified by function id
      std::map<unsigned int, std::map<unsigned int, memory_symbolRef> > internal;

      /// set of variable proxies accessed by a function
      std::map<unsigned int, std::set<unsigned int> > internal_variable_proxy;

      /// is the set of proxied variables
      std::set<unsigned int> proxied_variables;

      std::set<unsigned int> read_only_vars;

      ///set of all the internal variables
      std::map<unsigned int, memory_symbolRef> in_vars;

      /// for each var store the address space rangesize associated with it
      std::map<unsigned int, unsigned int> rangesize;

      ///set of variables allocated in registers of the interface
      std::map<unsigned int, std::map<unsigned int, memory_symbolRef> > parameter;
      ///set of all the internal parameters
      std::map<unsigned int, memory_symbolRef> params;
      ///set of call sites for __builtin_wait_call
      std::map<unsigned int, memory_symbolRef> callSites;

      /// store the objects that does not need to be attached to the bus
      std::set<unsigned int> private_memories;

      /// store if a given variable is accessed always with the same data_size or not
      std::map<unsigned int, unsigned int> same_data_size_accesses;

      /// ssa_names assigned to a given memory variable
      std::map<unsigned int, std::set<unsigned int> > source_values;

      /// parm_decl that has to be copied from the caller
      std::set<unsigned int> parm_decl_copied;

      /// parm_decl storage has to be initialized from the formal parameter
      std::set<unsigned int> parm_decl_stored;

      /// actual parameter that has to be loaded from a stored value
      std::set<unsigned int> actual_parm_loaded;

      ///it represents the next address that is available for internal allocation
      unsigned int next_base_address;

      /// is the start internal address
      unsigned int internal_base_address_start;

      /// is the maximum amount of private memory allocated
      unsigned int maximum_private_memory_size;

      /// total amount of internal memory allocated
      unsigned int total_amount_of_private_memory;

      /// total amount of parameter memory
      unsigned int total_amount_of_parameter_memory;

      ///it represents the base address of the external memory
      unsigned int off_base_address;

      ///it represents the next address that is available to allocate a variable out of the top module
      unsigned int next_off_base_address;

      /// bus data bitsize
      unsigned int bus_data_bitsize;

      /// bus address bitsize
      unsigned int &bus_addr_bitsize;

      /// bus size bitsize
      unsigned int bus_size_bitsize;

      /// maximum bitsize that can be accessed aligned
      unsigned int aligned_bitsize;

      /// bram bitsize
      unsigned int bram_bitsize;

      /// maximum bram bitsize
      unsigned int maxbram_bitsize;

      /// define if the Spec has data that can be externally accessed
      bool intern_shared_data;

      /// Spec accesses data having an address unknown at compile time
      bool use_unknown_addresses;

      /// true when at least one pointer conversion happen
      bool pointer_conversion;

      /// true when LOADs or STOREs perform unaligned accesses
      bool unaligned_accesses;

      /// is true when all pointers are resolved statically
      bool all_pointers_resolved;

      /// when true an implicit memcpy is called
      bool implicit_memcpy;

      /// internal address alignment
      unsigned int internal_base_address_alignment;

      /// external address alignment
      unsigned int external_base_address_alignment;

      /// parameter alignment
      const unsigned int parameter_alignment;

      /// number of LOAD/STORE per var
      std::map<unsigned int, unsigned int> n_mem_operations_per_var;

      /// when false object could be allocated starting from address 0
      bool null_pointer_check;

      /// define for each variable the number of references whenever it is possible
      std::map<unsigned int, size_t> maximum_references;

      /// define for each variable the number of loads whenever it is possible
      std::map<unsigned int, size_t> maximum_loads;

      /// true when packed vars are used
      bool packed_vars;


      /**
       * Alignment utility function
       */
      void align(unsigned int& address, unsigned int alignment)
      {
         if(address % alignment != 0)
            address = ((address / alignment) + 1) * alignment;
      }

   public:

      /**
       * Constructor
       */
      memory(const tree_managerRef TreeM, unsigned int off_base_address, unsigned int max_bram, bool null_pointer_check, bool initial_internal_address_p, unsigned int initial_internal_address, unsigned int &_address_bitsize);

      /**
       * Destructor
       */
      ~memory();

      /**
       * Return variables allocated out of the top module
       */
      std::map<unsigned int, memory_symbolRef> get_ext_memory_variables() const;

      /**
       * Allocates a variable to the set of variables allocated outside to outermost function
       */
      void add_external_variable(unsigned int var);

      /**
       * Allocates a variable to the set of variables allocated outside to outermost function. The corresponding symbol is already provided
       */
      void add_external_symbol(unsigned int var, const memory_symbolRef m_sym);

      /**
       * Allocates a variable to the set of variables allocated internally to the given function
       */
      void add_internal_variable(unsigned int funID_scope, unsigned int var);

      /**
       * allocate a proxy for the variable for the specified function
       * @param funID_scope is the function id
       * @param var is the variable
       */
      void add_internal_variable_proxy(unsigned int funID_scope, unsigned int var);

      /**
       * Compute the new base address based on the size of the given variable and align the memory as needed
       * @param address is the address to be evaluated
       * @param var is the variable that has to be reserved
       * @param alignment is the address alignment
       */
      void compute_next_base_address(unsigned int& address, unsigned int var, unsigned int alignment);

      /**
       * return the proxied internal variables associated with the function
       * @param funID_scope is the function id
       * @return the set of variables proxied in funID_scope
       */
      const std::set<unsigned int>& get_proxied_internal_variables(unsigned int funID_scope) const;

      /**
       * check if the function has proxied variables
       * @param funID_scope is the function id
       * @return true when there are proxied variables, false otherwise
       */
      bool has_proxied_internal_variables(unsigned int funID_scope) const;

      /**
       * return true if the variable is a proxied variable
       * @param var is the id of the variable
       * @return true when var is a proxied variable
       */
      bool is_a_proxied_variable(unsigned int var) const;

      /**
       * add a read only variable
       * @param var is the variable id
       */
      void add_read_only_variable(unsigned var);

      /**
       * return true when the variable is only read
       * @param var is the variable id
       * @return true when variable is only read, false otherwise
       */
      bool is_read_only_variable(unsigned var) const;

      /**
       * Allocates a variable to the set of variables allocated internally to the given function. The corresponding symbol is already provided
       */
      void add_internal_symbol(unsigned int funID_scope, unsigned int var, const memory_symbolRef m_sym);

      /**
       * add a var that safely cannot be attached to the bus
       * @param var is var index
       */
      void add_private_memory(unsigned int var);

      /**
       * set if a variable is always accessed with the same data size or not
       * @param var is the variable id
       * @param value is true when the variable is always accessed with the same data size or false otherwise
       */
      void set_sds_var(unsigned int var, bool value);

      /**
       * add a value to the set of values written in a given memory variable
       * @param var is the memory variable
       * @param value is the source value
       */
      void add_source_value(unsigned int var, unsigned int value);

      /**
       * return the set of values used to write a given memory variable
       * @param var is the memory variable
       * @return the set of values associated with var
       */
      const std::set<unsigned int>& get_source_values(unsigned int var) {return source_values[var];}

      /**
       * Allocates a parameter to the set of the interface registers
       */
      void add_parameter(unsigned int funID_scope, unsigned int var, bool is_last);

      /**
       * Allocates a parameter to the set of the interface registers
       */
      void add_parameter_symbol(unsigned int funID_scope, unsigned int var, const memory_symbolRef m_sym);

      /**
       * Test if a variable is allocated into the specified function
       */
      bool is_internal_variable(unsigned int funID_scope, unsigned int var) const;

      /**
       * Test if a variable is into the set of variables out of the top function
       */
      bool is_external_variable(unsigned int var) const;

      /**
       * Return true in case the variable is private
       * @param var is the index of the variable
       * @return true in case the variable is private
       */
      bool is_private_memory(unsigned int var) const;

      /**
       * check if the variable is always accessed with the same data size
       * @param var is the variable id
       * @return true when the variable is always accessed with the same data size, false otherwise
       */
      bool is_sds_var(unsigned int var) const;

      /**
       * return true if the var has been classified in term of same data size relation
       * @param var is the variable id
       * @return true when var is in the sds relation, false otherwise
       */
      bool has_sds_var(unsigned int var) const;

      /**
       * Test if a variable is into the set of interface registers
       */
      bool is_parameter(unsigned int funID_scope, unsigned int var) const;

      /**
       * Get call sites map.
       */
      const std::map<unsigned int, memory_symbolRef> get_callSites() const { return callSites; }

      /**
       * Get the current base address of the given call site
       */
      unsigned int get_callSite_base_address(unsigned int var) const;

      /**
       * Get the current base address of the given variable
       */
      unsigned int get_internal_base_address(unsigned int var) const;

      /**
       * Get the current base address of the given variable
       */
      unsigned int get_external_base_address(unsigned int var) const;

      /**
       * Get the current base address of the given variable
       */
      unsigned int get_parameter_base_address(unsigned int funId, unsigned int var) const;

      /**
       * Get the current base address of the given variable
       */
      unsigned int get_base_address(unsigned int var, unsigned int funId) const;

      /**
       * Get the first address of the function address space.
       */
      unsigned int get_first_address(unsigned int funId) const;

      /**
       * Get the last address of the function address space.
       */
      unsigned int get_last_address(unsigned int funId, const application_managerRef AppM) const;

      /**
       * Return the symbol associated with the given variable
       */
      memory_symbolRef get_symbol(unsigned int var, unsigned int funId) const;

      /**
       * return the address space rangesize associated with the given var
       * @param var is the variable considered
       */
      unsigned int get_rangesize(unsigned int var) const;

      /**
       * Check if there is a base address for the given call site.
       */
      bool has_callSite_base_address(unsigned int var) const;

      /**
       * Check if there is a base address for the given variable
       */
      bool has_internal_base_address(unsigned int var) const;

      /**
       * @return the numnber of LOAD/STORE operations per var
       */
      unsigned int get_n_mem_operations(unsigned int var) const {return n_mem_operations_per_var.find(var)->second;}

      void increment_n_mem_operations(unsigned int var)
      {
         if (n_mem_operations_per_var.find(var) == n_mem_operations_per_var.end())
            n_mem_operations_per_var[var] = 1;
         else
            ++n_mem_operations_per_var[var];
      }

      /**
       * Check if there is a base address for the given variable
       */
      bool has_external_base_address(unsigned int var) const;

      /**
       * Check if there is a base address for the given parameter
       */
      bool has_parameter_base_address(unsigned int var, unsigned int funId) const;

      /**
       * Check if there is a base address for the given variable
       */
      bool has_base_address(unsigned int var) const;

      /**
       * return true in case the parm_decl parameter has to be copied from the caller
       */
      bool is_parm_decl_copied(unsigned int var) const;

      /**
       * add a parm_decl to the set of parm_decl written
       */
      void add_parm_decl_copied(unsigned int var);

      /**
       * return true in case the parm_decl parameter has to be initialized from the formal value
       */
      bool is_parm_decl_stored(unsigned int var) const;

      /**
       * add a parm_decl to the set of parm_decl that has to be initialized
       */
      void add_parm_decl_stored(unsigned int var);

      /**
       * return true in case the actual parameter has to be initialized from a stored value
       */
      bool is_actual_parm_loaded(unsigned int var) const;

      /**
       * add an actual parameter to the set of parameter that has to be initialized from a stored value
       */
      void add_actual_parm_loaded(unsigned int var);


      /**
       * Return the variables allocated within the space of a given function
       */
      std::map<unsigned int, memory_symbolRef> get_function_vars(unsigned int funID_scope) const;

      /**
       * Return parameters allocated in register of the interface
       */
      std::map<unsigned int, memory_symbolRef> get_function_parameters(unsigned int funID_scope) const;

      /**
       * Return the first memory address not yet allocated
       */
      unsigned int get_memory_address() const;

      /**
       * Explicitly allocate a certain space in the external memory
       */
      void reserve_space(unsigned int space);

      /**
       * Returns the amount of memory allocated internally to the module
       */
      unsigned int get_allocated_space() const;

      /**
       * Return the total amount of memory allocated for the memory mapped parameters
       */
      unsigned int get_allocated_parameters_memory() const;

      /**
       * Returns the amount of memory allocated internally but not private
       */
      unsigned int get_allocated_intern_memory() const { return next_base_address-internal_base_address_start;}

      /**
       * return the maximum address allocated
       */
      unsigned int get_max_address() const;

      /**
       * set the bus data bitsize
       */
      void set_bus_data_bitsize(unsigned int bitsize) {bus_data_bitsize=bitsize;}

      /**
       * return the bitsize of the data bus
       */
      unsigned int get_bus_data_bitsize() const {return bus_data_bitsize;}

      /**
       * set the bus address bitsize
       */
      void set_bus_addr_bitsize(unsigned int bitsize) {bus_addr_bitsize=bitsize;}

      /**
       * return the bitsize of the address bus
       */
      unsigned int get_bus_addr_bitsize() const {return bus_addr_bitsize;}

      /**
       * set the bus size bitsize
       */
      void set_bus_size_bitsize(unsigned int bitsize) {bus_size_bitsize=bitsize;}

      /**
       * return the bitsize of the size bus
       */
      unsigned int get_bus_size_bitsize() const {return bus_size_bitsize;}

      /**
       * set the maximum the bitsize of the aligned accesses
       */
      void set_aligned_bitsize(unsigned int bitsize) {aligned_bitsize=bitsize;}

      /**
       * return the maximum bitsize of the aligned accesses
       */
      unsigned int get_aligned_bitsize() const {return aligned_bitsize;}

      /**
       * set the BRAM bitsize
       */
      void set_bram_bitsize(unsigned int bitsize) {bram_bitsize=bitsize;}

      /**
       * return the BRAM bitsize
       */
      unsigned int get_bram_bitsize() const {return bram_bitsize;}

      /**
       * set the maximum BRAM bitsize
       */
      void set_maxbram_bitsize(unsigned int bitsize) {maxbram_bitsize=bitsize;}

      /**
       * return the BRAM bitsize
       */
      unsigned int get_maxbram_bitsize() const {return maxbram_bitsize;}

      /**
       * define if the Spec has data that can be externally accessed
       */
      void set_intern_shared_data(bool has_accesses) { intern_shared_data=has_accesses;}

      /**
       *return true in case the specification has data that can be externally accessed
       */
      bool has_intern_shared_data() const {return intern_shared_data;}

      /**
       * define the maximum number of references for a given variable
       * @param var is variable
       * @param n_refs is the number of references
       */
      void set_maximum_references(unsigned int var, size_t n_refs) {maximum_references[var]=n_refs;}

      /**
       * return the maximum number of references for a given variable
       * @param var is the variable
       * @return the maximum number of references for var
       */
      size_t get_maximum_references(unsigned int var) const {return maximum_references.find(var) !=  maximum_references.end() ? maximum_references.find(var)->second : 0;}

      /**
       * define the maximum number of loads for a given variable
       * @param var is variable
       * @param n_refs is the number of loads
       */
      void set_maximum_loads(unsigned int var, size_t n_refs) {maximum_loads[var]=n_refs;}

      /**
       * return the maximum number of loads for a given variable
       * @param var is the variable
       * @return the maximum number of loads for var
       */
      size_t get_maximum_loads(unsigned int var) const {return maximum_loads.find(var) !=  maximum_loads.end() ? maximum_loads.find(var)->second : 0;}

      /**
       * define if there exist an object used by the Spec with an address not known at compile time
       */
      void set_use_unknown_addresses(bool accesses) {use_unknown_addresses=accesses;}

      /**
       *return true in case the specification use addresses not known at compile time
       */
      bool has_unknown_addresses() const {return use_unknown_addresses;}

      /**
       * set if a pointer conversion happen
       */
      void set_pointer_conversion(bool accesses) {pointer_conversion=accesses;}

      /**
       *return true in case at least one pointer conversion happen
       */
      bool has_pointer_conversion() const {return pointer_conversion;}

      /**
       * set if LOADs or STOREs perform unaligned accesses
       */
      void set_unaligned_accesses(bool accesses) {unaligned_accesses=accesses;}

      /**
       *return true in case of unaligned accesses
       */
      bool has_unaligned_accesses() const {return unaligned_accesses;}

      /**
       * define if all pointers have statically resolved
       */
      void set_all_pointers_resolved(bool resolved) {all_pointers_resolved=resolved;}

      /**
       *return true in case all pointers have been statically resolved
       */
      bool has_all_pointers_resolved() const {return all_pointers_resolved;}

      /**
       * @brief update the the packed variables status
       * @param packed is true when there is at least one packed variables
       */
      void set_packed_vars(bool packed) {packed_vars = packed_vars || packed;}

      /**
       * @return true in case packed vars are used
       */
      bool has_packed_vars() const {return packed_vars;}

      /**
       * define if there exist an implicit call of memcpy
       */
      void set_implicit_memcpy(bool cond) {implicit_memcpy=cond;}

      /**
       *return true in case the specification has an implicit call of memcpy
       */
      bool has_implicit_memcpy() const {return implicit_memcpy;}

      /**
       * return the internal base address alignment.
       */
      unsigned int get_internal_base_address_alignment() const { return internal_base_address_alignment; }

      /**
       * return the parameter alignment
       */
      unsigned int get_parameter_alignment() const { return parameter_alignment;}

      /**
       * set the internal base address alignment
       * @param _internal_base_address_alignment is the new alignment
       */
      void set_internal_base_address_alignment(unsigned int _internal_base_address_alignment);

      /**
       * Propagates the memory parameters from the source (innermost) module to the target (outermost) one
       */
      static
      void propagate_memory_parameters(const structural_objectRef src, const structural_managerRef tgt);

      /**
       * Adds the given memory parameter to the corresponding object
       */
      static
      void add_memory_parameter(const structural_managerRef SM, const std::string& name, const std::string& value);

      /**
       * Writes the current memory allocation into an XML description
       */
      void xwrite(xml_element* node);

      /**
       * @return return the number of internal symbols non private
       */
      unsigned int count_non_private_internal_symbols() const;

};
///refcount definition of the class
typedef refcount<memory> memoryRef;

#endif
