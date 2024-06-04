---
title: How to use ValueNote in your contract
---

This guide covers how to use [ValueNote] in your Aztec.nr contract. To learn more about notes in general, go to the [concepts](../../../../aztec/concepts/state_model/index.md#private-state).

`ValueNote` is a type of note for holding a `value` as a `Field` type. It comes with utils such as `increment` and `decrement` for working with integers.

#include_code value-note-def noir-projects/aztec-nr/value-note/src/value_note.nr rust

## Import ValueNote

You will need to import it into your Nargo.toml and contract file. 

### Nargo.toml

```toml
value_note = { git="https://github.com/AztecProtocol/aztec-packages/", tag="#include_aztec_version", directory="noir-projects/aztec-nr/value-note" }
```

### Contract

