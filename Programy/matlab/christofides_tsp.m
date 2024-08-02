function christofides_tsp()
    % Specify the input file path
    filename = 'C:\Users\mcmys\OneDrive\Pulpit\magisterka repo\magisterka-repo\Programy\BADANIA\Dane_wejsciowe\edges300.in';

    % Read the input file
    fileID = fopen(filename, 'r');
    if fileID == -1
        error('Cannot open the file.');
    end

    V = fscanf(fileID, '%d', 1);
    E = fscanf(fileID, '%d', 1);
    adjMatrix = zeros(V, V);

    for i = 1:E
        u = fscanf(fileID, '%d', 1);
        v = fscanf(fileID, '%d', 1);
        w = fscanf(fileID, '%f', 1);
        adjMatrix(u, v) = w;
        adjMatrix(v, u) = w;
    end
    fclose(fileID);

    % Start the timer
    tic;

    % Compute the MST using Prim's algorithm
    [mst, mstCost] = primMST(adjMatrix, V);

    % Find odd degree vertices in MST
    oddDegreeVertices = findOddDegreeVertices(mst, V);

    % Find minimum weight matching for odd degree vertices
    matching = minWeightMatching(adjMatrix, oddDegreeVertices);

    % Add matching edges to the MST to create an Eulerian graph
    for i = 1:size(matching, 1)
        u = matching(i, 1);
        v = matching(i, 2);
        mst(u, v) = adjMatrix(u, v);
        mst(v, u) = adjMatrix(u, v);
    end

    % Find an Eulerian cycle
    eulerianCycle = findEulerianCycle(mst, V);

    % Convert the Eulerian cycle to a Hamiltonian cycle
    [hamiltonianCycle, tourCost] = eulerianToHamiltonian(eulerianCycle, adjMatrix, V);

    % Stop the timer
    duration = toc;

    % Display the results
    fprintf('Znaleziona trasa: ');
    for i = 1:length(hamiltonianCycle)
        fprintf('%d ', hamiltonianCycle(i));
    end
    fprintf('%d\n', hamiltonianCycle(1)); % Return to the starting vertex
    fprintf('Suma wag krawedzi trasy: %.2f\n', tourCost);
    fprintf('Suma wag krawedzi w MST: %.2f\n', mstCost);
    fprintf('Czas dzialania programu: %.6f sekund\n', duration);

    % Save the results to a file
    saveResults(hamiltonianCycle, tourCost, mstCost, duration);
    % Save the results to a CSV file
    saveResultsToCSV(hamiltonianCycle, tourCost, mstCost, duration);
end

function [mst, mstCost] = primMST(adjMatrix, V)
    mst = zeros(V, V);
    key = inf(1, V);
    parent = -ones(1, V);
    inMST = false(1, V);

    key(1) = 0;

    for count = 1:V-1
        minKey = inf;
        u = -1;

        for v = 1:V
            if ~inMST(v) && key(v) < minKey
                minKey = key(v);
                u = v;
            end
        end

        inMST(u) = true;

        for v = 1:V
            if adjMatrix(u, v) && ~inMST(v) && adjMatrix(u, v) < key(v)
                parent(v) = u;
                key(v) = adjMatrix(u, v);
            end
        end
    end

    mstCost = 0;
    for i = 2:V
        mst(parent(i), i) = adjMatrix(parent(i), i);
        mst(i, parent(i)) = adjMatrix(parent(i), i);
        mstCost = mstCost + adjMatrix(parent(i), i);
    end
end

function oddDegreeVertices = findOddDegreeVertices(mst, V)
    oddDegreeVertices = [];
    for i = 1:V
        degree = sum(mst(i, :) ~= 0);
        if mod(degree, 2) == 1
            oddDegreeVertices = [oddDegreeVertices, i];
        end
    end
end

function matching = minWeightMatching(adjMatrix, oddDegreeVertices)
    n = length(oddDegreeVertices);
    visited = false(1, n);
    matching = [];

    for i = 1:n
        if ~visited(i)
            u = oddDegreeVertices(i);
            minWeight = inf;
            minVertex = -1;

            for j = 1:n
                if i ~= j && ~visited(j) && adjMatrix(u, oddDegreeVertices(j)) < minWeight
                    minWeight = adjMatrix(u, oddDegreeVertices(j));
                    minVertex = j;
                end
            end

            visited(minVertex) = true;
            matching = [matching; u, oddDegreeVertices(minVertex)];
        end
    end
end

function eulerianCycle = findEulerianCycle(mst, V)
    eulerianCycle = [];
    stack = [1];
    adjMatrix = mst;

    while ~isempty(stack)
        u = stack(end);
        hasUnvisited = false;

        for v = 1:V
            if adjMatrix(u, v) ~= 0
                stack = [stack, v];
                adjMatrix(u, v) = 0;
                adjMatrix(v, u) = 0;
                hasUnvisited = true;
                break;
            end
        end

        if ~hasUnvisited
            eulerianCycle = [eulerianCycle, u];
            stack(end) = [];
        end
    end
end

function [hamiltonianCycle, tourCost] = eulerianToHamiltonian(eulerianCycle, adjMatrix, V)
    hamiltonianCycle = [];
    visitedHamiltonian = false(1, V);

    for v = eulerianCycle
        if ~visitedHamiltonian(v)
            hamiltonianCycle = [hamiltonianCycle, v];
            visitedHamiltonian(v) = true;
        end
    end

    tourCost = 0;
    for i = 1:length(hamiltonianCycle)-1
        tourCost = tourCost + adjMatrix(hamiltonianCycle(i), hamiltonianCycle(i+1));
    end
    tourCost = tourCost + adjMatrix(hamiltonianCycle(end), hamiltonianCycle(1));
end

function saveResults(path, totalCost, mstCost, duration)
    fileID = fopen('results.txt', 'w');
    fprintf(fileID, 'Path: ');
    for i = 1:length(path)
        fprintf(fileID, '%d ', path(i));
    end
    fprintf(fileID, '\n');
    fprintf(fileID, 'Total cost of the tour: %.2f\n', totalCost);
    fprintf(fileID, 'Total cost of the MST: %.2f\n', mstCost);
    fprintf(fileID, 'Execution time: %.6f seconds\n', duration);
    fclose(fileID);
end

function saveResultsToCSV(path, totalCost, mstCost, duration)
    fileID = fopen('results.csv', 'w');
    fprintf(fileID, 'Vertex\n');
    for i = 1:length(path)
        fprintf(fileID, '%d\n', path(i));
    end
    fprintf(fileID, 'Total cost of the tour,%.2f\n', totalCost);
    fprintf(fileID, 'Total cost of the MST,%.2f\n', mstCost);
    fprintf(fileID, 'Execution time (seconds),%.6f\n', duration);
    fclose(fileID);
end
