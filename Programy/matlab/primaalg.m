function primalg()
    % Read the graph from a file
    filename = 'C:\Users\mcmys\source\repos\ConsoleApplication6\ConsoleApplication6\edges1000.txt';
    fileID = fopen(filename, 'r');
    if fileID == -1
        error('Unable to open file');
    end

    V = fscanf(fileID, '%d', 1);
    E = fscanf(fileID, '%d', 1);

    graph = zeros(V, V);
    totalWeight = 0;

    while ~feof(fileID)
        u = fscanf(fileID, '%d', 1);
        v = fscanf(fileID, '%d', 1);
        w = fscanf(fileID, '%d', 1);
        if isempty([u, v, w])
            break;
        end
        graph(u, v) = w;
        graph(v, u) = w; % Assuming the graph is undirected
        totalWeight = totalWeight + w;
    end
    fclose(fileID);

    % Display the graph matrix for debugging
    disp('Graph matrix:');
    disp(graph);

    % Measure the execution time
    tic;
    [path, mstWeight] = approxTSP(graph);
    duration = toc;

    % Calculate the total length of the TSP path
    [totalLength, edgeLengths] = calculatePathLength(graph, path);
    isPathValid = verifyPath(path, V);

    % Display results
    fprintf('Approximate TSP path: ');
    fprintf('%d ', path); % Display 1-indexed path for MATLAB
    fprintf('\n\n');

    fprintf('Edge lengths: ');
    fprintf('%d ', edgeLengths);
    fprintf('\n\n');

    fprintf('Total length of the approximate TSP path: %d\n', totalLength);
    fprintf('\n\n');
    fprintf('Total weight of the MST: %d\n', mstWeight);
    fprintf('\n\n');
    fprintf('Execution time: %.2f seconds\n', duration);
    fprintf('\n\n');
    fprintf('Path validity: %s\n', string(isPathValid));

    pathLength = numel(path) - 1; % Subtract 1 to get the number of edges
    if pathLength == V
        pathLength = pathLength - 1; % Adjust for the additional return to start
    end
    fprintf('Path length (number of edges): %d\n', pathLength);

    % Save results to a file
    saveResults(path, totalLength, edgeLengths, mstWeight, duration, pathLength);
end

function [parent, mstWeight] = primMST(graph)
    V = size(graph, 1);
    parent = -ones(1, V);
    key = inf(1, V);
    inMST = false(1, V);

    key(1) = 0;
    pq = PriorityQueue(V);
    pq.insert(1, 0); % Initialize with vertex 1

    mstWeight = 0; % Initialize total weight of MST

    while ~pq.isEmpty()
        [u, ~] = pq.extractMin();
        if inMST(u)
            continue;
        end
        inMST(u) = true;

        if parent(u) ~= -1
            mstWeight = mstWeight + graph(u, parent(u));
        end

        for v = 1:V
            if graph(u, v) && ~inMST(v) && graph(u, v) < key(v)
                key(v) = graph(u, v);
                pq.insert(v, key(v));
                parent(v) = u;
            end
        end
    end
end

function path = preorderTraversal(mst, u, visited, path)
    visited(u) = true;
    path(end + 1) = u;
    for v = mst{u}
        if ~visited(v)
            path = preorderTraversal(mst, v, visited, path);
        end
    end
end

function [path, mstWeight] = approxTSP(graph)
    [parent, mstWeight] = primMST(graph);
    V = size(graph, 1);
    mst = cell(1, V);

    for i = 2:V
        mst{parent(i)} = [mst{parent(i)}, i];
        mst{i} = [mst{i}, parent(i)];
    end

    visited = false(1, V);
    path = [];
    path = preorderTraversal(mst, 1, visited, path);
    path(end + 1) = 1; % Close the cycle

    % Convert path to 1-indexed for compatibility with MATLAB
    path = path;
end

function [totalLength, edgeLengths] = calculatePathLength(graph, path)
    totalLength = 0;
    edgeLengths = zeros(1, numel(path) - 1);
    for i = 1:numel(path) - 1
        length = graph(path(i), path(i + 1)); % No need for adjustment
        fprintf('Edge from %d to %d: length = %d\n', path(i), path(i + 1), length);
        totalLength = totalLength + length;
        edgeLengths(i) = length;
    end
    fprintf('Total calculated length: %d\n', totalLength);
end

function isValid = verifyPath(path, numVertices)
    visitedVertices = false(1, numVertices);
    for v = path
        visitedVertices(v) = true; % No need for adjustment
    end
    isValid = all(visitedVertices);
end

function saveResults(path, totalLength, edgeLengths, mstWeight, duration, pathLength)
    fileID = fopen('results.txt', 'w');
    fprintf(fileID, 'Path: ');
    fprintf(fileID, '%d ', path); % Display 1-indexed path for MATLAB
    fprintf(fileID, '\n');

    fprintf(fileID, 'Edge lengths: ');
    fprintf(fileID, '%d ', edgeLengths);
    fprintf(fileID, '\n');

    fprintf(fileID, 'Total length of the approximate TSP path: %d\n', totalLength);
    fprintf(fileID, 'Total weight of the MST: %d\n', mstWeight);
    fprintf(fileID, 'Execution time: %.2f seconds\n', duration);
    fprintf(fileID, 'Path length (number of edges): %d\n', pathLength);
    fclose(fileID);
end