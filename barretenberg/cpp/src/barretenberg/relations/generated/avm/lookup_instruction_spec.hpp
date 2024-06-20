

#pragma once

#include "barretenberg/relations/generic_lookup/generic_lookup_relation.hpp"

#include <cstddef>
#include <tuple>

namespace bb {

/**
 * @brief This class contains an example of how to set LookupSettings classes used by the
 * GenericLookupRelationImpl class to specify a scaled lookup
 *
 * @details To create your own lookup:
 * 1) Create a copy of this class and rename it
 * 2) Update all the values with the ones needed for your lookup
 * 3) Update "DECLARE_LOOKUP_IMPLEMENTATIONS_FOR_ALL_SETTINGS" and "DEFINE_LOOKUP_IMPLEMENTATIONS_FOR_ALL_SETTINGS" to
 * include the new settings
 * 4) Add the relation with the chosen settings to Relations in the flavor (for example,"`
 *   using Relations = std::tuple<GenericLookupRelation<ExampleXorLookupSettings,
 * FF>>;)`
 *
 */
class lookup_instruction_spec_lookup_settings {
  public:
    /**
     * @brief The number of read terms (how many lookups we perform) in each row
     *
     */
    static constexpr size_t READ_TERMS = 1;
    /**
     * @brief The number of write terms (how many additions to the lookup table we make) in each row
     *
     */
    static constexpr size_t WRITE_TERMS = 1;

    /**
     * @brief The type of READ_TERM used for each read index (basic and scaled)
     *
     */
    static constexpr size_t READ_TERM_TYPES[READ_TERMS] = { 0 };

    /**
     * @brief They type of WRITE_TERM used for each write index
     *
     */
    static constexpr size_t WRITE_TERM_TYPES[WRITE_TERMS] = { 0 };

    /**
     * @brief How many values represent a single lookup object. This value is used by the automatic read term
     * implementation in the relation in case the lookup is a basic or scaled tuple and in the write term if it's a
     * basic tuple
     *
     */
    static constexpr size_t LOOKUP_TUPLE_SIZE = 56;

    /**
     * @brief The polynomial degree of the relation telling us if the inverse polynomial value needs to be computed
     *
     */
    static constexpr size_t INVERSE_EXISTS_POLYNOMIAL_DEGREE = 4;

    /**
     * @brief The degree of the read term if implemented arbitrarily. This value is not used by basic and scaled read
     * terms, but will cause compilation error if not defined
     *
     */
    static constexpr size_t READ_TERM_DEGREE = 0;

    /**
     * @brief The degree of the write term if implemented arbitrarily. This value is not used by the basic write
     * term, but will cause compilation error if not defined
     *
     */

    static constexpr size_t WRITE_TERM_DEGREE = 0;

    /**
     * @brief If this method returns true on a row of values, then the inverse polynomial exists at this index.
     * Otherwise the value needs to be set to zero.
     *
     * @details If this is true then the lookup takes place in this row
     *
     */

    template <typename AllEntities> static inline auto inverse_polynomial_is_computed_at_row(const AllEntities& in)
    {
        return (in.main_sel_not_first == 1 || in.instr_spec_sel_instr_spec == 1);
    }

    /**
     * @brief Subprocedure for computing the value deciding if the inverse polynomial value needs to be checked in this
     * row
     *
     * @tparam Accumulator Type specified by the lookup relation
     * @tparam AllEntities Values/Univariates of all entities row
     * @param in Value/Univariate of all entities at row/edge
     * @return Accumulator
     */

    template <typename Accumulator, typename AllEntities>
    static inline auto compute_inverse_exists(const AllEntities& in)
    {
        using View = typename Accumulator::View;
        const auto is_operation = View(in.main_sel_not_first);
        const auto is_table_entry = View(in.instr_spec_sel_instr_spec);
        return (is_operation + is_table_entry - is_operation * is_table_entry);
    }

