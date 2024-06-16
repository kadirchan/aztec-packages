import { Meter, createNoopMeter } from '@opentelemetry/api';

import { TelemetryClient } from './telemetry.js';

export class NoopTelemetryClient implements TelemetryClient {
  getMeter(): Meter {
    return createNoopMeter();
  }

  stop(): Promise<void> {
    return Promise.resolve();
  }
}
