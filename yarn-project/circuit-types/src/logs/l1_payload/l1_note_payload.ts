import { AztecAddress, type GrumpkinPrivateKey, type KeyValidationRequest, type PublicKey } from '@aztec/circuits.js';
import { randomBytes } from '@aztec/foundation/crypto';
import { Fr } from '@aztec/foundation/fields';
import { BufferReader, serializeToBuffer } from '@aztec/foundation/serialize';

import { EncryptedNoteLogIncomingBody } from './encrypted_log_incoming_body/index.js';
import { L1Payload } from './l1_payload.js';
import { Note } from './payload.js';

// Note type id can occupy only 4 bytes. The rest is 0 and is used to determine whether a note was successfully
// decrypted in tagged_log.ts
const NUM_BYTES_PER_NOTE_TYPE_ID = 4;

function isNoteTypeIdValid(noteTypeId: Fr): boolean {
  const buf = noteTypeId.toBuffer();
  // check that the first 28 bytes are zero
  return !buf.subarray(0, Fr.SIZE_IN_BYTES - NUM_BYTES_PER_NOTE_TYPE_ID).some(x => x !== 0);
}

/**
 * A class which wraps note data which is pushed on L1.
 * @remarks This data is required to compute a nullifier/to spend a note. Along with that this class contains
 * the necessary functionality to encrypt and decrypt the data.
 */
export class L1NotePayload extends L1Payload {
  constructor(
    /**
     * A note as emitted from Noir contract. Can be used along with private key to compute nullifier.
     */
    public note: Note,
    /**
     * Address of the contract this tx is interacting with.
     */
    public contractAddress: AztecAddress,
    /**
     * Storage slot of the underlying note.
     */
    public storageSlot: Fr,
    /**
     * Type identifier for the underlying note, required to determine how to compute its hash and nullifier.
     */
    public noteTypeId: Fr,
  ) {
    super();
    if (!isNoteTypeIdValid(noteTypeId)) {
      throw new Error(`NoteTypeId should occupy only ${NUM_BYTES_PER_NOTE_TYPE_ID} bytes`);
    }
  }

  /**
   * Deserializes the L1NotePayload object from a Buffer.
   * @param buffer - Buffer or BufferReader object to deserialize.
   * @returns An instance of L1NotePayload.
   */
  static fromBuffer(buffer: Buffer | BufferReader): L1NotePayload {
    const reader = BufferReader.asReader(buffer);
    return new L1NotePayload(
      reader.readObject(Note),
      reader.readObject(AztecAddress),
      Fr.fromBuffer(reader),
      Fr.fromBuffer(reader),
    );
  }

  /**
   * Serializes the L1NotePayload object into a Buffer.
   * @returns Buffer representation of the L1NotePayload object.
   */
  toBuffer() {
    return serializeToBuffer([this.note, this.contractAddress, this.storageSlot, this.noteTypeId]);
  }

  /**
   * Create a random L1NotePayload object (useful for testing purposes).
   * @param contract - The address of a contract the note was emitted from.
   * @returns A random L1NotePayload object.
   */
  static random(contract = AztecAddress.random()) {
    const noteTypeId = Fr.fromBuffer(
      Buffer.concat([
        Buffer.alloc(Fr.SIZE_IN_BYTES - NUM_BYTES_PER_NOTE_TYPE_ID),
        randomBytes(NUM_BYTES_PER_NOTE_TYPE_ID),
      ]),
    );
    return new L1NotePayload(Note.random(), contract, Fr.random(), noteTypeId);
  }

  public encrypt(ephSk: GrumpkinPrivateKey, recipient: AztecAddress, ivpk: PublicKey, ovKeys: KeyValidationRequest) {
    return super._encrypt(
      this.contractAddress,
      ephSk,
      recipient,
      ivpk,
      ovKeys,
      new EncryptedNoteLogIncomingBody(this.storageSlot, this.noteTypeId, this.note),
    );
  }

  /**
   * Decrypts a ciphertext as an incoming log.
   *
   * This is executable by the recipient of the note, and uses the ivsk to decrypt the payload.
   * The outgoing parts of the log are ignored entirely.
   *
   * Produces the same output as `decryptAsOutgoing`.
   *
   * @param ciphertext - The ciphertext for the log
   * @param ivsk - The incoming viewing secret key, used to decrypt the logs
   * @returns The decrypted log payload
   */
  public static decryptAsIncoming(ciphertext: Buffer | bigint[], ivsk: GrumpkinPrivateKey) {
    const input = Buffer.isBuffer(ciphertext) ? ciphertext : Buffer.from(ciphertext.map((x: bigint) => Number(x)));
    const reader = BufferReader.asReader(input);

    const [address, incomingBody] = super._decryptAsIncoming(
      reader.readToEnd(),
      ivsk,
      EncryptedNoteLogIncomingBody.fromCiphertext,
    );

    if (isNoteTypeIdValid(incomingBody.noteTypeId)) {
      // We received valid note type id, hence the encryption was performed correctly
      return new L1NotePayload(incomingBody.note, address, incomingBody.storageSlot, incomingBody.noteTypeId);
    }

    // We failed to decrypt the note, return undefined
    return undefined;
  }

  /**
   * Decrypts a ciphertext as an outgoing log.
   *
   * This is executable by the sender of the note, and uses the ovsk to decrypt the payload.
   * The outgoing parts are decrypted to retrieve information that allows the sender to
   * decrypt the incoming log, and learn about the note contents.
   *
   * Produces the same output as `decryptAsIncoming`.
   *
   * @param ciphertext - The ciphertext for the log
   * @param ovsk - The outgoing viewing secret key, used to decrypt the logs
   * @returns The decrypted log payload
   */
  public static decryptAsOutgoing(ciphertext: Buffer | bigint[], ovsk: GrumpkinPrivateKey) {
    const input = Buffer.isBuffer(ciphertext) ? ciphertext : Buffer.from(ciphertext.map((x: bigint) => Number(x)));
    const reader = BufferReader.asReader(input);

    const [address, incomingBody] = super._decryptAsOutgoing(
      reader.readToEnd(),
      ovsk,
      EncryptedNoteLogIncomingBody.fromCiphertext,
    );

    if (isNoteTypeIdValid(incomingBody.noteTypeId)) {
      // We received valid note type id, hence the encryption was performed correctly
      return new L1NotePayload(incomingBody.note, address, incomingBody.storageSlot, incomingBody.noteTypeId);
    }

    // We failed to decrypt the note, return undefined
    return undefined;
  }

  public equals(other: L1NotePayload) {
    return (
      this.note.equals(other.note) &&
      this.contractAddress.equals(other.contractAddress) &&
      this.storageSlot.equals(other.storageSlot) &&
      this.noteTypeId.equals(other.noteTypeId)
    );
  }
}