    /**
     * @brief Get all the entities for the lookup when need to update them
     *
     * @details The generic structure of this tuple is described in ./generic_lookup_relation.hpp . The following is
     description for the current case:
     The entities are returned as a tuple of references in the following order (this is for ):
     * - The entity/polynomial used to store the product of the inverse values
     * - The entity/polynomial that specifies how many times the lookup table entry at this row has been looked up
     * - READ_TERMS entities/polynomials that enable individual lookup operations
     * - The entity/polynomial that enables adding an entry to the lookup table in this row
     * - LOOKUP_TUPLE_SIZE entities/polynomials representing the basic tuple being looked up as the first read term
     * - LOOKUP_TUPLE_SIZE entities/polynomials representing the previous accumulators in the second read term
     (scaled tuple)
     * - LOOKUP_TUPLE_SIZE entities/polynomials representing the shifts in the second read term (scaled tuple)
     * - LOOKUP_TUPLE_SIZE entities/polynomials representing the current accumulators in the second read term
     (scaled tuple)
     * - LOOKUP_TUPLE_SIZE entities/polynomials representing basic tuples added to the table
     *
     * @return All the entities needed for the lookup
     */

    template <typename AllEntities> static inline auto get_const_entities(const AllEntities& in)
    {

        return std::forward_as_tuple(in.lookup_instruction_spec,
                                     in.lookup_instruction_spec_counts,
                                     in.main_sel_not_first,
                                     in.instr_spec_sel_instr_spec,
                                     in.main_opcode_val,
                                     in.main_sel_op_sender,
                                     in.main_sel_op_address,
                                     in.main_sel_op_storage_address,
                                     in.main_sel_op_chain_id,
                                     in.main_sel_op_version,
                                     in.main_sel_op_block_number,
                                     in.main_sel_op_coinbase,
                                     in.main_sel_op_timestamp,
                                     in.main_sel_op_fee_per_l2_gas,
                                     in.main_sel_op_fee_per_da_gas,
                                     in.main_sel_op_transaction_fee,
                                     in.main_sel_op_l2gasleft,
                                     in.main_sel_op_dagasleft,
                                     in.main_sel_op_note_hash_exists,
                                     in.main_sel_op_emit_note_hash,
                                     in.main_sel_op_nullifier_exists,
                                     in.main_sel_op_emit_nullifier,
                                     in.main_sel_op_l1_to_l2_msg_exists,
                                     in.main_sel_op_emit_unencrypted_log,
                                     in.main_sel_op_emit_l2_to_l1_msg,
                                     in.main_sel_op_get_contract_instance,
                                     in.main_sel_op_sload,
                                     in.main_sel_op_sstore,
                                     in.main_sel_op_radix_le,
                                     in.main_sel_op_sha256,
                                     in.main_sel_op_poseidon2,
                                     in.main_sel_op_keccak,
                                     in.main_sel_op_pedersen,
                                     in.main_sel_op_add,
                                     in.main_sel_op_sub,
                                     in.main_sel_op_mul,
                                     in.main_sel_op_div,
                                     in.main_sel_op_fdiv,
                                     in.main_sel_op_not,
                                     in.main_sel_op_eq,
                                     in.main_sel_op_and,
                                     in.main_sel_op_or,
                                     in.main_sel_op_xor,
                                     in.main_sel_op_cast,
                                     in.main_sel_op_lt,
                                     in.main_sel_op_lte,
                                     in.main_sel_op_shl,
                                     in.main_sel_op_shr,
                                     in.main_sel_op_internal_call,
                                     in.main_sel_op_internal_return,
                                     in.main_sel_op_jump,
                                     in.main_sel_op_jumpi,
                                     in.main_sel_op_halt,
                                     in.main_sel_op_external_call,
                                     in.main_sel_op_mov,
                                     in.main_sel_op_cmov,
                                     in.main_sel_mem_op_a,
                                     in.main_sel_mem_op_b,
                                     in.main_sel_mem_op_c,
                                     in.main_sel_mem_op_d,
                                     in.main_clk,
                                     in.instr_spec_sel_op_sender,
                                     in.instr_spec_sel_op_address,
                                     in.instr_spec_sel_op_storage_address,
                                     in.instr_spec_sel_op_chain_id,
                                     in.instr_spec_sel_op_version,
                                     in.instr_spec_sel_op_block_number,
                                     in.instr_spec_sel_op_coinbase,
                                     in.instr_spec_sel_op_timestamp,
                                     in.instr_spec_sel_op_fee_per_l2_gas,
                                     in.instr_spec_sel_op_fee_per_da_gas,
                                     in.instr_spec_sel_op_transaction_fee,
                                     in.instr_spec_sel_op_l2gasleft,
                                     in.instr_spec_sel_op_dagasleft,
                                     in.instr_spec_sel_op_note_hash_exists,
                                     in.instr_spec_sel_op_emit_note_hash,
                                     in.instr_spec_sel_op_nullifier_exists,
                                     in.instr_spec_sel_op_emit_nullifier,
                                     in.instr_spec_sel_op_l1_to_l2_msg_exists,
                                     in.instr_spec_sel_op_emit_unencrypted_log,
                                     in.instr_spec_sel_op_emit_l2_to_l1_msg,
                                     in.instr_spec_sel_op_get_contract_instance,
                                     in.instr_spec_sel_op_sload,
                                     in.instr_spec_sel_op_sstore,
                                     in.instr_spec_sel_op_radix_le,
                                     in.instr_spec_sel_op_sha256,
                                     in.instr_spec_sel_op_poseidon2,
                                     in.instr_spec_sel_op_keccak,
                                     in.instr_spec_sel_op_pedersen,
                                     in.instr_spec_sel_op_add,
                                     in.instr_spec_sel_op_sub,
                                     in.instr_spec_sel_op_mul,
                                     in.instr_spec_sel_op_div,
                                     in.instr_spec_sel_op_fdiv,
                                     in.instr_spec_sel_op_not,
                                     in.instr_spec_sel_op_eq,
                                     in.instr_spec_sel_op_and,
                                     in.instr_spec_sel_op_or,
                                     in.instr_spec_sel_op_xor,
                                     in.instr_spec_sel_op_cast,
                                     in.instr_spec_sel_op_lt,
                                     in.instr_spec_sel_op_lte,
                                     in.instr_spec_sel_op_shl,
                                     in.instr_spec_sel_op_shr,
                                     in.instr_spec_sel_op_internal_call,
                                     in.instr_spec_sel_op_internal_return,
                                     in.instr_spec_sel_op_jump,
                                     in.instr_spec_sel_op_jumpi,
                                     in.instr_spec_sel_op_halt,
                                     in.instr_spec_sel_op_external_call,
                                     in.instr_spec_sel_op_mov,
                                     in.instr_spec_sel_op_cmov,
                                     in.instr_spec_sel_mem_op_a,
                                     in.instr_spec_sel_mem_op_b,
                                     in.instr_spec_sel_mem_op_c,
                                     in.instr_spec_sel_mem_op_d);
    }

