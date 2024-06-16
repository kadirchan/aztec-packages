import { type CircuitName } from '@aztec/circuit-types/stats';
import { type Timer } from '@aztec/foundation/timer';
import {
  Attributes,
  type Gauge,
  type Histogram,
  Metrics,
  type TelemetryClient,
  ValueType,
} from '@aztec/telemetry-client';

export class ProverInstrumentation {
  private simulationDuration: Histogram;
  private witGenDuration: Gauge;
  private provingDuration: Gauge;

  private witGenInputSize: Gauge;
  private witGenOutputSize: Gauge;

  private proofSize: Gauge;
  private circuitSize: Gauge;
  private circuitPublicInputCount: Gauge;

  constructor(telemetry: TelemetryClient, name: string = 'bb-prover') {
    const meter = telemetry.getMeter(name);
    const msBuckets = [100, 250, 500, 1_000, 2_500, 5_000, 10_000, 30_000, 60_000, 90_000];
    this.simulationDuration = meter.createHistogram(Metrics.CIRCUIT_SIMULATION_DURATION, {
      description: 'Records how long it takes to simulate a circuit',
      unit: 'ms',
      valueType: ValueType.INT,
      advice: {
        explicitBucketBoundaries: msBuckets,
      },
    });

    this.witGenDuration = meter.createGauge(Metrics.CIRCUIT_WITNESS_GEN_DURATION, {
      description: 'Records how long it takes to generate the partial witness for a circuit',
      unit: 'ms',
      valueType: ValueType.INT,
    });

    // ideally this would be a histogram, but proving takes a long time on the server
    // and they don't happen that often so Prometheus & Grafana have a hard time handling it
    this.provingDuration = meter.createGauge(Metrics.CIRCUIT_PROVING_DURATION, {
      unit: 'ms',
      description: 'Records how long it takes to prove a circuit',
      valueType: ValueType.INT,
    });

    this.witGenInputSize = meter.createGauge(Metrics.CIRCUIT_WITNESS_GEN_INPUT_SIZE, {
      unit: 'By',
      description: 'Records the size of the input to the witness generation',
      valueType: ValueType.INT,
    });

    this.witGenOutputSize = meter.createGauge(Metrics.CIRCUIT_WITNESS_GEN_OUTPUT_SIZE, {
      unit: 'By',
      description: 'Records the size of the output of the witness generation',
      valueType: ValueType.INT,
    });

    this.proofSize = meter.createGauge(Metrics.CIRCUIT_PROVING_PROOF_SIZE, {
      unit: 'By',
      description: 'Records the size of the proof generated for a circuit',
      valueType: ValueType.INT,
    });

    this.circuitPublicInputCount = meter.createGauge(Metrics.CIRCUIT_PUBLIC_INPUTS_COUNT, {
      description: 'Records the number of public inputs in a circuit',
      valueType: ValueType.INT,
    });

    this.circuitSize = meter.createGauge(Metrics.CIRCUIT_SIZE, {
      description: 'Records the size of the circuit in gates',
      valueType: ValueType.INT,
    });
  }

  recordDuration(
    metric: 'simulationDuration' | 'witGenDuration' | 'provingDuration',
    circuitName: CircuitName,
    timerOrMs: Timer | number,
  ) {
    const ms = Math.ceil(typeof timerOrMs === 'number' ? timerOrMs : timerOrMs.ms());
    this[metric].record(ms, {
      [Attributes.PROTOCOL_CIRCUIT_NAME]: circuitName,
      [Attributes.PROTOCOL_CIRCUIT_TYPE]: 'server',
    });
  }

  recordAvmDuration(metric: 'witGenDuration' | 'provingDuration', appCircuitName: string, timerOrMs: Timer | number) {
    const ms = Math.ceil(typeof timerOrMs === 'number' ? timerOrMs : timerOrMs.ms());
    this[metric].record(ms, {
      [Attributes.APP_CIRCUIT_NAME]: appCircuitName,
    });
  }

  recordSize(
    metric: 'witGenInputSize' | 'witGenOutputSize' | 'proofSize' | 'circuitSize' | 'circuitPublicInputCount',
    circuitName: CircuitName,
    size: number,
  ) {
    this[metric].record(size, {
      [Attributes.PROTOCOL_CIRCUIT_NAME]: circuitName,
      [Attributes.PROTOCOL_CIRCUIT_TYPE]: 'server',
    });
  }

  recordAvmSize(
    metric: 'witGenInputSize' | 'witGenOutputSize' | 'proofSize' | 'circuitSize' | 'circuitPublicInputCount',
    appCircuitName: string,
    size: number,
  ) {
    this[metric].record(size, {
      [Attributes.APP_CIRCUIT_NAME]: appCircuitName,
    });
  }
}
