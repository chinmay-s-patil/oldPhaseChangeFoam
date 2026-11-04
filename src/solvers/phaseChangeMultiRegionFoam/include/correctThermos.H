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
    Modified 2026-10 by Chinmay S Patil: Updated thermophysical properties accounting for phase change.
\*---------------------------------------------------------------------------*/

forAll(fluidRegions, i)
{
    rhoThermo& thermo = thermoFluid[i];
    radiation::radiationModel& rad = radiation[i];
    fv::options& fvOptions = fluidFvOptions[i];
    volScalarField& he = thermo.he();
    fvOptions.correct(he);
    thermo.correct();
    phaseChangeModel& pcmModel = pcmModelsFluid[i];
    if (pcmModel.active())
    {
        pcmModel.correct();
        pcmModel.postEnergySolve(he, finalIter);
        thermo.correct();
        pcmModel.applyKappaToThermo();
    }
    rad.correct();
}

forAll(solidRegions, i)
{
    solidThermo& thermo = thermos[i];
    fv::options& fvOptions = solidHeatSources[i];
    volScalarField& h = thermo.he();
    fvOptions.correct(h);
    thermo.correct();
    phaseChangeModel& pcModel = phaseChangeModels[i];
    if (pcModel.active())
    {
        pcModel.correct();
        pcModel.postEnergySolve(h, finalIter);
        thermo.correct();
        pcModel.applyKappaToThermo();
    }
}