    /**
     * @brief Get all the entities for the lookup when we only need to read them
     * @details Same as in get_const_entities, but nonconst
     *
     * @return All the entities needed for the lookup
     */

    template <typename AllEntities> static inline auto get_nonconst_entities(AllEntities& in)
    {

        return std::forward_as_tuple(in.lookup_instruction_spec,
                                     in.lookup_instruction_spec_counts,
                                     in.main_sel_not_first,
                                     in.instr_spec_sel_instr_spec,
                                     in.main_opcode_val,
                                     in.main_sel_op_sender,
                                     in.main_sel_op_address,
                                     in.main_sel_op_storage_address,
                                     in.main_sel_op_chain_id,
                                     in.main_sel_op_version,
                                     in.main_sel_op_block_number,
                                     in.main_sel_op_coinbase,
                                     in.main_sel_op_timestamp,
                                     in.main_sel_op_fee_per_l2_gas,
                                     in.main_sel_op_fee_per_da_gas,
                                     in.main_sel_op_transaction_fee,
                                     in.main_sel_op_l2gasleft,
                                     in.main_sel_op_dagasleft,
                                     in.main_sel_op_note_hash_exists,
                                     in.main_sel_op_emit_note_hash,
                                     in.main_sel_op_nullifier_exists,
                                     in.main_sel_op_emit_nullifier,
                                     in.main_sel_op_l1_to_l2_msg_exists,
                                     in.main_sel_op_emit_unencrypted_log,
                                     in.main_sel_op_emit_l2_to_l1_msg,
                                     in.main_sel_op_get_contract_instance,
                                     in.main_sel_op_sload,
                                     in.main_sel_op_sstore,
                                     in.main_sel_op_radix_le,
                                     in.main_sel_op_sha256,
                                     in.main_sel_op_poseidon2,
                                     in.main_sel_op_keccak,
                                     in.main_sel_op_pedersen,
                                     in.main_sel_op_add,
                                     in.main_sel_op_sub,
                                     in.main_sel_op_mul,
                                     in.main_sel_op_div,
                                     in.main_sel_op_fdiv,
                                     in.main_sel_op_not,
                                     in.main_sel_op_eq,
                                     in.main_sel_op_and,
                                     in.main_sel_op_or,
                                     in.main_sel_op_xor,
                                     in.main_sel_op_cast,
                                     in.main_sel_op_lt,
                                     in.main_sel_op_lte,
                                     in.main_sel_op_shl,
                                     in.main_sel_op_shr,
                                     in.main_sel_op_internal_call,
                                     in.main_sel_op_internal_return,
                                     in.main_sel_op_jump,
                                     in.main_sel_op_jumpi,
                                     in.main_sel_op_halt,
                                     in.main_sel_op_external_call,
                                     in.main_sel_op_mov,
                                     in.main_sel_op_cmov,
                                     in.main_sel_mem_op_a,
                                     in.main_sel_mem_op_b,
                                     in.main_sel_mem_op_c,
                                     in.main_sel_mem_op_d,
                                     in.main_clk,
                                     in.instr_spec_sel_op_sender,
                                     in.instr_spec_sel_op_address,
                                     in.instr_spec_sel_op_storage_address,
                                     in.instr_spec_sel_op_chain_id,
                                     in.instr_spec_sel_op_version,
                                     in.instr_spec_sel_op_block_number,
                                     in.instr_spec_sel_op_coinbase,
                                     in.instr_spec_sel_op_timestamp,
                                     in.instr_spec_sel_op_fee_per_l2_gas,
                                     in.instr_spec_sel_op_fee_per_da_gas,
                                     in.instr_spec_sel_op_transaction_fee,
                                     in.instr_spec_sel_op_l2gasleft,
                                     in.instr_spec_sel_op_dagasleft,
                                     in.instr_spec_sel_op_note_hash_exists,
                                     in.instr_spec_sel_op_emit_note_hash,
                                     in.instr_spec_sel_op_nullifier_exists,
                                     in.instr_spec_sel_op_emit_nullifier,
                                     in.instr_spec_sel_op_l1_to_l2_msg_exists,
                                     in.instr_spec_sel_op_emit_unencrypted_log,
                                     in.instr_spec_sel_op_emit_l2_to_l1_msg,
                                     in.instr_spec_sel_op_get_contract_instance,
                                     in.instr_spec_sel_op_sload,
                                     in.instr_spec_sel_op_sstore,
                                     in.instr_spec_sel_op_radix_le,
                                     in.instr_spec_sel_op_sha256,
                                     in.instr_spec_sel_op_poseidon2,
                                     in.instr_spec_sel_op_keccak,
                                     in.instr_spec_sel_op_pedersen,
                                     in.instr_spec_sel_op_add,
                                     in.instr_spec_sel_op_sub,
                                     in.instr_spec_sel_op_mul,
                                     in.instr_spec_sel_op_div,
                                     in.instr_spec_sel_op_fdiv,
                                     in.instr_spec_sel_op_not,
                                     in.instr_spec_sel_op_eq,
                                     in.instr_spec_sel_op_and,
                                     in.instr_spec_sel_op_or,
                                     in.instr_spec_sel_op_xor,
                                     in.instr_spec_sel_op_cast,
                                     in.instr_spec_sel_op_lt,
                                     in.instr_spec_sel_op_lte,
                                     in.instr_spec_sel_op_shl,
                                     in.instr_spec_sel_op_shr,
                                     in.instr_spec_sel_op_internal_call,
                                     in.instr_spec_sel_op_internal_return,
                                     in.instr_spec_sel_op_jump,
                                     in.instr_spec_sel_op_jumpi,
                                     in.instr_spec_sel_op_halt,
                                     in.instr_spec_sel_op_external_call,
                                     in.instr_spec_sel_op_mov,
                                     in.instr_spec_sel_op_cmov,
                                     in.instr_spec_sel_mem_op_a,
                                     in.instr_spec_sel_mem_op_b,
                                     in.instr_spec_sel_mem_op_c,
                                     in.instr_spec_sel_mem_op_d);
    }
};

template <typename FF_>
using lookup_instruction_spec_relation = GenericLookupRelation<lookup_instruction_spec_lookup_settings, FF_>;
template <typename FF_> using lookup_instruction_spec = GenericLookup<lookup_instruction_spec_lookup_settings, FF_>;

} // namespace bb
