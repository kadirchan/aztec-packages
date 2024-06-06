// #[oracle(storageRead)]
// fn storage_read_oracle<N>(_storage_slot: Field, _number_of_elements: Field) -> [Field; N] {}

// unconstrained fn storage_read_oracle_wrapper<N>(_storage_slot: Field) -> [Field; N] {
//     storage_read_oracle(_storage_slot, N)
// }

// pub fn storage_read<N>(storage_slot: Field) -> [Field; N] {
//     storage_read_oracle_wrapper(storage_slot)
// }

// #[oracle(storageWrite)]
// fn storage_write_oracle<N>(_storage_slot: Field, _values: [Field; N]) -> [Field; N] {}

// unconstrained pub fn storage_write<N>(storage_slot: Field, fields: [Field; N]) {
//     let _hash = storage_write_oracle(storage_slot, fields);
// }

#[oracle(storageRead)]
fn storage_read_oracle<N>(_storage_slot: Field, _number_of_elements: Field) -> [Field; N] {}

unconstrained pub fn storage_read<N>(storage_slot: Field) -> [Field; N] {
    let mut fields = [0; N];
    for i in 0..N {
        let one_field: [Field; 1] = storage_read_oracle(storage_slot + i as Field, 1);
        fields[i] = one_field[0];
    }
    fields
}

#[oracle(storageWrite)]
fn storage_write_oracle<N>(_storage_slot: Field, _values: [Field; N]) -> [Field; N] {}

unconstrained pub fn storage_write<N>(storage_slot: Field, fields: [Field; N]) {
    for i in 0..N {
        let _unused_hash = storage_write_oracle(storage_slot + i as Field, [fields[i]]);
    }
}
