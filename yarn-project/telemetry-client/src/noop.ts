import {
  AttributeValue,
  Attributes,
  Context,
  Exception,
  Link,
  Meter,
  Span,
  SpanContext,
  SpanOptions,
  SpanStatus,
  TimeInput,
  Tracer,
  createNoopMeter,
} from '@opentelemetry/api';

import { TelemetryClient } from './telemetry.js';

export class NoopTelemetryClient implements TelemetryClient {
  getMeter(): Meter {
    return createNoopMeter();
  }

  getTracer(): Tracer {
    return new NoopTracer();
  }

  stop(): Promise<void> {
    return Promise.resolve();
  }
}

// @opentelemetry/api internally uses NoopTracer and NoopSpan but they're not exported
// make our own versions
// https://github.com/open-telemetry/opentelemetry-js/issues/4518#issuecomment-2179405444
class NoopTracer implements Tracer {
  startSpan(): Span {
    return new NoopSpan();
  }

  startActiveSpan<F extends (...args: any[]) => any>(
    _name: unknown,
    _options: unknown,
    _context?: unknown,
    fn?: F,
  ): ReturnType<F> {
    return fn?.(new NoopSpan());
  }
}

class NoopSpan implements Span {
  private recording: boolean = true;
  addEvent(): this {
    return this;
  }

  addLink(): this {
    return this;
  }

  addLinks(): this {
    return this;
  }

  end(): void {
    this.recording = false;
  }

  isRecording(): boolean {
    return this.recording;
  }

  recordException(exception: Exception, time?: TimeInput | undefined): void {
    return;
  }

  setAttribute(key: string, value: AttributeValue): this {
    return this;
  }

  setAttributes(attributes: Attributes): this {
    return this;
  }

  setStatus(status: SpanStatus): this {
    return this;
  }

  spanContext(): SpanContext {
    return {
      spanId: '',
      traceId: '',
      traceFlags: 0,
    };
  }

  updateName(name: string): this {
    return this;
  }
}
