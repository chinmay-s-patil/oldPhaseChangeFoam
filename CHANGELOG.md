# Changelog & Remediation Record

All notable changes, license alignments, and code hygiene updates to `phaseChangeFoam` are documented in this file.

---

## [0.2.1] - 2026-11-05

### Fixed
* **Stefan Benchmark Setup Documentation**: Corrected documented Stefan problem parameters in `docs/validation.md` to $T_{\text{hot}} = 350\text{ K}$, $T_m = 305\text{ K}$, and $T_{\text{init}} = 280\text{ K}$ (matching `verification/phaseChange1D` case files and transcendental root $\lambda = 0.445590$, $s_{\text{exact}} = 28.182\text{ mm}$).
* **Interface Heat Flux Reporting**: Corrected interface heat flux error from a unit slip of $0.012\%$ to $1.19\%$ ($1581\text{ W/m}^2$ vs $1600\text{ W/m}^2$) and aligned pass tolerance with the actual script gate ($< 2.0\%$).
* **Local Path Leakage & Broken Links**: Sanitized all hardcoded `/home/lavender` fallbacks across 20 test runners and analysis scripts with portable dynamic OpenFOAM lookups (`os.path.expanduser` and `glob`). Converted absolute file-scheme links (`/home/lavender/...`) to relative repository paths in `validation/gallium2D/README.md` and `ANALYSIS_REPORT.md`.
* **CI Link Verification**: Expanded GitHub Actions link checker to recursively validate all markdown files across the repository (`**/*.md`).
* **Release & Citation Metadata**: Synchronized `CITATION.cff` version and release date with repository tags and changelog.
* **Reference Evidence & Digitization Clarity**: Removed stale references to deleted third-party paper PDF in `validation/gallium2D/reference_data/README.md` and `runTest.py`, plainly documenting that Brent curves were traced directly from Fig. 9 with $\pm 1\text{ mm}$ uncertainty.
* **Cross-Document Consistency**: Reconciled de Vahl Davis cavity dimensions ($L = 0.01\text{ m}$), iterations ($500$), and Nusselt results in `validation/README.md`. Harmonized gallium aspect ratio notations ($A_{H/W} = 0.714$, $A_{W/H} = 1.40$) and updated `validation/runAll.py` and `validation/README.md` to consistently represent the complete 5-rung validation ladder.
* **Ye & Arıcı Citation Precision**: Disambiguated citations between the two 2023 papers by Ye & Arıcı in *Int. Commun. Heat Mass Transfer*: properly attributed Eq. (6) to Vol. 144, 106780 (*"3D validation, 2D feasibility, corrected and developed correlations..."*), and 2D interface benchmark data to Vol. 147, 106952 (*"Redefined interface error, 2D verification and validation..."*), fixing paper titles and DOIs across `README.md`, `validation/README.md`, and `docs/validation.md`.
* **Claim-by-Claim Audit Reconciliations**:
  * *Lee Model Verification Artifact*: Executed and committed `verification/leeModel/lee_verification_results.txt` substantiating species mass conservation ($\sum Y_i = 1$ to $< 10^{-12}$), interspecies closure ($< 10^{-12}$), rate increment agreement ($\sim 10^{-8}$), energy closure ($< 0.01\%$), and Clausius-Clapeyron $T_{\text{sat}}(p)$ limits across all 9 cases.
  * *Stefan Temporal Self-Convergence*: Qualified order $p = 0.96$ as temporal self-convergence evaluated relative to the $\Delta t = 1\text{ s}$ baseline on a fixed 1 mm mesh.
  * *Restart Precision Wording*: Replaced "restart bit-invariance" with "restart-consistent to write precision ($< 10^{-9}\text{ K}$)".
  * *Tin Conduction Consistency Check*: Reframed early tin melting ($f_L = 0.0881$ vs $0.0893$, $-1.23\%$) as an early-conduction consistency check.
  * *de Vahl Davis Velocity Convergence*: Clarified that mean Nusselt converges monotonically ($+1.97\% \to +0.70\% \to +0.64\%$), while peak velocity errors fluctuate non-monotonically within 1–2%, and noted 1500 iterations for the $80 \times 80$ grid.
  * *Gallium Benchmarking Framing*: Clarified that Ye & Arıcı agreement (+0.58%) compares against a fitted 2D numerical correlation rather than physical experimental data.
  * *Gallium vs Brent Systematic Offset*: Reported systematic positive offset (+18.2%, +9.7%, +8.1%, +7.5%) across all time steps, dropped uncited "outlier" phrasing, and stated plainly that while later numerical studies report higher values ($f_L \approx 0.570\text{--}0.574$), the exact cause of the offset against Brent et al. (1988) has not been isolated.
  * *CI Regression Gates Disclosure*: Replaced "pre-fixed engineering tolerance gates" with user-defined regression gates and disclosed narrow margins ($0.0402$ margin on $f_L$; $3.46\text{ mm}$ vs $4.45\text{ mm}$ on front deviation).
  * *GCI Asymptotic Scope*: Softened GCI reporting ($p = 2.70$, $2.04\% / 0.67\%$) to apparent mesh-sensitivity indices under ASME V&V 20 guidelines, noting that non-monotonicity of global liquid fraction precludes claiming strict asymptotic range.
  * *EHC vs Enthalpy-Porosity Outer Correctors*: Clarified that EP with $\lambda = 1.0, n_{\text{outer}} = 8$ achieves excellent energy closure ($-0.06\%$), while EHC's key advantage is maintaining closure at low outer correctors ($n_{\text{outer}} = 3\text{--}4$).
  * *Gau & Viskanta 3D Screening Front Provenance*: Traced midplane front targets to Gau & Viskanta (1986) Fig. 5 pour-out curves, documenting that while broad cavity-width regression screens pass, the local front position exhibits $32.2\%$ deviation ($11.24\text{ mm}$ vs $8.50\text{ mm}$) due to initial heater transient inertia.
  * *Hysteresis Case Scope & Citation*: Clarified that `pcmHysteresis_experimental` evaluates cyclic path logic on synthetic windows ($[308, 312]\text{ K}$ and $[303, 307]\text{ K}$) rather than fitting raw experimental DSC curves; corrected Bony & Citherlet citation to *Energy and Buildings*, Vol. 39, Issue 10, DOI `10.1016/j.enbuild.2006.10.017`.
  * *Master Verification Gate Verification*: Verified actual runtime (~3.3 min / ~200 s) and check count (13 suites, 90+ quantitative checks) across `verification/runAll.py`.
  * *Build and Packaging Cleanup*: Removed nonexistent include directories (`porousFluid`, `porousSolid`) from solver `Make/options` and nonexistent `tutorials/*` path from `.reuse/dep5`. Pointed Docker command in `README.md` to tagged `v0.2.1`.

