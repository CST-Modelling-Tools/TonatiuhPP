// --------------------------------------------------------------------
// This file is part of libDistMesh.
//
// libDistMesh is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// libDistMesh is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libDistMesh. If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2015 Patrik Gebhardt
// Contact: patrik.gebhardt@rub.de
// --------------------------------------------------------------------

#pragma once

namespace distmesh {
namespace constants {
    // algorithm stops, when maximum relative points movement is below threshold
//    static double const pointsMovementThreshold = 1e-3;
    static double const pointsMovementThreshold = 1e-2;

    // triangulation is updated, when maximum relative points movement is above threshold
    static double const retriangulationThreshold = 1e-1;

    // relative threshold in the geometry evaluations
//    static double const geometryEvaluationThreshold = 1e-3;
    static double const geometryEvaluationThreshold = 1e-2;
//    static double const geometryEvaluationThresholdInit = 0.3;
//    static double const geometryEvaluationThresholdInit = 0.5;

    // time step for updating points positions with Euler's method
//    static double const deltaT = 1e-1;
    static double const deltaT = 0.2;

    // step size for numerical differentiation
//    static double const deltaX = std::sqrt(std::numeric_limits<double>::epsilon());
    static double const deltaX = 1e-6;

    // algorithm will be terminated after the maximum number of iterations,
    // when no convergence can be achieved
//    static unsigned const maxSteps = 10000;
    static unsigned const maxSteps = 200;
}
}
