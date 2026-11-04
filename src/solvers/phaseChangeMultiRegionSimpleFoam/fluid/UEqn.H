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
    Modified 2026-10 by Chinmay S Patil: Added Darcy-type mushy-zone drag terms for steady state.
\*---------------------------------------------------------------------------*/

    // Solve the Momentum equation

    MRF.correctBoundaryVelocity(U);

    UEqn =
    (
        fvm::div(phi, U)
      + MRF.DDt(rho, U)
      + turbulence.divDevRhoReff(U)
     ==
        fvOptions(rho, U)
    );

    if (pcmModel.active() && !pcmModel.suppressConvection())
    {
        UEqn += fvm::Sp(pcmModel.momentumSp(), U);
    }

    UEqn.relax();


    fvOptions.constrain(UEqn);

    if (momentumPredictor)
    {
        solve
        (
            UEqn
        ==
            fvc::reconstruct
            (
                (
                - ghf*fvc::snGrad(rho)
                - fvc::snGrad(p_rgh)
                )*mesh.magSf()
            )
        );

        fvOptions.correct(U);
    }
