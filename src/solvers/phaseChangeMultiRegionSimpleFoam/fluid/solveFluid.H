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
    Modified 2026-10 by Chinmay S Patil: Steady-state fluid solving with phase change.
\*---------------------------------------------------------------------------*/

//  Pressure-velocity SIMPLE corrector

    {
        phaseChange.correct();
        phaseChange.checkMassConservation();
        if (pcmModel.active())
        {
            pcmModel.correct();
        }

        if (frozenFlow)
        {
            #include "EEqn.H"
        }
        else
        {
            p_rgh.storePrevIter();
            rho.storePrevIter();

            #include "UEqn.H"
            #include "YEqn.H"
            #include "EEqn.H"
            if (!coupled)
            {
                #include "pEqn.H"
                turbulence.correct();
            }
        }
    }



