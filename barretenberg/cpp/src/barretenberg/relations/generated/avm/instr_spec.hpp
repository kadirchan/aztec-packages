
#pragma once
#include "../../relation_parameters.hpp"
#include "../../relation_types.hpp"
#include "./declare_views.hpp"

namespace bb::Avm_vm {

template <typename FF> struct Instr_specRow {
    FF instr_spec_rwa{};
    FF instr_spec_rwb{};
    FF instr_spec_rwc{};
    FF instr_spec_rwd{};
    FF instr_spec_sel_mem_op_a{};
    FF instr_spec_sel_mem_op_b{};
    FF instr_spec_sel_mem_op_c{};
    FF instr_spec_sel_mem_op_d{};
    FF instr_spec_sel_mov_ia_to_ic{};
    FF instr_spec_sel_mov_ib_to_ic{};
    FF instr_spec_sel_op_add{};
    FF instr_spec_sel_op_address{};
    FF instr_spec_sel_op_and{};
    FF instr_spec_sel_op_block_number{};
    FF instr_spec_sel_op_cast{};
    FF instr_spec_sel_op_chain_id{};
    FF instr_spec_sel_op_cmov{};
    FF instr_spec_sel_op_coinbase{};
    FF instr_spec_sel_op_dagasleft{};
    FF instr_spec_sel_op_div{};
    FF instr_spec_sel_op_emit_l2_to_l1_msg{};
    FF instr_spec_sel_op_emit_note_hash{};
    FF instr_spec_sel_op_emit_nullifier{};
    FF instr_spec_sel_op_emit_unencrypted_log{};
    FF instr_spec_sel_op_eq{};
    FF instr_spec_sel_op_external_call{};
    FF instr_spec_sel_op_fdiv{};
    FF instr_spec_sel_op_fee_per_da_gas{};
    FF instr_spec_sel_op_fee_per_l2_gas{};
    FF instr_spec_sel_op_get_contract_instance{};
    FF instr_spec_sel_op_halt{};
    FF instr_spec_sel_op_internal_call{};
    FF instr_spec_sel_op_internal_return{};
    FF instr_spec_sel_op_jump{};
    FF instr_spec_sel_op_jumpi{};
    FF instr_spec_sel_op_keccak{};
    FF instr_spec_sel_op_l1_to_l2_msg_exists{};
    FF instr_spec_sel_op_l2gasleft{};
    FF instr_spec_sel_op_lt{};
    FF instr_spec_sel_op_lte{};
    FF instr_spec_sel_op_mov{};
    FF instr_spec_sel_op_mul{};
    FF instr_spec_sel_op_not{};
    FF instr_spec_sel_op_note_hash_exists{};
    FF instr_spec_sel_op_nullifier_exists{};
    FF instr_spec_sel_op_or{};
    FF instr_spec_sel_op_pedersen{};
    FF instr_spec_sel_op_poseidon2{};
    FF instr_spec_sel_op_radix_le{};
    FF instr_spec_sel_op_sender{};
    FF instr_spec_sel_op_sha256{};
    FF instr_spec_sel_op_shl{};
    FF instr_spec_sel_op_shr{};
    FF instr_spec_sel_op_sload{};
    FF instr_spec_sel_op_sstore{};
    FF instr_spec_sel_op_storage_address{};
    FF instr_spec_sel_op_sub{};
    FF instr_spec_sel_op_timestamp{};
    FF instr_spec_sel_op_transaction_fee{};
    FF instr_spec_sel_op_version{};
    FF instr_spec_sel_op_xor{};
    FF instr_spec_sel_resolve_ind_addr_a{};
    FF instr_spec_sel_resolve_ind_addr_b{};
    FF instr_spec_sel_resolve_ind_addr_c{};
    FF instr_spec_sel_resolve_ind_addr_d{};
    FF main_id_zero{};

    [[maybe_unused]] static std::vector<std::string> names();
};

inline std::string get_relation_label_instr_spec(int index)
{
    switch (index) {}
    return std::to_string(index);
}

template <typename FF_> class instr_specImpl {
  public:
    using FF = FF_;

    static constexpr std::array<size_t, 65> SUBRELATION_PARTIAL_LENGTHS{
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    };

    template <typename ContainerOverSubrelations, typename AllEntities>
    void static accumulate(ContainerOverSubrelations& evals,
                           const AllEntities& new_term,
                           [[maybe_unused]] const RelationParameters<FF>&,
                           [[maybe_unused]] const FF& scaling_factor)
    {

        // Contribution 0
        {
            Avm_DECLARE_VIEWS(0);

            auto tmp = (instr_spec_sel_op_sender * (-instr_spec_sel_op_sender + FF(1)));
            tmp *= scaling_factor;
            std::get<0>(evals) += tmp;
        }
        // Contribution 1
        {
            Avm_DECLARE_VIEWS(1);

            auto tmp = (instr_spec_sel_op_address * (-instr_spec_sel_op_address + FF(1)));
            tmp *= scaling_factor;
            std::get<1>(evals) += tmp;
        }
        // Contribution 2
        {
            Avm_DECLARE_VIEWS(2);

            auto tmp = (instr_spec_sel_op_storage_address * (-instr_spec_sel_op_storage_address + FF(1)));
            tmp *= scaling_factor;
            std::get<2>(evals) += tmp;
        }
        // Contribution 3
        {
            Avm_DECLARE_VIEWS(3);

            auto tmp = (instr_spec_sel_op_chain_id * (-instr_spec_sel_op_chain_id + FF(1)));
            tmp *= scaling_factor;
            std::get<3>(evals) += tmp;
        }
        // Contribution 4
        {
            Avm_DECLARE_VIEWS(4);

            auto tmp = (instr_spec_sel_op_version * (-instr_spec_sel_op_version + FF(1)));
            tmp *= scaling_factor;
            std::get<4>(evals) += tmp;
        }
        // Contribution 5
        {
            Avm_DECLARE_VIEWS(5);

            auto tmp = (instr_spec_sel_op_block_number * (-instr_spec_sel_op_block_number + FF(1)));
            tmp *= scaling_factor;
            std::get<5>(evals) += tmp;
        }
        // Contribution 6
        {
            Avm_DECLARE_VIEWS(6);

            auto tmp = (instr_spec_sel_op_coinbase * (-instr_spec_sel_op_coinbase + FF(1)));
            tmp *= scaling_factor;
            std::get<6>(evals) += tmp;
        }
        // Contribution 7
        {
            Avm_DECLARE_VIEWS(7);

            auto tmp = (instr_spec_sel_op_timestamp * (-instr_spec_sel_op_timestamp + FF(1)));
            tmp *= scaling_factor;
            std::get<7>(evals) += tmp;
        }
        // Contribution 8
        {
            Avm_DECLARE_VIEWS(8);

            auto tmp = (instr_spec_sel_op_fee_per_l2_gas * (-instr_spec_sel_op_fee_per_l2_gas + FF(1)));
            tmp *= scaling_factor;
            std::get<8>(evals) += tmp;
        }
        // Contribution 9
        {
            Avm_DECLARE_VIEWS(9);

            auto tmp = (instr_spec_sel_op_fee_per_da_gas * (-instr_spec_sel_op_fee_per_da_gas + FF(1)));
            tmp *= scaling_factor;
            std::get<9>(evals) += tmp;
        }
        // Contribution 10
        {
            Avm_DECLARE_VIEWS(10);

            auto tmp = (instr_spec_sel_op_transaction_fee * (-instr_spec_sel_op_transaction_fee + FF(1)));
            tmp *= scaling_factor;
            std::get<10>(evals) += tmp;
        }
        // Contribution 11
        {
            Avm_DECLARE_VIEWS(11);

            auto tmp = (instr_spec_sel_op_l2gasleft * (-instr_spec_sel_op_l2gasleft + FF(1)));
            tmp *= scaling_factor;
            std::get<11>(evals) += tmp;
        }
        // Contribution 12
        {
            Avm_DECLARE_VIEWS(12);

            auto tmp = (instr_spec_sel_op_dagasleft * (-instr_spec_sel_op_dagasleft + FF(1)));
            tmp *= scaling_factor;
            std::get<12>(evals) += tmp;
        }
        // Contribution 13
        {
            Avm_DECLARE_VIEWS(13);

            auto tmp = (instr_spec_sel_op_note_hash_exists * (-instr_spec_sel_op_note_hash_exists + FF(1)));
            tmp *= scaling_factor;
            std::get<13>(evals) += tmp;
        }
        // Contribution 14
        {
            Avm_DECLARE_VIEWS(14);

            auto tmp = (instr_spec_sel_op_emit_note_hash * (-instr_spec_sel_op_emit_note_hash + FF(1)));
            tmp *= scaling_factor;
            std::get<14>(evals) += tmp;
        }
        // Contribution 15
        {
            Avm_DECLARE_VIEWS(15);

            auto tmp = (instr_spec_sel_op_nullifier_exists * (-instr_spec_sel_op_nullifier_exists + FF(1)));
            tmp *= scaling_factor;
            std::get<15>(evals) += tmp;
        }
        // Contribution 16
        {
            Avm_DECLARE_VIEWS(16);

            auto tmp = (instr_spec_sel_op_emit_nullifier * (-instr_spec_sel_op_emit_nullifier + FF(1)));
            tmp *= scaling_factor;
            std::get<16>(evals) += tmp;
        }
        // Contribution 17
        {
            Avm_DECLARE_VIEWS(17);

            auto tmp = (instr_spec_sel_op_l1_to_l2_msg_exists * (-instr_spec_sel_op_l1_to_l2_msg_exists + FF(1)));
            tmp *= scaling_factor;
            std::get<17>(evals) += tmp;
        }
        // Contribution 18
        {
            Avm_DECLARE_VIEWS(18);

            auto tmp = (instr_spec_sel_op_emit_unencrypted_log * (-instr_spec_sel_op_emit_unencrypted_log + FF(1)));
            tmp *= scaling_factor;
            std::get<18>(evals) += tmp;
        }
        // Contribution 19
        {
            Avm_DECLARE_VIEWS(19);

            auto tmp = (instr_spec_sel_op_emit_l2_to_l1_msg * (-instr_spec_sel_op_emit_l2_to_l1_msg + FF(1)));
            tmp *= scaling_factor;
            std::get<19>(evals) += tmp;
        }
        // Contribution 20
        {
            Avm_DECLARE_VIEWS(20);

            auto tmp = (instr_spec_sel_op_get_contract_instance * (-instr_spec_sel_op_get_contract_instance + FF(1)));
            tmp *= scaling_factor;
            std::get<20>(evals) += tmp;
        }
        // Contribution 21
        {
            Avm_DECLARE_VIEWS(21);

            auto tmp = (instr_spec_sel_op_sload * (-instr_spec_sel_op_sload + FF(1)));
            tmp *= scaling_factor;
            std::get<21>(evals) += tmp;
        }
        // Contribution 22
        {
            Avm_DECLARE_VIEWS(22);

            auto tmp = (instr_spec_sel_op_sstore * (-instr_spec_sel_op_sstore + FF(1)));
            tmp *= scaling_factor;
            std::get<22>(evals) += tmp;
        }
        // Contribution 23
        {
            Avm_DECLARE_VIEWS(23);

            auto tmp = (instr_spec_sel_op_radix_le * (-instr_spec_sel_op_radix_le + FF(1)));
            tmp *= scaling_factor;
            std::get<23>(evals) += tmp;
        }
        // Contribution 24
        {
            Avm_DECLARE_VIEWS(24);

            auto tmp = (instr_spec_sel_op_sha256 * (-instr_spec_sel_op_sha256 + FF(1)));
            tmp *= scaling_factor;
            std::get<24>(evals) += tmp;
        }
        // Contribution 25
        {
            Avm_DECLARE_VIEWS(25);

            auto tmp = (instr_spec_sel_op_poseidon2 * (-instr_spec_sel_op_poseidon2 + FF(1)));
            tmp *= scaling_factor;
            std::get<25>(evals) += tmp;
        }
        // Contribution 26
        {
            Avm_DECLARE_VIEWS(26);

            auto tmp = (instr_spec_sel_op_keccak * (-instr_spec_sel_op_keccak + FF(1)));
            tmp *= scaling_factor;
            std::get<26>(evals) += tmp;
        }
        // Contribution 27
        {
            Avm_DECLARE_VIEWS(27);

            auto tmp = (instr_spec_sel_op_pedersen * (-instr_spec_sel_op_pedersen + FF(1)));
            tmp *= scaling_factor;
            std::get<27>(evals) += tmp;
        }
        // Contribution 28
        {
            Avm_DECLARE_VIEWS(28);

            auto tmp = (instr_spec_sel_op_add * (-instr_spec_sel_op_add + FF(1)));
            tmp *= scaling_factor;
            std::get<28>(evals) += tmp;
        }
        // Contribution 29
        {
            Avm_DECLARE_VIEWS(29);

            auto tmp = (instr_spec_sel_op_sub * (-instr_spec_sel_op_sub + FF(1)));
            tmp *= scaling_factor;
            std::get<29>(evals) += tmp;
        }
        // Contribution 30
        {
            Avm_DECLARE_VIEWS(30);

            auto tmp = (instr_spec_sel_op_mul * (-instr_spec_sel_op_mul + FF(1)));
            tmp *= scaling_factor;
            std::get<30>(evals) += tmp;
        }
        // Contribution 31
        {
            Avm_DECLARE_VIEWS(31);

            auto tmp = (instr_spec_sel_op_div * (-instr_spec_sel_op_div + FF(1)));
            tmp *= scaling_factor;
            std::get<31>(evals) += tmp;
        }
        // Contribution 32
        {
            Avm_DECLARE_VIEWS(32);

            auto tmp = (instr_spec_sel_op_fdiv * (-instr_spec_sel_op_fdiv + FF(1)));
            tmp *= scaling_factor;
            std::get<32>(evals) += tmp;
        }
        // Contribution 33
        {
            Avm_DECLARE_VIEWS(33);

            auto tmp = (instr_spec_sel_op_not * (-instr_spec_sel_op_not + FF(1)));
            tmp *= scaling_factor;
            std::get<33>(evals) += tmp;
        }
        // Contribution 34
        {
            Avm_DECLARE_VIEWS(34);

            auto tmp = (instr_spec_sel_op_eq * (-instr_spec_sel_op_eq + FF(1)));
            tmp *= scaling_factor;
            std::get<34>(evals) += tmp;
        }
        // Contribution 35
        {
            Avm_DECLARE_VIEWS(35);

            auto tmp = (instr_spec_sel_op_and * (-instr_spec_sel_op_and + FF(1)));
            tmp *= scaling_factor;
            std::get<35>(evals) += tmp;
        }
        // Contribution 36
        {
            Avm_DECLARE_VIEWS(36);

            auto tmp = (instr_spec_sel_op_or * (-instr_spec_sel_op_or + FF(1)));
            tmp *= scaling_factor;
            std::get<36>(evals) += tmp;
        }
        // Contribution 37
        {
            Avm_DECLARE_VIEWS(37);

            auto tmp = (instr_spec_sel_op_xor * (-instr_spec_sel_op_xor + FF(1)));
            tmp *= scaling_factor;
            std::get<37>(evals) += tmp;
        }
        // Contribution 38
        {
            Avm_DECLARE_VIEWS(38);

            auto tmp = (instr_spec_sel_op_cast * (-instr_spec_sel_op_cast + FF(1)));
            tmp *= scaling_factor;
            std::get<38>(evals) += tmp;
        }
        // Contribution 39
        {
            Avm_DECLARE_VIEWS(39);

            auto tmp = (instr_spec_sel_op_lt * (-instr_spec_sel_op_lt + FF(1)));
            tmp *= scaling_factor;
            std::get<39>(evals) += tmp;
        }
        // Contribution 40
        {
            Avm_DECLARE_VIEWS(40);

            auto tmp = (instr_spec_sel_op_lte * (-instr_spec_sel_op_lte + FF(1)));
            tmp *= scaling_factor;
            std::get<40>(evals) += tmp;
        }
        // Contribution 41
        {
            Avm_DECLARE_VIEWS(41);

            auto tmp = (instr_spec_sel_op_shl * (-instr_spec_sel_op_shl + FF(1)));
            tmp *= scaling_factor;
            std::get<41>(evals) += tmp;
        }
        // Contribution 42
        {
            Avm_DECLARE_VIEWS(42);

            auto tmp = (instr_spec_sel_op_shr * (-instr_spec_sel_op_shr + FF(1)));
            tmp *= scaling_factor;
            std::get<42>(evals) += tmp;
        }
        // Contribution 43
        {
            Avm_DECLARE_VIEWS(43);

            auto tmp = (instr_spec_sel_op_internal_call * (-instr_spec_sel_op_internal_call + FF(1)));
            tmp *= scaling_factor;
            std::get<43>(evals) += tmp;
        }
        // Contribution 44
        {
            Avm_DECLARE_VIEWS(44);

            auto tmp = (instr_spec_sel_op_internal_return * (-instr_spec_sel_op_internal_return + FF(1)));
            tmp *= scaling_factor;
            std::get<44>(evals) += tmp;
        }
        // Contribution 45
        {
            Avm_DECLARE_VIEWS(45);

            auto tmp = (instr_spec_sel_op_jump * (-instr_spec_sel_op_jump + FF(1)));
            tmp *= scaling_factor;
            std::get<45>(evals) += tmp;
        }
        // Contribution 46
        {
            Avm_DECLARE_VIEWS(46);

            auto tmp = (instr_spec_sel_op_jumpi * (-instr_spec_sel_op_jumpi + FF(1)));
            tmp *= scaling_factor;
            std::get<46>(evals) += tmp;
        }
        // Contribution 47
        {
            Avm_DECLARE_VIEWS(47);

            auto tmp = (instr_spec_sel_op_halt * (-instr_spec_sel_op_halt + FF(1)));
            tmp *= scaling_factor;
            std::get<47>(evals) += tmp;
        }
        // Contribution 48
        {
            Avm_DECLARE_VIEWS(48);

            auto tmp = (instr_spec_sel_op_external_call * (-instr_spec_sel_op_external_call + FF(1)));
            tmp *= scaling_factor;
            std::get<48>(evals) += tmp;
        }
        // Contribution 49
        {
            Avm_DECLARE_VIEWS(49);

            auto tmp = (instr_spec_sel_op_mov * (-instr_spec_sel_op_mov + FF(1)));
            tmp *= scaling_factor;
            std::get<49>(evals) += tmp;
        }
        // Contribution 50
        {
            Avm_DECLARE_VIEWS(50);

            auto tmp = (instr_spec_sel_op_cmov * (-instr_spec_sel_op_cmov + FF(1)));
            tmp *= scaling_factor;
            std::get<50>(evals) += tmp;
        }
        // Contribution 51
        {
            Avm_DECLARE_VIEWS(51);

            auto tmp = (instr_spec_sel_mem_op_a * (-instr_spec_sel_mem_op_a + FF(1)));
            tmp *= scaling_factor;
            std::get<51>(evals) += tmp;
        }
        // Contribution 52
        {
            Avm_DECLARE_VIEWS(52);

            auto tmp = (instr_spec_sel_mem_op_b * (-instr_spec_sel_mem_op_b + FF(1)));
            tmp *= scaling_factor;
            std::get<52>(evals) += tmp;
        }
        // Contribution 53
        {
            Avm_DECLARE_VIEWS(53);

            auto tmp = (instr_spec_sel_mem_op_c * (-instr_spec_sel_mem_op_c + FF(1)));
            tmp *= scaling_factor;
            std::get<53>(evals) += tmp;
        }
        // Contribution 54
        {
            Avm_DECLARE_VIEWS(54);

            auto tmp = (instr_spec_sel_mem_op_d * (-instr_spec_sel_mem_op_d + FF(1)));
            tmp *= scaling_factor;
            std::get<54>(evals) += tmp;
        }
        // Contribution 55
        {
            Avm_DECLARE_VIEWS(55);

            auto tmp = (instr_spec_sel_resolve_ind_addr_a * (-instr_spec_sel_resolve_ind_addr_a + FF(1)));
            tmp *= scaling_factor;
            std::get<55>(evals) += tmp;
        }
        // Contribution 56
        {
            Avm_DECLARE_VIEWS(56);

            auto tmp = (instr_spec_sel_resolve_ind_addr_b * (-instr_spec_sel_resolve_ind_addr_b + FF(1)));
            tmp *= scaling_factor;
            std::get<56>(evals) += tmp;
        }
        // Contribution 57
        {
            Avm_DECLARE_VIEWS(57);

            auto tmp = (instr_spec_sel_resolve_ind_addr_c * (-instr_spec_sel_resolve_ind_addr_c + FF(1)));
            tmp *= scaling_factor;
            std::get<57>(evals) += tmp;
        }
        // Contribution 58
        {
            Avm_DECLARE_VIEWS(58);

            auto tmp = (instr_spec_sel_resolve_ind_addr_d * (-instr_spec_sel_resolve_ind_addr_d + FF(1)));
            tmp *= scaling_factor;
            std::get<58>(evals) += tmp;
        }
        // Contribution 59
        {
            Avm_DECLARE_VIEWS(59);

            auto tmp = (instr_spec_rwa * (-instr_spec_rwa + FF(1)));
            tmp *= scaling_factor;
            std::get<59>(evals) += tmp;
        }
        // Contribution 60
        {
            Avm_DECLARE_VIEWS(60);

            auto tmp = (instr_spec_rwb * (-instr_spec_rwb + FF(1)));
            tmp *= scaling_factor;
            std::get<60>(evals) += tmp;
        }
        // Contribution 61
        {
            Avm_DECLARE_VIEWS(61);

            auto tmp = (instr_spec_rwc * (-instr_spec_rwc + FF(1)));
            tmp *= scaling_factor;
            std::get<61>(evals) += tmp;
        }
        // Contribution 62
        {
            Avm_DECLARE_VIEWS(62);

            auto tmp = (instr_spec_rwd * (-instr_spec_rwd + FF(1)));
            tmp *= scaling_factor;
            std::get<62>(evals) += tmp;
        }
        // Contribution 63
        {
            Avm_DECLARE_VIEWS(63);

            auto tmp = (instr_spec_sel_mov_ia_to_ic -
                        (instr_spec_sel_op_mov + (instr_spec_sel_op_cmov * (-main_id_zero + FF(1)))));
            tmp *= scaling_factor;
            std::get<63>(evals) += tmp;
        }
        // Contribution 64
        {
            Avm_DECLARE_VIEWS(64);

            auto tmp = (instr_spec_sel_mov_ib_to_ic - (instr_spec_sel_op_cmov * main_id_zero));
            tmp *= scaling_factor;
            std::get<64>(evals) += tmp;
        }
    }
};

template <typename FF> using instr_spec = Relation<instr_specImpl<FF>>;

} // namespace bb::Avm_vm