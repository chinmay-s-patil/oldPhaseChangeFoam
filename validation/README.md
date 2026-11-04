# Natural Convection & Phase Change Validation Ladder

This directory contains the self-contained validation suite for the `phaseChangeFoam` suite of solvers (`phaseChangeMultiRegionFoam` and `phaseChangeMultiRegionSimpleFoam`).

It implements a progressive five-rung validation ladder verifying natural convection, cyclic thermal hysteresis, liquid metal melting benchmarks, and physical experimental validation screening.

---

## Validation Hierarchy

```
[ Rung 5: 3D Pure Gallium Cavity Melting ]
Gau & Viskanta (1986)
Physical experimental validation screening with no-slip sidewall braking
Transient solver: phaseChangeMultiRegionFoam
               ▲
               │  (Physical experimental validation)
               │
[ Rung 4: 2D Gallium Melting Benchmark ]
Brent, Voller, Reid (1988) / Ye & Arıcı (2023)
Coupled phase change, Darcy mushy drag, natural convection (Ra ≈ 6.06e5, Pr = 0.0216, Ste = 0.039)
Transient solver: phaseChangeMultiRegionFoam
               ▲
               │  (Coupled convection-diffusion phase change)
               │
[ Rung 3: 2D Pure Tin Melting ]
Bertrand et al. (1999) / Hannoun et al. (2003)
Liquid metal melting benchmark (Ra = 2.50e5, Pr = 0.02, Ste = 0.01)
Transient solver: phaseChangeMultiRegionFoam
               ▲
               │  (Low-Prandtl number liquid metal regime)
               │
[ Rung 2: EHC Cyclic Thermal Hysteresis ]
Direction-dependent melting and freezing loop with 5 K hysteresis window
State-variable subcooling tracking and discrete energy conservation
Transient solver: phaseChangeMultiRegionFoam
               ▲
               │  (Hysteresis path state logic)
               │
[ Rung 1: Natural Convection Cavity ]
de Vahl Davis (1983)
Pure buoyancy circulation (Ra = 10^5, Pr = 0.71) in square cavity
Steady-state solver: phaseChangeMultiRegionSimpleFoam
```

---

## Benchmarks Overview

### Rung 1: de Vahl Davis (1983) Natural Convection Cavity
- **Directory**: [`deVahlDavis2D/`](deVahlDavis2D/)
- **Solver**: `phaseChangeMultiRegionSimpleFoam` (steady-state Boussinesq)
- **Physics**: Laminar natural convection in a differentially heated square cavity ($L = 0.01\text{ m}$) at $\text{Ra} = 10^5, \text{Pr} = 0.71$.
- **Benchmark Metric**: Mean Nusselt number along the hot wall ($\overline{\text{Nu}} = 4.519$).
- **Status**: **PASSED** ($\overline{\text{Nu}} = 4.5506$, relative difference **0.70%** on nominal $50 \times 50$ grid at 500 iterations; **0.64%** on $80 \times 80$, tolerance $\le 5\%$).
- **Runtime**: $\approx 2\text{ s}$ (500 SIMPLE iterations).

### Rung 2: EHC Cyclic Thermal Hysteresis
- **Directory**: [`pcmHysteresis_experimental/`](pcmHysteresis_experimental/)
- **Solver**: `phaseChangeMultiRegionFoam`
- **Physics**: Direction-dependent cyclic melting ($[308, 312]\text{ K}$) and freezing ($[303, 307]\text{ K}$) loop with state-variable subcooling retention.
- **Status**: **PASSED** (100% molten at peak heating, complete subcooling retention at $310.9\text{ K}$, complete crystallization at cycle end).
- **Runtime**: $\approx 2\text{ s}$.

### Rung 3: Bertrand / Hannoun Pure Tin Melting
- **Directory**: [`tin2D/`](tin2D/)
- **Solver**: `phaseChangeMultiRegionFoam`
- **Physics**: Low-Prandtl liquid metal melting in a square cavity ($\text{Ra} = 2.50 \times 10^5, \text{Pr} = 0.02, \text{Ste} = 0.01$).
- **Status**: **PASSED** (Early-time conduction regime at $t = 100\text{ s}$: simulated $f_L = 0.0881$ matches exact 1D Stefan limit $0.0892$ within $-1.23\%$).
- **Runtime**: $\approx 15\text{ s}$.

### Rung 4: Brent, Voller, Reid (1988) / Ye & Arıcı (2023) Gallium Melting Benchmark
- **Directory**: [`gallium2D/`](gallium2D/)
- **Solver**: `phaseChangeMultiRegionFoam` (transient PIMPLE with enthalpy-porosity / EHCDarcy)
- **Physics**: Melting of pure gallium in a rectangular enclosure ($W = 8.89\text{ cm}, H = 6.35\text{ cm}$, height-to-width aspect ratio $A_{H/W} = 0.714$, width-to-height ratio $A_{W/H} = 1.40$) subject to hot vertical wall ($T_h = 311.15\text{ K}$) and initial subcooling ($T_c = 301.45\text{ K}, T_m = 302.93\text{ K}$).
  - Stefan number: $\text{Ste} = c_p \Delta T / L = 0.039$
  - Prandtl number: $\text{Pr} = \nu / \alpha = 0.0216$
  - Rayleigh number: $\text{Ra} = g \beta \Delta T H^3 / (\nu \alpha) \approx 6.06 \times 10^5$
