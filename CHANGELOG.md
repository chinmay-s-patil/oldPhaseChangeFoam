# Changelog

All notable changes, features, and verification records for `phaseChangeFoam` are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.1.0] - 2026-11-08

Initial public release of the `phaseChangeFoam` multi-region conjugate heat transfer and phase change solver suite for OpenFOAM-v2412.

### Features & Capabilities

* **Multi-Region Solvers**:
  * `phaseChangeMultiRegionFoam`: Transient PIMPLE multi-region solver coupling solid and fluid regions with species phase change and solid-liquid PCM.
  * `phaseChangeMultiRegionSimpleFoam`: Steady-state SIMPLE solver for multi-region buoyancy-driven natural convection and conduction.

* **Solid-Liquid Phase Change Models (`phaseChangeModel`)**:
  * `EHCDarcy`: Path-based sensible heat capacity with linearised latent heat sources and Carman-Kozeny Darcy drag damping for convecting fluid regions.
  * `EHC`: Effective heat capacity formulation with directional thermal hysteresis and subcooling tracking for solid or pure-conduction domains.
  * `enthalpyPorosity`: Source-based formulation following Voller & Prakash (1987) with Newton projection on liquid fraction and mushy-zone Darcy resistance.

* **Fluid Phase Change Models (`fluidPhaseChangeModel`)**:
  * `Lee`: Volumetric species evaporation and condensation kinetics with constant or Clausius-Clapeyron pressure-dependent saturation temperature $T_{\text{sat}}(p)$.
  * `constantSource`: Fixed-rate mass/energy source model for verification baselines.

* **Utilities & Scripts**:
  * `writePCDict`: Utility to generate region-specific `constant/<region>/phaseChangeDict` dictionary templates with solver defaults and comments.
  * Clean scripts: `Allclean`, `verification/Allclean`, and `validation/Allclean` to purge generated simulation run cases, time steps, logs, and caches across the repository.
  * Deduplicated multi-region solver headers (sharing `fluid/initContinuityErrs.H` between transient and steady solvers) and purged obsolete scratch test files.
  * Harmonized test runner permissions (`chmod 755`) across all verification and validation `runTest.py` suites.

### Verification & Validation

* **Analytical & Code Verification (`verification/runAll.py`)**:
  * 13 automated test suites verifying moving melt front against Carslaw & Jaeger two-phase Stefan analytical solutions ($0.43\%$ front position error), semi-infinite two-slab contact temperature ($0.024\%$ error), interface flux continuity, discrete energy closure, and Lee model interspecies mass and enthalpy conservation.
* **Progressive Validation Ladder (`validation/runAll.py`)**:
  * Rung 1: de Vahl Davis (1983) laminar natural convection cavity ($\text{Ra} = 10^5$, $\overline{\text{Nu}}$ within $0.64\%$).
  * Rung 2: EHC cyclic thermal hysteresis state-logic verification (`validation/pcmHysteresis`).
  * Rung 3: Bertrand et al. (1999) / Hannoun et al. (2003) pure tin melting early-conduction consistency check.
  * Rung 4: Brent et al. (1988) / Ye & Arıcı (2023) 2D pure gallium melting benchmark.
  * Rung 5: Gau & Viskanta (1986) 3D pure gallium melting preliminary screening run.

### Licensing & Attribution

* Licensed under GPL-3.0-or-later (`LICENSE`).
* Complete upstream OpenFOAM attribution headers and GNU GPL §5(a) modification notices retained across derived files.
* REUSE-compliant copyright and licensing metadata (`REUSE.toml`, `LICENSES/GPL-3.0-or-later.txt`).
