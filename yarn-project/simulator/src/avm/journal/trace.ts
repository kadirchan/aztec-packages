import { AvmContractInstanceHint, AvmExecutionHints, AvmExternalCallHint, AvmKeyValueHint, AztecAddress, ContractStorageRead, ContractStorageUpdateRequest, EthAddress, Gas, L2ToL1Message, LogHash, NoteHash, Nullifier, ReadRequest } from '@aztec/circuits.js';
import { UnencryptedFunctionL2Logs, UnencryptedL2Log } from '@aztec/circuit-types';
import { ContractInstanceWithAddress } from '@aztec/types/contracts';
import { EventSelector } from '@aztec/foundation/abi';
import { Fr } from '@aztec/foundation/fields';
import { type FieldsOf } from '@aztec/foundation/types';

import {
  type PublicExecution,
  type PublicExecutionResult,
} from '../../public/execution.js';
import {
  type TracedContractInstance,
} from './trace_types.js';
import { AvmRevertReason } from '../errors.js';
import { createSimulationError } from '../../common/errors.js';

export type TraceNestedCallArgs = [
  /** The execution that triggered this result. */
  /*execution:*/PublicExecution,
  /** How much gas was available for this public execution. */
  /*startGasLeft:*/Gas,
  /** How much gas was left after this public execution. */
  /*endGasLeft:*/Gas,
  /** Transaction fee set for this tx. */
  /*transactionFee:*/Fr,
  /** Bytecode used for this execution. */
  /*bytecode:*/Buffer,
  /** Calldata used for this execution. */
  /*calldata:*/Fr[],
  /** The return values of the function. */
  /*returnValues:*/Fr[],
  /** Whether the execution reverted. */
  /*reverted:*/boolean,
  /** The revert reason if the execution reverted. */
  /*revertReason:*/AvmRevertReason | undefined,
];

export class AvmSideEffectTrace {
  /** The side effect counter at the start of the function call. */
  public readonly startSideEffectCounter: number;
  /** The side effect counter increments with every call to the trace. */
  public sideEffectCounter: number; // kept as number until finalized for efficiency

  public contractStorageReads: ContractStorageRead[] = [];
  public contractStorageUpdateRequests: ContractStorageUpdateRequest[] = [];

  public noteHashReadRequests: ReadRequest[] = [];
  public newNoteHashes: NoteHash[] = [];

  public nullifierReadRequests: ReadRequest[] = [];
  public nullifierNonExistentReadRequests: ReadRequest[] = [];
  public newNullifiers: Nullifier[] = [];

  public l1ToL2MsgReadRequests: ReadRequest[] = [];
  public newL2ToL1Messages: L2ToL1Message[] = [];

  public unencryptedLogs: UnencryptedL2Log[] = [];
  public allUnencryptedLogs: UnencryptedL2Log[] = [];
  public unencryptedLogsHashes: LogHash[] = [];

  public gotContractInstances: ContractInstanceWithAddress[] = [];

  public nestedExecutions: PublicExecutionResult[] = [];

  public avmCircuitHints: AvmExecutionHints;

  constructor(parentTrace?: AvmSideEffectTrace) {
    const startSideEffectCounter = parentTrace ? parentTrace.sideEffectCounter : 0;
    this.sideEffectCounter = startSideEffectCounter;
    this.startSideEffectCounter = startSideEffectCounter;
    // TODO(4805): consider tracking the parent's trace vector lengths so we can enforce limits
    this.avmCircuitHints = AvmExecutionHints.empty();
  }

  public getCounter() {
    return this.sideEffectCounter;
  }

  public tracePublicStorageRead(storageAddress: Fr, slot: Fr, value: Fr, _exists: boolean, _cached: boolean) {
    // TODO(4805): check if some threshold is reached for max storage reads
    // (need access to parent length, or trace needs to be initialized with parent's contents)
    // NOTE: exists and cached are unused for now but may be used for optimizations or kernel hints later
    this.contractStorageReads.push(
      new ContractStorageRead(slot, value, this.sideEffectCounter, AztecAddress.fromField(storageAddress))
    );
    this.avmCircuitHints.storageValues.items.push(new AvmKeyValueHint(/*key=*/new Fr(this.sideEffectCounter), /*value=*/value));
    this.incrementSideEffectCounter();
  }

