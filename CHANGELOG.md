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

## [0.2.0] - 2026-11-04

### Added
* **Three-Tier Verification & Validation Hierarchy**: Formalized ASME V&V 10 / Roache structure into Tier 1 (Verified exact analytic solutions), Tier 2 (Benchmarked numerical CFD literature), and Tier 3 (Physical experimental validation roadmap).
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
