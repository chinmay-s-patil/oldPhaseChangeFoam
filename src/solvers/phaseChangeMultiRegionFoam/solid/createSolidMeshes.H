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
    Modified 2026-10 by Chinmay S Patil: Created solid meshes.
\*---------------------------------------------------------------------------*/

    const wordList solidNames(rp["solid"]);

    PtrList<fvMesh> solidRegions(solidNames.size());

    forAll(solidNames, i)
    {
        if (args.dryRun() || args.found("dry-run-write"))
        {
            Info
                << "Operating in 'dry-run' mode:"
                << " case will run for 1 time step.  "
                << "All checks assumed OK on a clean exit" << endl;

            FieldBase::allowConstructFromLargerSize = true;

            // Create a simplified 1D mesh and attempt to re-create boundary
            // conditions
            solidRegions.set
            (
                i,
                new simplifiedMeshes::columnFvMesh(runTime, solidNames[i])
            );

            // Stop after 1 iteration of the simplified mesh

            if (args.found("dry-run-write"))
            {
                // Using saWriteNow triggers function objects execute(), write()
                runTime.stopAt(Time::saWriteNow);

                // Make sure mesh gets output to the current time (since
                // instance no longer constant)
                solidRegions[i].setInstance(runTime.timeName());
            }
            else
            {
                // Using saNoWriteNow triggers function objects execute(),
                // but not write()
                runTime.stopAt(Time::saNoWriteNow);
            }

            functionObject::outputPrefix = "postProcessing-dry-run";
        }
        else
        {
            Info<< "Create solid mesh for region " << solidNames[i]
                << " for time = " << runTime.timeName() << nl << endl;

            solidRegions.set
            (
                i,
                new fvMesh
                (
                    IOobject
                    (
                        solidNames[i],
                        runTime.timeName(),
                        runTime,
                        IOobject::MUST_READ
                    )
                )
            );

            // Force calculation of geometric properties to prevent it being
            // done
            // later in e.g. some boundary evaluation
            //(void)solidRegions[i].weights();
            //(void)solidRegions[i].deltaCoeffs();
        }
    }