  public tracePublicStorageWrite(storageAddress: Fr, slot: Fr, value: Fr) {
    // TODO(4805): check if some threshold is reached for max storage writes
    // (need access to parent length, or trace needs to be initialized with parent's contents)
    this.contractStorageUpdateRequests.push(
      new ContractStorageUpdateRequest(slot, value, this.sideEffectCounter, storageAddress)
    );
    this.incrementSideEffectCounter();
  }

  public traceNoteHashCheck(_storageAddress: Fr, noteHash: Fr, exists: boolean, _leafIndex: Fr) {
    // TODO(4805): check if some threshold is reached for max note hash checks
    // NOTE: storageAddress is unused but will be important when an AVM circuit processes an entire enqueued call
    // TODO(dbanks12): leafIndex is unused for now but later must be used by kernel to constrain that the kernel
    // is in fact checking the leaf indicated by the user
    this.noteHashReadRequests.push(
      new ReadRequest(noteHash, this.sideEffectCounter)
    );
    this.avmCircuitHints.noteHashExists.items.push(new AvmKeyValueHint(/*key=*/new Fr(this.sideEffectCounter), /*value=*/new Fr(exists ? 1 : 0)));
    this.incrementSideEffectCounter();
  }

  public traceNewNoteHash(_storageAddress: Fr, noteHash: Fr) {
    // TODO(4805): check if some threshold is reached for max new note hash
    // NOTE: storageAddress is unused but will be important when an AVM circuit processes an entire enqueued call
    // TODO(dbanks12): non-existent note hashes should emit a read request of the note hash that actually
    // IS there, and the AVM circuit should accept THAT noteHash as a hint. The circuit will then compare
    // the noteHash against the one provided by the user code to determine what to return to the user (exists or not),
    // and will then propagate the actually-present noteHash to its public inputs.
    this.newNoteHashes.push(new NoteHash(noteHash, this.sideEffectCounter));
    this.incrementSideEffectCounter();
  }

  public traceNullifierCheck(_storageAddress: Fr, nullifier: Fr, exists: boolean, _isPending: boolean, _leafIndex: Fr) {
    // TODO(4805): check if some threshold is reached for max new nullifier
    // NOTE: storageAddress is unused but will be important when an AVM circuit processes an entire enqueued call
    // NOTE: isPending and leafIndex are unused for now but may be used for optimizations or kernel hints later
    const readRequest = new ReadRequest(nullifier, this.sideEffectCounter);
    if (exists) {
      this.nullifierReadRequests.push(readRequest);
    } else {
      this.nullifierNonExistentReadRequests.push(readRequest);
    }
    this.avmCircuitHints.nullifierExists.items.push(new AvmKeyValueHint(/*key=*/new Fr(this.sideEffectCounter), /*value=*/new Fr(exists ? 1 : 0)));
    this.incrementSideEffectCounter();
  }

  public traceNewNullifier(_storageAddress: Fr, nullifier: Fr) {
    // TODO(4805): check if some threshold is reached for max new nullifier
    // NOTE: storageAddress is unused but will be important when an AVM circuit processes an entire enqueued call
    this.newNullifiers.push(
      new Nullifier(nullifier, this.sideEffectCounter, /*noteHash=*/ Fr.ZERO),
    );
    this.incrementSideEffectCounter();
  }

  public traceL1ToL2MessageCheck(msgHash: Fr, _msgLeafIndex: Fr, exists: boolean) {
    // TODO(4805): check if some threshold is reached for max message reads
    // TODO(dbanks12): leafIndex is unused for now but later must be used by kernel to constrain that the kernel
    // is in fact checking the leaf indicated by the user
    this.l1ToL2MsgReadRequests.push(
      new ReadRequest(msgHash, this.sideEffectCounter)
    );
    this.avmCircuitHints.l1ToL2MessageExists.items.push(new AvmKeyValueHint(/*key=*/new Fr(this.sideEffectCounter), /*value=*/new Fr(exists ? 1 : 0)));
    this.incrementSideEffectCounter();
  }

  public traceNewL2ToL1Message(recipient: Fr, content: Fr) {
    const recipientAddress = EthAddress.fromField(recipient);
    this.newL2ToL1Messages.push(new L2ToL1Message(recipientAddress, content, this.sideEffectCounter));
    this.incrementSideEffectCounter();
  }

