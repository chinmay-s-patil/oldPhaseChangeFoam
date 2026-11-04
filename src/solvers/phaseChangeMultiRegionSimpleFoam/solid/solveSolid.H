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
    Modified 2026-10 by Chinmay S Patil: Added steady-state solid phase change energy solution.
\*---------------------------------------------------------------------------*/

{
    if (pcModel.active())
    {
        pcModel.correct();
    }

    tmp<volScalarField> tkEff = pcModel.active() ? pcModel.k() : tmp<volScalarField>(thermo.kappa());
    tmp<volScalarField> tCpThermo = thermo.Cp();
    tmp<volScalarField> trhoEff = pcModel.active() ? pcModel.rhoEff() : tmp<volScalarField>(thermo.rho());
    volScalarField alphaEff("alphaEff", tkEff() / tCpThermo());

    for (int nonOrth=0; nonOrth<=nNonOrthCorr; ++nonOrth)
    {
        fvScalarMatrix hEqn
        (
            (
                thermo.isotropic()
              ? -fvm::laplacian(betav*alphaEff, h, "laplacian(alpha,h)")
              : -fvm::laplacian(betav*taniAlpha(), h, "laplacian(alpha,h)")
            )
          ==
            fvOptions(trhoEff(), h)
        );

        if (pcModel.active())
        {
            hEqn += betav * pcModel.latentHeatSource()
                  + fvm::Sp(betav * pcModel.latentHeatSp(), h)
                  - betav * pcModel.latentHeatSp() * h;
        }

        hEqn.relax();

        fvOptions.constrain(hEqn);

        if (coupled)
        {
            fvMatrixAssemblyPtr->addFvMatrix(hEqn);
            break;
        }
        else
        {
            Info<< "\nSolving for solid region "
                << solidRegions[i].name() << endl;

            hEqn.solve();
            fvOptions.correct(h);
            thermo.correct();

            if (pcModel.active())
            {
                pcModel.postEnergySolve(h);
                thermo.correct();
                pcModel.applyKappaToThermo();
            }

            Info<< "Min/max T:" << min(thermo.T()).value() << ' '
                << max(thermo.T()).value() << endl;
        }
    }
}

