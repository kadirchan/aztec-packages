import { type Tx } from '@aztec/circuit-types';
import { type Histogram, Metrics, type TelemetryClient, type UpDownCounter } from '@aztec/telemetry-client';

export class TxPoolMetrics {
  private txInMempool: UpDownCounter;
  private txSize: Histogram;

  constructor(telemetry: TelemetryClient) {
    const meter = telemetry.getMeter('tx_mempool');
    this.txInMempool = meter.createUpDownCounter(Metrics.MEMPOOL_TX_COUNT, {
      description: 'The current number of transactions in the mempool',
    });

    this.txSize = meter.createHistogram(Metrics.MEMPOOL_TX_SIZE, {
      unit: 'By',
      description: 'The size of transactions in the mempool',
      advice: {
        explicitBucketBoundaries: [
          5_000, // 5KB
          10_000,
          20_000,
          50_000,
          75_000,
          100_000, // 100KB
          200_000,
        ],
      },
    });
  }

  public recordTxs(txs: Tx[]) {
    for (const tx of txs) {
      this.txSize.record(tx.getSize());
    }

    this.txInMempool.add(txs.length);
  }

  public removeTxs(count = 1) {
    if (count < 0) {
      throw new Error('Count must be positive');
    }
    this.txInMempool.add(-1 * count);
  }
}