  public traceUnencryptedLog(contractAddress: Fr, event: Fr, log: Fr[]) {
    const ulog = new UnencryptedL2Log(
      AztecAddress.fromField(contractAddress),
      EventSelector.fromField(event),
      Buffer.concat(log.map(f => f.toBuffer())),
    );
    const basicLogHash = Fr.fromBuffer(ulog.hash());
    this.unencryptedLogs.push(ulog);
    this.allUnencryptedLogs.push(ulog);
    // TODO(6578): explain magic number 4 here
    this.unencryptedLogsHashes.push(new LogHash(basicLogHash, this.sideEffectCounter, new Fr(ulog.length + 4)));
    this.incrementSideEffectCounter();
  }

  public traceGetContractInstance(instance: TracedContractInstance) {
    this.gotContractInstances.push(instance);
    this.avmCircuitHints.contractInstances.items.push(
      new AvmContractInstanceHint(
        instance.address,
        new Fr(instance.exists ? 1 : 0),
        instance.salt,
        instance.deployer,
        instance.contractClassId,
        instance.initializationHash,
        instance.publicKeysHash,
      )
    );
    this.incrementSideEffectCounter();
  }

  private incrementSideEffectCounter() {
    this.sideEffectCounter++;
  }

  /**
   * Accept some results from a finished child's trace
   * @param incomingTrace - the incoming trace to process
   */
  public processFinishedChildTrace(incomingTrace: AvmSideEffectTrace) {
    // it is assumed that the incoming trace was initialized with this as parent, so accept counter
    this.sideEffectCounter = incomingTrace.sideEffectCounter;
    this.allUnencryptedLogs.push(...incomingTrace.allUnencryptedLogs);
    // NOTE: eventually if the AVM circuit processes an entire enqueued call,
    // this function will accept all of the child trace's side effects
  }

  /**
   * Trace a nested call.
   * Accept some results from a finished nested call's trace into this one.
   */
  public traceNestedCall(
    /** The trace of the nested call. */
    nestedCallTrace: AvmSideEffectTrace,
    ...args: TraceNestedCallArgs
  ) {
    const result = nestedCallTrace.toExecutionResult(...args);
    this.sideEffectCounter = result.endSideEffectCounter.toNumber();
    // when a nested call returns, caller accepts its updated counter
    this.allUnencryptedLogs.push(...result.allUnencryptedLogs.logs);
    // NOTE: eventually if the AVM circuit processes an entire enqueued call,
    // this function will accept all of the nested's side effects into this instance
    this.nestedExecutions.push(result);

    const gasUsed = new Gas(
      result.startGasLeft.daGas - result.endGasLeft.daGas,
      result.startGasLeft.l2Gas - result.endGasLeft.l2Gas,
    );
     this.avmCircuitHints.externalCalls.items.push(new AvmExternalCallHint(/*success=*/ new Fr(result.reverted ? 0 : 1), result.returnValues, gasUsed));

  }

  /**
   * Convert this trace to a PublicExecutionResult for use externally to the simulator.
   */
  public toExecutionResult(
    /** The execution that triggered this result. */
    execution: PublicExecution,
    /** How much gas was available for this public execution. */
    startGasLeft: Gas,
    /** How much gas was left after this public execution. */
    endGasLeft: Gas,
    /** Transaction fee set for this tx. */
    transactionFee: Fr,
    /** Bytecode used for this execution. */
    bytecode: Buffer,
    /** Calldata used for this execution. */
    calldata: Fr[],
    /** The return values of the function. */
    returnValues: Fr[],
    /** Whether the execution reverted. */
    reverted: boolean,
    /** The revert reason if the execution reverted. */
    revertReason?: AvmRevertReason,
  ): PublicExecutionResult {
    return {
      ...(this as FieldsOf<AvmSideEffectTrace>),
      execution,

      startSideEffectCounter: new Fr(this.startSideEffectCounter),
      endSideEffectCounter: new Fr(this.sideEffectCounter),
      startGasLeft,
      endGasLeft,
      transactionFee,

      bytecode,
      calldata,
      returnValues,
      reverted,
      revertReason: revertReason ? createSimulationError(revertReason) : undefined,

      // correct the type on these now that they are finalized (lists won't grow)
      unencryptedLogs: new UnencryptedFunctionL2Logs(this.unencryptedLogs),
      allUnencryptedLogs: new UnencryptedFunctionL2Logs(this.allUnencryptedLogs),
    };
  }
}
