import { Meter } from '@opentelemetry/api';
import { OTLPMetricExporter } from '@opentelemetry/exporter-metrics-otlp-http';
import { HostMetrics } from '@opentelemetry/host-metrics';
import { Resource } from '@opentelemetry/resources';
import { MeterProvider, PeriodicExportingMetricReader } from '@opentelemetry/sdk-metrics';
import { SEMRESATTRS_SERVICE_NAME, SEMRESATTRS_SERVICE_VERSION } from '@opentelemetry/semantic-conventions';

import { TelemetryClient } from './telemetry.js';

export class OpenTelemetryClient implements TelemetryClient {
  hostMetrics: HostMetrics | undefined;
  protected constructor(private resource: Resource, private meterProvider: MeterProvider) {}

  getMeter(name: string): Meter {
    return this.meterProvider.getMeter(name, this.resource.attributes[SEMRESATTRS_SERVICE_VERSION] as string);
  }

  public start() {
    this.hostMetrics = new HostMetrics({
      name: this.resource.attributes[SEMRESATTRS_SERVICE_NAME] as string,
      meterProvider: this.meterProvider,
    });

    this.hostMetrics.start();
  }

  public async stop() {
    await Promise.all([this.meterProvider.shutdown()]);
  }

  public static createAndStart(name: string, version: string, collectorHostname: string): OpenTelemetryClient {
    const resource = new Resource({
      [SEMRESATTRS_SERVICE_NAME]: name,
      [SEMRESATTRS_SERVICE_VERSION]: version,
    });

    const meterProvider = new MeterProvider({
      resource,
      readers: [
        new PeriodicExportingMetricReader({
          exporter: new OTLPMetricExporter({
            hostname: collectorHostname,
          }),
        }),
      ],
    });

    const service = new OpenTelemetryClient(resource, meterProvider);
    service.start();

    return service;
  }
}
