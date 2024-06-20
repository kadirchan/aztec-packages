import {
  AttributeValue,
  MetricOptions,
  Gauge as OtelGauge,
  Histogram as OtelHistogram,
  UpDownCounter as OtelUpDownCounter,
  Span,
  SpanStatusCode,
  Tracer,
} from '@opentelemetry/api';

import * as Attributes from './attributes.js';
import * as Metrics from './metrics.js';

export { ValueType } from '@opentelemetry/api';

type ValuesOf<T> = T extends Record<string, infer U> ? U : never;

/** Global registry of attributes */
type Attributes = Partial<Record<ValuesOf<typeof Attributes>, AttributeValue>>;
export { Attributes };

/** Global registry of metrics */
type Metrics = (typeof Metrics)[keyof typeof Metrics];
export { Metrics };

export type Gauge = OtelGauge<Attributes>;
export type Histogram = OtelHistogram<Attributes>;
export type UpDownCounter = OtelUpDownCounter<Attributes>;

export { Tracer };

// INTERNAL NOTE: this interface is the same as opentelemetry's Meter, but with proper types
/**
 * A meter that provides instruments for recording metrics.
 */
export interface Meter {
  /**
   * Creates a new gauge instrument. A gauge is a metric that represents a single numerical value that can arbitrarily go up and down.
   * @param name - The name of the gauge
   * @param options - The options for the gauge
   */
  createGauge(name: Metrics, options?: MetricOptions): Gauge;

  /**
   * Creates a new histogram instrument. A histogram is a metric that samples observations (usually things like request durations or response sizes) and counts them in configurable buckets.
   * @param name - The name of the histogram
   * @param options - The options for the histogram
   */
  createHistogram(name: Metrics, options?: MetricOptions): Histogram;

  /**
   * Creates a new counter instrument. A counter can go up or down with a delta from the previous value.
   * @param name - The name of the counter
   * @param options - The options for the counter
   */
  createUpDownCounter(name: Metrics, options?: MetricOptions): UpDownCounter;
}

/**
 * A telemetry service that provides meters for recording metrics.
 */
export interface TelemetryClient {
  /**
   * Creates a new meter
   * @param name - The name of the meter.
   */
  getMeter(name: string): Meter;

  /**
   * Creates a new tracer
   * @param name - The name of the tracer.
   */
  getTracer(name: string): Tracer;

  /**
   * Stops the telemetry client.
   */
  stop(): Promise<void>;
}

interface Traceable {
  tracer: Tracer;
}

type SpanDecorator<T extends Traceable, F extends (...args: any[]) => any> = (
  originalMethod: F,
  context: ClassMethodDecoratorContext<T>,
) => F;

export function trackSpan<T extends Traceable, F extends (...args: any[]) => any>(
  spanName: string | ((this: T, ...args: Parameters<F>) => string),
  attributes?: Attributes | ((this: T, ...args: Parameters<F>) => Attributes),
  extraAttributes?: (this: T, returnValue: Awaited<ReturnType<F>>) => Attributes,
): SpanDecorator<T, F> {
  return (originalMethod: F, context: ClassMethodDecoratorContext<T>) => {
    return async function replacementMethod(this: T, ...args: Parameters<F>): Promise<Awaited<ReturnType<F>>> {
      const name = typeof spanName === 'function' ? spanName.call(this, ...args) : spanName;
      const currentAttrs = typeof attributes === 'function' ? attributes.call(this, ...args) : attributes;
      return this.tracer.startActiveSpan(name, async (span: Span) => {
        for (const [key, value] of Object.entries(currentAttrs ?? {})) {
          span.setAttribute(key, value);
        }

        try {
          const res = await originalMethod.call(this, ...args);
          const extraAttrs = extraAttributes?.call(this, res);
          for (const [key, value] of Object.entries(extraAttrs ?? {})) {
            span.setAttribute(key, value);
          }

          return res;
        } catch (err) {
          span.setStatus({
            code: SpanStatusCode.ERROR,
            message: String(err),
          });
        } finally {
          span.end();
        }
      });
    } as F;
  };
}
