# Verification and Validation Guide

Quantitative verification suites, numerical benchmark comparisons, and experimental validation roadmap for `phaseChangeFoam`.

---

## Terminology and Verification Hierarchy

In accordance with ASME V&V 10 / Roache (1998) standards, this document distinguishes between three levels of code assessment:

1. **Verified (Code Verification & Analytic Solutions)**: Solving the equations right — comparing numerical solutions against exact analytical solutions, verifying discrete energy and mass conservation, and checking scheme consistency and state-variable invariance.
2. **Benchmarked (Code-to-Code Numerical Verification)**: Comparing simulation outputs against established, peer-reviewed numerical benchmark solutions (e.g. de Vahl Davis 1983, Ye & Arıcı 2023, Hannoun et al. 2003, Brent et al. 1988).
3. **Validated (Physical Experimental Validation)**: Solving the right equations — comparing model predictions against experimental physical measurements (e.g. Gau & Viskanta 1986, Bertrand et al. 1999).

> [!NOTE]
> **Status Assessment**: `phaseChangeFoam` is currently **verified on its core numerical mechanics** and **benchmarked on 2D laminar natural convection and pure metal melting**. It is **not yet validated against physical experimental datasets**. Unrun validation targets and planned tests are systematically documented in the [Verification & Validation Roadmap](#4-verification--validation-roadmap-not-yet-covered) rather than omitted.

---

## 1. Verified: Analytic Solutions, MMS & Conservation Physics

The primary verification tests reside in the `verification/` directory (13 specialized test suites, 60+ quantitative gates, run via `python3 verification/runAll.py`). These cases verify specific mathematical and physical components in isolation:

| Suite & Directory | Physics & Mechanisms Verified | Verification Type | Key Acceptance Metrics |
| :--- | :--- | :--- | :--- |
| [`verification/singleCellOvershoot/`](../verification/singleCellOvershoot/) | **EHC Thermal Hysteresis & Subcooling**<br>State-variable tracking (`phaseState`, `T_reversal`), cyclic melt/freeze loops, unequal $C_p$, mushy start, restart invariance (22 cases). | Analytic / Discrete State | Complete cycle energy closure $< 0.1\%$, exact path recovery, restart bit-invariance. |
| [`verification/phaseChangeCoupled/`](../verification/phaseChangeCoupled/) | **Multi-Region CHT Interface Coupling**<br>Conjugate solid-liquid boundary energy flux continuity, harmonic interface thermal conductivity $k_{\text{eff}}$, multi-region energy balance. | Analytic / Energy Balance | Interface flux continuity $< 0.05\%$, overall coupled region energy balance $< 0.1\%$. |
| [`verification/leeModel/`](../verification/leeModel/) | **Volumetric Species Evaporation & Boiling**<br>Clausius-Clapeyron $T_{\text{sat}}(p)$, species mass and latent/sensible energy conservation, high $C \Delta t$ limiter, heated pot liquid exhaustion and vapor superheating (9 cases). | Analytic / Thermodynamic | Mass conservation error $< 10^{-10}$, energy closure $< 0.01\%$, Clausius-Clapeyron $T_{\text{sat}}(2\text{ bar})$ rate error $< 0.1\%$. |
| [`verification/phaseChange1D*/`](../verification/phaseChange1D/) | **1D Conduction Stefan-Type Melting**<br>Neumann flux boundary conditions, variable thermal conductivity $k(T)$, unequal heat capacities $C_{p,s} \neq C_{p,l}$, mushy-zone porosity damping. | Semi-Analytic / Similarity | Liquid fraction and temperature profile tracking against 1D Stefan limit. |
| [`verification/constantSource/`](../verification/constantSource/) | **Fixed-Rate Mass/Energy Injection Baseline**<br>Baseline verification for continuity source terms and scalar transport coupling. | Exact Conservation | Integral mass and enthalpy injection match discrete source within machine precision. |

---

## 2. Benchmarked: Code-to-Code Numerical Benchmarks

The benchmark suite tests coupled non-linear flow and phase change against published numerical CFD literature:

```
[ Rung 2: 2D Gallium Melting ]
Coupled phase change, Darcy mushy drag, natural convection (Ra ≈ 6.06e5, Pr = 0.0216, Ste = 0.039)
Benchmarked against Ye & Arıcı (2023), Hannoun et al. (2003), Brent et al. (1988)
Transient solver: phaseChangeMultiRegionFoam
               ▲
               │  (Verifies coupled thermal & phase change mechanics)
               │
[ Rung 1: 2D Natural Convection Cavity ]
Buoyancy-driven circulation in square cavity (Ra = 10^5, Pr = 0.71)
Benchmarked against de Vahl Davis (1983)
Steady-state solver: phaseChangeMultiRegionSimpleFoam
```

---

### Rung 1: Natural Convection Cavity (de Vahl Davis 1983)

* **Directory**: [`validation/deVahlDavis2D/`](../validation/deVahlDavis2D/)
* **Solver**: `phaseChangeMultiRegionSimpleFoam` (steady-state Boussinesq)
* **Runner**: `python3 validation/deVahlDavis2D/runTest.py`

#### Setup & Objectives
Laminar buoyancy-driven natural convection in an air-filled square cavity ($L = 0.01$ m, $\text{Pr} = 0.71$) at $\text{Ra} = 10^5$:
* Hot vertical wall: $T_h = 305.0$ K; Cold vertical wall: $T_c = 295.0$ K; Horizontal walls: Adiabatic ($\partial T / \partial y = 0$)
* Grid: $50 \times 50$ uniform cells (structured)
* Non-dimensional velocity scales: $u^* = u / (\alpha / L)$, $v^* = v / (\alpha / L)$ with $\alpha = \nu / \text{Pr} = 2.146 \times 10^{-6}\text{ m}^2/\text{s}$

#### Quantitative Comparison with de Vahl Davis (1983)

| Metric | Target (de Vahl Davis 1983) | Computed (`phaseChangeFoam`) | Relative Difference | Status |
| :--- | :---: | :---: | :---: | :---: |
| **Mean Nusselt ($\overline{\text{Nu}}$)** | $4.519$ | $4.5506$ | $+0.70\%$ | **PASSED** (Gate: $\le 5\%$) |
| **Max Nusselt ($\text{Nu}_{\max}$)** | $7.717$ | $7.9300$ | $+2.76\%$ | Reference |
| **Min Nusselt ($\text{Nu}_{\min}$)** | $0.729$ | $0.7200$ | $-1.23\%$ | Reference |
| **Peak Vertical Velocity $v_{\max}^*$ (at $y/L = 0.5$)** | $68.59$ | $68.75$ | $+0.23\%$ | **PASSED** (Gate: $\le 5\%$) |
| **Location $x/L$ of $v_{\max}^*$** | $0.066$ | $0.070$ | Within 1 cell ($\Delta x = 0.02$) | Reference |
| **Peak Horizontal Velocity $u_{\max}^*$ (at $x/L = 0.5$)** | $34.73$ | $35.53$ | $+2.30\%$ | **PASSED** (Gate: $\le 5\%$) |
| **Location $y/L$ of $u_{\max}^*$** | $0.855$ | $0.850$ | Within 1 cell ($\Delta y = 0.02$) | Reference |
| **Temperature Bounds** | $[295.0, 305.0]\text{ K}$ | $[295.0, 305.0]\text{ K}$ | $0.0\text{ K}$ overshoot | **PASSED** |

Execution time is ~3 s for 500 SIMPLE iterations using second-order bounded TVD convection schemes (`Gauss limitedLinear 1`). All target metrics agree within $2.8\%$ of the benchmark.

---

### Rung 2: Gallium Melting Benchmark

* **Directory**: [`validation/gallium2D/`](../validation/gallium2D/)
* **Solver**: `phaseChangeMultiRegionFoam` (transient PIMPLE)
* **Runner**: `python3 validation/gallium2D/runTest.py` (2-minute regression gate)
* **Extended Analysis**: [`validation/gallium2D/analysis/ANALYSIS_REPORT.md`](../validation/gallium2D/analysis/ANALYSIS_REPORT.md)

Melting of pure gallium in a rectangular enclosure ($W = 0.0889$ m, $H = 0.0635$ m, aspect ratio $A = 0.714$) subject to an isothermal hot vertical wall ($T_h = 38.0^\circ\text{C} = 311.15$ K) and subcooled initial cavity ($T_{\text{init}} = T_c = 28.3^\circ\text{C} = 301.45$ K, $T_m = 29.78^\circ\text{C} = 302.93$ K, $\text{Ra} \approx 6.06 \times 10^5$, $\text{Pr} = 0.0216$, $\text{Ste} = 0.0391$).

![Gallium 2D Melting Animation](images/gallium_melting_brent_comparison.gif)

![Gallium Melt Front & Liquid Fraction Comparison](images/gallium_melt_front_comparison.png)

#### Multi-Model Comparison across Full 17-Minute Transient ($42 \times 32$ Grid, $\Delta t = 0.1$ s)

| Metric | Benchmark Target | EP ($\lambda = 0.2$, tutorial) | EP ($\lambda = 1.0, n_{\text{outer}}=8$) | EHCDarcy ($n_{\text{outer}}=3$) | EHCDarcy ($n_{\text{outer}}=8$) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Energy closure ($t = 2$ min)** | $\le 2.0\%$ | $+7.35\%$ *(FAIL)* | **-0.06% (-1.2 J)** | **-0.38% (-7.2 J)** | **-0.34% (-6.4 J)** |
| **Energy closure ($t = 6$ min)** | $\le 2.0\%$ | $+7.37\%$ *(FAIL)* | **+0.11% (+4.2 J)** | **-0.26% (-9.6 J)** | **-0.23% (-8.4 J)** |
| **Energy closure ($t = 17$ min)** | $\le 2.0\%$ | $+7.49\%$ *(FAIL)* | **-0.10% (-8.0 J)** | **-0.16% (-12.7 J)** | **-0.13% (-10.3 J)** |
| **Mean front dev. ($t = 2$ min)** | $\le 5.0\% W$ ($4.45$ mm) | $1.95$ mm ($2.19\%$) | $1.71$ mm ($1.93\%$) | $1.68$ mm ($1.89\%$) | **1.67 mm (1.88%)** |
| **Mean front dev. ($t = 6$ min)** | $\le 5.0\% W$ ($4.45$ mm) | $2.60$ mm ($2.93\%$) | $1.98$ mm ($2.22\%$) | $1.88$ mm ($2.11\%$) | **1.87 mm (2.10%)** |
| **Mean front dev. ($t = 10$ min)** | $\le 5.0\% W$ ($4.45$ mm) | $3.51$ mm ($3.95\%$) | $2.45$ mm ($2.76\%$) | $2.37$ mm ($2.67\%$) | **2.35 mm (2.65%)** |
| **Mean front dev. ($t = 17$ min)** | $\le 5.0\% W$ ($4.45$ mm) | $5.43$ mm ($6.11\%$) *(FAIL)* | $3.52$ mm ($3.96\%$) | $3.49$ mm ($3.93\%$) | **3.46 mm (3.89%)** |
| **Peak front dev. ($t = 17$ min)** | $\le 10.0\% W$ ($8.89$ mm) | $9.74$ mm ($10.95\%$) *(FAIL)* | $6.40$ mm ($7.20\%$) | $6.38$ mm ($7.18\%$) | **6.34 mm (7.13%)** |
| **Liquid fraction $f_L$ ($t = 17$ min)** | $0.5334$ (Brent 1988) | $0.5967$ | **0.5745** | **0.5732** | **0.5736** |
| **Ye & Arıcı (2023) Eq. 6** | $0.5703$ | $+0.0264$ | **+0.0042** | **+0.0029** | **+0.0033 (+0.58%)** |
| **2-Minute CPU Solver Time** | — | $38.7$ s | $39.4$ s | **15.6 s** | $35.0$ s |

#### Candor on Brent et al. (1988) as a Historical Numerical Outlier

The simulated 17-minute liquid fraction ($f_L = 0.5736$) sits **7.5% above Brent et al.'s 1988 numerical prediction ($0.5334$)**. This difference is expected and well understood in modern literature:

1. **Historical Numerical Resolution**: Brent et al. (1988) solved the cavity using a $42 \times 32$ grid but switched to a large time step ($\Delta t = 0.5$ s) after only $t = 10$ s, using an early explicit Newton source-term formulation that lagged convergence during rapid early convection development.
2. **Consensus of Converged Benchmarks**: Hannoun, Alexiades, and Mai (2003) and Ye and Arıcı (2023) demonstrated that grid-converged 2D solutions yield $f_L \approx 0.570\text{--}0.574$. Ye & Arıcı's numerical correlation (Eq. 6) predicts $\phi_n = 0.5703$ at 17 min, which agrees with `phaseChangeFoam` within **0.33 percentage points (0.58% relative difference)**. Brent (1988) represents a historical, under-resolved numerical prediction rather than a true grid-converged limit.

#### Melt Front Deviation Framing

Front deviations relative to Brent (1988, Fig. 9) grow monotonically with time:
$$1.67\text{ mm (1.88% } W) \to 1.87\text{ mm (2.10% } W) \to 2.35\text{ mm (2.65% } W) \to 3.46\text{ mm (3.89% } W)$$
* These deviations are on the order of the front's **diffuse numerical interface thickness** ($\Delta x = W/42 = 2.12$ mm, $\Delta T_m = 0.5$ K).
* While they satisfy the pre-fixed $\le 5.0\% W$ ($4.45$ mm) engineering tolerance gate, late-time deviation ($3.46$ mm, $3.89\% W$) is close to this threshold.

#### Discrete Energy Balance Formulation

In OpenFOAM's finite-volume formulation with implicit backward-Euler time discretization (`Euler ddtScheme`), discrete energy storage over time step $\Delta t_n$ is evaluated at time level $t^n$:
$$\Delta E_{\text{stored}}^n = \dot{Q}_{\text{net}}(t^n)\,\Delta t_n$$
Discrete boundary energy input is evaluated as the right-endpoint Riemann sum:
$$E_{\text{net, in}}(t_N) = \sum_{n=1}^N \dot{Q}_{\text{net}}(t_n)\,\Delta t_n = \sum_{n=1}^N \left[\dot{Q}_{\text{hot}}(t_n) - \dot{Q}_{\text{cold, out}}(t_n)\right]\,\Delta t_n$$
Using composite trapezoidal quadrature introduces an artificial non-physical offset ($+3.63$ J at 2 min) because the intense initial conduction surge ($\dot{Q}(0.1\text{ s}) \approx 81$ W) decays rapidly. Evaluating the energy audit using right-endpoint summation ensures mathematical consistency with backward-Euler discrete equations.

---

### Spatial Grid Convergence & GCI (EHCDarcy)

Three structured meshes were evaluated to $t = 17$ min:
* Grid 3 (Coarse): $32 \times 24$ (768 cells, cell area metric $h_3 = 2.71$ mm)
* Grid 2 (Medium): $42 \times 32$ (1344 cells, $\Delta x = 2.12$ mm, $\Delta y = 1.98$ mm, cell area metric $h_2 = 2.05$ mm, $r_{32} = 1.322$)
* Grid 1 (Fine): $63 \times 48$ (3024 cells, $\Delta x = 1.41$ mm, $\Delta y = 1.32$ mm, cell area metric $h_1 = 1.37$ mm, $r_{21} = 1.500$)

#### Mid-Height Phase Front ($y/H = 0.5$ at $17$ min)
* Computed positions: $x_{\text{front}} = 56.01$ mm (coarse) $\to 57.06$ mm (medium) $\to 57.68$ mm (fine).
* **Observed spatial order**: $p = 2.70$ (ASME non-uniform refinement solution). In moving boundary problems, non-linear phase transitions combined with first-order Euler time stepping produce an apparent spatial order exceeding nominal second order.
* **Richardson Extrapolated Front**: $\phi_{\text{ext}} = 57.99$ mm.
* **Grid Convergence Index (GCI)**:
  * Medium grid: $\text{GCI}_{\text{med}}^{32} = 2.04\%$
  * Fine grid: $\text{GCI}_{\text{fine}}^{21} = 0.67\%$
  * Asymptotic consistency ratio: $\frac{\text{GCI}_{\text{med}}^{32}}{r_{21}^p\,\text{GCI}_{\text{fine}}^{21}} = \frac{2.04\%}{1.50^{2.70} \times 0.67\%} = 1.01$ (within $1\%$ of theoretical limit $1.00$).

> [!WARNING]
> **Non-Monotonicity of Global Liquid Fraction**: While the mid-height front position converges monotonically, global liquid fraction $f_L(17\text{ min})$ is non-monotonic across grids: $0.5682 \to 0.5736 \to 0.5725$. Total spread across all grids is only $0.0054$ ($0.94\%$), confirming spatial insensitivity beyond $42 \times 32$, but formal Richardson extrapolation cannot be claimed for global $f_L$.

---

### Parametric Sensitivity Studies (EHCDarcy)

Comprehensive sweeps from the extended analysis report ([`ANALYSIS_REPORT.md`](../validation/gallium2D/analysis/ANALYSIS_REPORT.md)) establish parameter sensitivities:

#### 1. Temporal Discretization ($\Delta t$ Halving)
* $\Delta t = 0.10$ s (1,200 steps): $f_L(2\text{m}) = 0.1303$, 2-min energy deficit = $-6.43$ J ($-0.341\%$).
* $\Delta t = 0.05$ s (2,400 steps): $f_L(2\text{m}) = 0.1305$, 2-min energy deficit = $-1.50$ J ($-0.080\%$).
* $\Delta t = 0.025$ s (4,800 steps): $f_L(2\text{m}) = 0.1306$, mid-height front shifts by only $0.03$ mm.
* Halving $\Delta t$ cuts the startup conservation deficit by **76.7%**, confirming that remaining energy discrepancy is a temporal startup discretization artifact rather than a formulation defect.

#### 2. Darcy Porosity Damping Constant ($C_u \in [10^4, 10^8]\text{ kg/(m}^3\text{ s)}$)
Sweeping $C_u$ across four orders of magnitude at $t = 17$ min:

| Darcy Constant $C_u$ | $f_L(10\text{ min})$ | $f_L(17\text{ min})$ | $\overline{\text{Nu}}_{\text{hot}}(17\text{ min})$ | Front Tilt ($x_{0.8}/x_{0.2}$) |
| :--- | :---: | :---: | :---: | :---: |
| $1.0 \times 10^4$ (weak damping) | $0.3906$ | $0.5987$ | $4.74$ | $2.74$ |
| $1.0 \times 10^5$ | $0.3817$ | $0.5848$ | $4.62$ | $2.55$ |
| **$1.6 \times 10^6$ (baseline)** | **$0.3747$** | **$0.5736$** | **$4.50$** | **$2.41$** |
| $1.0 \times 10^7$ | $0.3720$ | $0.5694$ | $4.47$ | $2.35$ |
| $1.0 \times 10^8$ (severe damping) | $0.3703$ | $0.5667$ | $4.44$ | $2.30$ |

#### 3. Numerical Melting Window ($\Delta T_{\text{melt}} \in [0.1, 0.5]\text{ K}$)
* $\Delta T_{\text{melt}} = 0.1$ K ($302.93 \pm 0.05$ K): $f_L(17\text{ min}) = 0.5727$
* $\Delta T_{\text{melt}} = 0.2$ K ($302.93 \pm 0.10$ K): $f_L(17\text{ min}) = 0.5728$
* $\Delta T_{\text{melt}} = 0.5$ K ($302.93 \pm 0.25$ K, baseline): $f_L(17\text{ min}) = 0.5736$
* Varying $\Delta T_{\text{melt}}$ from $0.5$ K down to $0.1$ K shifts $f_L(17\text{ min})$ by only $0.0009$ ($0.16\%$).

---

## 3. Validated: Physical Experimental Comparisons

> [!IMPORTANT]
> **Experimental Validation Status: Not Yet Claimed**  
> `phaseChangeFoam` does not claim physical experimental validation. All current quantitative results are code-to-code verification against numerical CFD solutions.

### Why 2D CFD Differs from Gallium Physical Experiments (Gau & Viskanta 1986)

Gau and Viskanta (1986) conducted foundational physical gallium melting experiments. However, direct quantitative validation against their raw data is complicated by physical factors absent in 2D CFD models:

1. **Heating Jacket Thermal Inertia**: In physical experiments, the hot wall took several minutes to ramp up to $38.0^\circ\text{C}$ due to heater heat capacity and thermal contact resistance. Numerical benchmarks impose an idealized Heaviside step at $t = 0$.
2. **Pour-Out Measurement Disruption**: Experimental front contours were obtained by draining liquid gallium at discrete times, disrupting the developing convection roll.
3. **3D Endwall Boundary-Layer Drag**: Physical test cells have side windows that exert viscous drag, slowing the recirculation roll relative to 2D planar simulations.
4. **Anisotropic Gallium Conductivity**: Solid gallium forms orthorhombic crystals with anisotropic thermal conductivity ($k \in [28, 41]\text{ W/(m K)}$), whereas CFD models assume isotropic $k = 32.0\text{ W/(m K)}$.

---

## 4. Verification & Validation Roadmap (Not Yet Covered)

To systematically close coverage gaps, future validation work is prioritized into three tiers:

### Tier 1: Cheap, Analytic, High Value (Formal Code Verification)

| Test | Physics / Mechanisms Checked | Pass Criteria & Tolerances | Status |
| :--- | :--- | :--- | :---: |
| **Neumann 1D Stefan Melting** (one- and two-phase, $k, C_p$ differ) | Melt front $s(t) = 2\lambda\sqrt{\alpha t}$, temperature profile, latent heat integration | Front position error $< 1\%$ as $\Delta T_m \to 0$; sweep $\Delta x, \Delta t, \Delta T_m$ | Covered in `verification/phaseChange1D*` (expand sweep) |
| **Two-Slab Contact Temperature** (semi-infinite, $T_i = \frac{e_1 T_1 + e_2 T_2}{e_1 + e_2}$, $e = \sqrt{k\rho C_p}$) | Multi-region interface coupling, harmonic flux continuity | Interface temperature error $< 0.5\%$; repeat with PCM in one slab and non-conformal meshes | Planned |
| **Composite Wall Steady Conduction** (series thermal resistance) | Multi-region interface energy coupling in `phaseChangeMultiRegionSimpleFoam` | Net wall heat flux error $< 0.1\%$ | Planned |
| **Darcy Mushy Slab** (uniform $f_L$, imposed $\Delta P$) | Carman-Kozeny mushy-zone drag sink in isolation | Darcy velocity error $< 1\%$ vs analytic Carman-Kozeny | Planned |
| **PCM-Off vs chtMultiRegionFoam** (channel / heated fin) | Base conjugate heat transfer consistency (no regression) | Relative difference $< 10^{-6}$ on $T, \mathbf{u}$ | Planned |
| **MMS (Method of Manufactured Solutions)** | Formal spatial and temporal discretization order for energy equation with smooth $C_{p,\text{eff}}(T)$ ramp | Observed order within $\pm 0.1$ of nominal ($1$ in time, $2$ in space) | Planned |
| **Restart & Parallel Invariance** ($0 \to T$ vs $0 \to T/2 \to \text{restart} \to T$) | Hysteresis state fields (`phaseState`, `T_reversal`) written and restored; domain decomposition | Restart difference $< 10^{-10}$; parallel vs serial difference $< 10^{-6}$ | Partial (restart covered in `singleCellOvershoot` cases 21-22) |

### Tier 2: Feature Coverage Expansion

| Test | Physics / Mechanisms Checked | Pass Criteria & Tolerances | Status |
| :--- | :--- | :--- | :---: |
| **Lee Model: Closed Adiabatic Box Tsat Relaxation** | Multi-component mixture relaxes to $T_{\text{sat}}(p)$ under Clausius-Clapeyron | Final temperature matches $T_{\text{sat}}(p)$ within $< 0.05$ K | Covered in `verification/leeModel` |
| **Lee Model: Conservation Audit** | Species sum $\sum Y_i = 1$, total mass $\int \dot{m} \, dt = m_{\text{vap}}$ | Total mass conserved to machine precision; energy closure $< 0.01\%$ | Covered in `verification/leeModel` |
| **Lee Model: Continuity Source Divergence** | $\nabla \cdot \mathbf{u} = \dot{m}(1/\rho_v - 1/\rho_l)$ in closed domain | Velocity divergence matches phase change volume expansion within $< 0.5\%$ | Planned |
| **Lee Model: 1D Stefan Evaporation Layer** | Superheated wall vapor growth vs analytic one-phase Stefan solution in high-transfer limit | Interface position error $< 2\%$ | Planned |
| **`constantSource` vs Lee Model** | Fixed mass/energy source vs equivalent dynamic rate | Equivalent bulk enthalpy rise within $< 0.1\%$ | Covered in `verification/constantSource` |
| **de Vahl Davis Natural Convection Sweep** | $\text{Ra} = 10^3, 10^4, 10^5, 10^6$ with $\overline{\text{Nu}}, \text{Nu}_{\max}, \text{Nu}_{\min}, u_{\max}^*, v_{\max}^*$ | $\overline{\text{Nu}}$ within $2\%$ across all Rayleigh numbers | Planned |
| **Steady SIMPLE vs Transient PIMPLE Limit** | Long-time limit of `phaseChangeMultiRegionFoam` vs steady-state `phaseChangeMultiRegionSimpleFoam` | Velocity and temperature fields match within $< 0.1\%$ | Planned |
| **`maxAlphaCo` Adaptive Time-Step Limiting** | Large nominal $\Delta t$ with `maxAlphaCo` enabled vs fixed small $\Delta t$ | Prevents front-skipping; energy closure preserved within $< 1.0\%$ | Planned |
| **Gallium Parametric Sweeps ($\Delta t, \Delta T_m, C_u$)** | Sensitivity documentation for pure gallium benchmark | Documented in `validation/gallium2D/analysis/` | **Documented** |
| **EP vs EHCDarcy on 1D Stefan Melting** | Formulation consistency against analytic solution on identical grid | Front position agreement within $< 0.5\%$ between models | Planned |

### Tier 3: Physical Experimental Validation

| Test | Physical System | Validation Dataset | Key Validation Metrics | Status |
| :--- | :--- | :--- | :--- | :---: |
| **Pure Gallium Melting** | Vertical cavity melting | Gau and Viskanta (1986) | Phase-front position vs time across cavity height with experimental thermal boundary conditions | Planned |
| **PCM Cavity Melting** | Horizontal / vertical enclosure | Bertrand et al. (1999) | Comparison across n-octadecane melting benchmark exercise | Planned |
| **DSC Thermal Hysteresis** | Experimental DSC heating/cooling curves | Literature paraffin or salt hydrate | Match non-equilibrium subcooling and asymmetric latent heat release curves | Planned |
| **Solidification with Natural Convection** | Water freezing with density anomaly | Kowalewski and Rebow (1999) | Inversion of natural convection roll near $4^\circ\text{C}$ density maximum | Optional |

---

## 5. Running the Test Suites

### Master Verification Gate (13 suites, ~90 s)
```bash
python3 verification/runAll.py
```

### Validation Ladder Fast Gate (Rung 1 natural convection, ~2.5 s)
```bash
python3 validation/runAll.py --fast
```

### Full Validation Ladder (Rungs 1 and 2, ~38 s)
```bash
python3 validation/runAll.py
```

### Running Individual Cases
```bash
# Rung 1: de Vahl Davis natural convection cavity
cd validation/deVahlDavis2D
python3 runTest.py

# Rung 2: Brent gallium melting 2-minute regression gate
cd validation/gallium2D
python3 runTest.py
```
