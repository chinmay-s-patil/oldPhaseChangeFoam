# Case Setup & Reference Configurations

> [!NOTE]
> No standalone tutorial cases are included in this release. The runnable simulation setups provided in this repository are **verification and validation test suites**. Users configuring new simulations should consult the verified case setups in [`verification/`](../verification/) and [`validation/`](../validation/) as reference configurations.

---

## Reference Case Setups

Verified simulation cases and automated runners are available in the test suites:

* **Verification Suite**: [`verification/`](../verification/) — Quantitative verification cases covering phase change kinetics, energy conservation, and thermal hysteresis.
* **Validation Benchmarks**: [`validation/`](../validation/) — Physical validation cases including pure gallium melting benchmarks.

For theoretical formulations, model equations, and keyword dictionaries, please refer to:
* [`docs/models.md`](models.md) — Governing equations, thermodynamic consistency, species evaporation, and PCM formulations.
* [`docs/dictionary.md`](dictionary.md) — Keyword reference tables and solver control templates.
* [`docs/validation.md`](validation.md) — Benchmark comparison procedures and quantitative convergence data.