## [0.2.0] - 2026-11-04

### Added
* **Three-Tier Verification & Validation Hierarchy**: Formalized ASME V&V 20 / Roache structure into Tier 1 (Verified exact analytic solutions), Tier 2 (Benchmarked numerical CFD literature), and Tier 3 (Physical experimental validation roadmap).
* **ASME V&V 20 Framework**: Outlined roadmap for 3D pure gallium cavity validation against Gau & Viskanta (1986) with experimental uncertainty quantification and grid convergence index (GCI).
* **de Vahl Davis Natural Convection Benchmark**: Multi-grid spatial convergence sequence ($40 \times 40$, $50 \times 50$, $80 \times 80$) demonstrating monotonic convergence of mean Nusselt and peak velocities.
* **Master Automated Verification Gate**: Suite runner `verification/runAll.py` consolidating 13 suites and 60+ quantitative gates.

## [0.1.1] - 2026-10-27

### License and Attribution Alignment
* **Full GNU GPL v3 Text Restored**: Restored the complete, unmodified 674-line text of the GNU General Public License Version 3 (`LICENSE`), including Preamble, Sections 0–17, and the "How to Apply" appendix.
* **SPDX License Consistency**: Aligned the project to `GPL-3.0-or-later` uniformly across `README.md`, repository badge, `CITATION.cff`, and all source code files.
* **Verbatim Upstream Copyright Attribution**: Restored exact upstream OpenFOAM-v2412 attribution headers (`Copyright (C) 2011-2016 OpenFOAM Foundation`, `Copyright (C) 2017-2019,2022 OpenCFD Ltd.`) to derived solvers (`phaseChangeMultiRegionFoam.C`, `phaseChangeMultiRegionSimpleFoam.C`) and all 43 derived include files in `fluid/`, `solid/`, and `include/`.
* **Novel Model & Utility Attribution**: Applied `Copyright (C) 2026 Chinmay S Patil` and `SPDX-License-Identifier: GPL-3.0-or-later` to all novel solid PCM models (`phaseChangeModel`, `ehcPhaseChangeModel`, `ehcDarcyPhaseChangeModel`, `enthalpyPorosityPhaseChangeModel`, `nonePhaseChangeModel`), fluid phase change models (`fluidPhaseChangeModel`, `leeFluidPhaseChangeModel`, `constantSourceFluidPhaseChangeModel`, `noneFluidPhaseChangeModel`), and the `writePCDict` utility.
* **In-File GNU GPL §5(a) Modification Notices**: Embedded explicit modification notices inside each derived file (e.g. `Modified 2026-10 by Chinmay S Patil: <details>`) detailing specific algorithmic additions, thermal hysteresis tracking, and conjugate coupling logic.
* **REUSE Compliance (DEP-5)**: Added `.reuse/dep5` mapping license and copyright declarations across all case files, dictionaries, documentation, and test suites.

