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
    Modified 2026-10 by Chinmay S Patil: Multi-region Courant number evaluation.
\*---------------------------------------------------------------------------*/

    scalar CoNum = -GREAT;
    forAll(fluidRegions, regionI)
    {
        CoNum = max
        (
            compressibleCourantNo
            (
                fluidRegions[regionI],
                runTime,
                rhoFluid[regionI],
                phiFluid[regionI]
            ),
            CoNum
        );
    }
