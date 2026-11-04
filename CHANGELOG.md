# Changelog & Remediation Record

All notable changes, license alignments, and code hygiene updates to `phaseChangeFoam` are documented in this file.

---

## [Unreleased] - 2026-10-27

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