### Repository Hygiene and File Cleanup
* **Removed Publisher-Copyrighted Materials**: Deleted third-party PDF papers from `validation/papers/` to ensure full compliance with academic publisher copyrights; references and DOIs are maintained in documentation.
* **Removed Internal Work Artifacts**: Removed internal working notes (`ideas/`), temporary directories (`scratch/`), directory tree dumps (`log.tree`), and compiled Python bytecode (`__pycache__/`).
* **Enhanced `.gitignore`**: Added patterns ignoring build artifacts (`*.o`, `*.so`, `*.dep`), time step directories, temporary run outputs, `log.tree`, and `tutorials/`.
* **Sanitized Work Logs**: Replaced internal development logs containing local paths with this clean `CHANGELOG.md`.

### Bug Fixes in Verification Suites
* **Working Directory State Restoration**: Fixed `setup_single_cell_case` in `verification/singleCellOvershoot/runTest.py` to restore the original working directory upon exit, preventing recursive directory nesting under `case9_variableDensity/`.
* **Duplicate Mesh Directory Generation**: Replaced recursive `cp -r` commands with clean destination removal prior to mesh copying in `verification/singleCellOvershoot/runTest.py`, `verification/testModelFeatures/runTest.py`, and `verification/phaseChange1D_unequalCp/runTest.py`, eliminating duplicate `polyMesh/polyMesh` directories on rerun.
* **Cleaned Stray Test Artifacts**: Purged nested bug directories and duplicate mesh hierarchies.

### Documentation Updates
* **Tutorial Status Clarification**: Updated `README.md` and `docs/tutorials.md` with clear notes indicating standalone demonstration tutorials are actively undergoing development as a work in progress and pointing users to verified test and benchmark suites for runnable cases.
* **CI Verification Compatibility**: Ensured all documentation links in `README.md` and `docs/*.md` resolve properly in automated continuous integration workflows.

## [0.1.0] - 2026-10-26

* Initial release of `phaseChangeFoam` multi-region solver suite for OpenFOAM v2412.