- **Benchmark Metrics**:
  1. Liquid volume fraction evolution $f_L(t)$ across full $17\text{ min}$ transient ($f_L = 0.5736$ vs Ye & Arıcı correlation $0.5703$, $+0.58\%$).
  2. Melt front profile $x_{\text{front}}(y)$ at $t = 2, 6, 10, 17\text{ min}$ compared against Brent et al. (1988) Fig. 9 numerical front curves.
  3. Discrete backward-Euler energy balance audit ($|\Delta E| \le 0.5\%$).
- **Status**: **PASSED** (2-min regression gate: $f_L = 0.1303 \pm 0.002$, energy closure $-0.34\%$).
- **Runtime**: $\approx 35\text{ s}$ for 2-min gate; $\approx 4\text{--}5\text{ min}$ for full 17-min transient.

### Rung 5: Gau & Viskanta (1986) 3D Gallium Cavity Validation Screening
- **Directory**: [`gallium3D_experimental/`](gallium3D_experimental/)
- **Solver**: `phaseChangeMultiRegionFoam`
- **Physics**: 3D rectangular cavity ($8.89\text{ cm} \times 6.35\text{ cm} \times 3.81\text{ cm}$) with physical no-slip inspection windows exerting 3D sidewall viscous drag.
- **Status**: **SCREENING RUN** (Preliminary $120\text{ s}$ run demonstrates sidewall braking; ASME V&V 20 roadmap for experimental heater transient underway).
- **Runtime**: $\approx 2.5\text{ min}$.

---

## Reference Data & Literature Sources
- **Brent, Voller, Reid (1988)**: "Enthalpy-Porosity Technique for Modeling Convection-Diffusion Phase Change: Application to the Melting of a Pure Metal", *Numerical Heat Transfer*, 13(3), pp. 297–318. DOI: [10.1080/10407788808913615](https://doi.org/10.1080/10407788808913615).
- **de Vahl Davis (1983)**: "Natural convection of air in a square cavity: a bench mark numerical solution", *Int. J. Numer. Meth. Fluids*, 3, pp. 249–264. DOI: [10.1002/fld.1650030305](https://doi.org/10.1002/fld.1650030305).
- **Hannoun, Alexiades, Mai (2003)**: "Resolving the Controversy over Tin and Gallium Melting in a Rectangular Cavity", *Numerical Heat Transfer, Part B: Fundamentals*, 44(3), pp. 253–276. DOI: [10.1080/716100520](https://doi.org/10.1080/716100520).
- **Bertrand et al. (1999)**: "Melting Driven by Natural Convection. A Comparison of Numerical Solutions", *Int. J. Numer. Meth. Heat & Fluid Flow*, 9(8), pp. 854–877. DOI: [10.1108/09615539910299863](https://doi.org/10.1108/09615539910299863).
- **Ye & Arıcı (2023)**: "Redefined-interface-error-based 2D verification and validation study on natural convection melting: Benchmark with pure Gallium", *International Communications in Heat and Mass Transfer*, 147, 106952. DOI: [10.1016/j.icheatmasstransfer.2023.106952](https://doi.org/10.1016/j.icheatmasstransfer.2023.106952).
- **Gau & Viskanta (1986)**: "Melting and Solidification of a Metal System—I. Melting from a Vertical Wall", *Int. J. Heat Mass Transfer*, 29(12), pp. 1951–1963. DOI: [10.1016/0017-9310(86)90011-2](https://doi.org/10.1016/0017-9310(86)90011-2).
- **`gallium2D/reference_data/`**: Digitized melt front coordinates ($t = 2, 6, 10, 17\text{ min}$) traced from Brent et al. (1988) Fig. 9 with $\pm 1\text{ mm}$ uncertainty.

---

## Running the Validation Ladder

Run the validation ladder from the repository root:

```bash
# Run the complete ladder (All 5 rungs, ~3.5 min):
python3 validation/runAll.py

# Run fast gate (Rungs 1 & 2: de Vahl Davis & EHC hysteresis, ~5 s):
python3 validation/runAll.py --fast

# Run individual benchmarks:
python3 validation/deVahlDavis2D/runTest.py
python3 validation/pcmHysteresis_experimental/runTest.py
python3 validation/tin2D/runTest.py
python3 validation/gallium2D/runTest.py --endTime 120 --plot
python3 validation/gallium3D_experimental/runTest.py
```
