use crate::context::PrivateContext;
use crate::note::note_header::NoteHeader;
use dep::protocol_types::{grumpkin_point::GrumpkinPoint, abis::function_selector::FunctionSelector};

trait EventInterface<N> {
    // Should be autogenerated by the #[aztec(event)] macro unless it is overridden by a custom implementation
    fn _selector(self) -> FunctionSelector;
    fn to_be_bytes(self, randomness: Field) -> [u8; N];
}
