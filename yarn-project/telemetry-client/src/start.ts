import { NoopTelemetryClient } from './noop.js';
import { OpenTelemetryClient } from './otel.js';
import { TelemetryClient } from './telemetry.js';

export interface TelemetryClientConfig {
  collectorHost?: string;
}

export function createAndStartTelemetryClient(
  config: TelemetryClientConfig,
  serviceName: string,
  serviceVersion?: string,
): TelemetryClient {
  if (config.collectorHost) {
    return OpenTelemetryClient.createAndStart(serviceName, serviceVersion ?? '0.0.0', config.collectorHost);
  } else {
    return new NoopTelemetryClient();
  }
}

export function getConfigEnvVars(): TelemetryClientConfig {
  const { OTEL_COLLECTOR_HOSTNAME } = process.env;

  return {
    collectorHost: OTEL_COLLECTOR_HOSTNAME,
  };
}
