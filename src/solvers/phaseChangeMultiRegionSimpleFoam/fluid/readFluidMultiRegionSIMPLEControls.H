/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2022 OpenCFD Ltd.
    Copyright (C) 2026 Chinmay S Patil
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM, with modifications for phaseChangeFoam.
    SPDX-License-Identifier: GPL-3.0-or-later
    Modified 2026-10 by Chinmay S Patil: Added SIMPLE controls for phase change.
\*---------------------------------------------------------------------------*/

    const dictionary& simple = fluidRegions[i].solutionDict().subDict("SIMPLE");

    const int nCorr =
        simple.getOrDefault<int>("nCorrectors", 1);

    const int nNonOrthCorr =
        simple.getOrDefault<int>("nNonOrthogonalCorrectors", 0);

    const bool momentumPredictor =
        simple.getOrDefault("momentumPredictor", true);

    simple.readIfPresent("frozenFlow", frozenFlowFluid[i]);
