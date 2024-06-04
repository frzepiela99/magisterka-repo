function mst()
    tic; % Start timing

    % Load the data
    [N, M, edges] = loadData('C:\\Users\\mcmys\\source\\repos\\ConsoleApplication6\\ConsoleApplication6\\edges1000.txt');

    % Initialize variables
    W = 0; % suma wag krawędzi w MST
    L = 0; % suma długości krawędzi w MST
    vs = zeros(M+1, 3); % Vertex structure: [row, v1, v2]

    % Sort edges by weight
    edges = sortrows(edges, 3);

    % Initialize graph data for visualization
    edge_list = [];
    edge_weights = [];

    % Build the MST
    edge_count = 0;
    i = 1;
    while edge_count < M
        t = edges(i, :);
        if vs(t(1), 1) < 2 && vs(t(2), 1) < 2
            if nocycle(vs, t)
                disp([num2str(t(3)) ' ' num2str(t(1)) ' ' num2str(t(2)) ' II ' num2str(edge_count + 1)]);
                vs(t(1), 1) = vs(t(1), 1) + 1;
                vs(t(2), 1) = vs(t(2), 1) + 1;
                if vs(t(1), 2) == 0
                    vs(t(1), 2) = t(2);
                else
                    vs(t(1), 3) = t(2);
                end
                if vs(t(2), 2) == 0
                    vs(t(2), 2) = t(1);
                else
                    vs(t(2), 3) = t(1);
                end
                W = W + t(3);
                L = L + abs(t(1) - t(2));
                edge_count = edge_count + 1;
                
                % Add edge to graph data
                edge_list = [edge_list; t(1), t(2)];
                edge_weights = [edge_weights; t(3)];
            end
        end
        i = i + 1;
    end

    % Output results
    disp(['Suma wag krawedzi w MST: ' num2str(W)]);
    disp(['Suma dlugosci krawedzi w MST: ' num2str(L)]);
    disp(['Czas dzialania programu: ' num2str(toc) ' s']);

    % Visualize the MST
    G = graph(edge_list(:,1), edge_list(:,2), edge_weights);
    figure;
    p = plot(G, 'Layout', 'force'); % Use force layout for better node distribution
    title('Minimalne Drzewo Rozpinające (MST)');

    % Customize the plot
    p.NodeColor = 'r';
    p.EdgeColor = 'g';
    p.MarkerSize = 5;
    p.LineWidth = 1.5;
    p.NodeLabel = {}; % Remove node labels to reduce clutter
    highlight(p, 'Edges', 1:size(edge_list, 1), 'EdgeColor', 'b', 'LineWidth', 1.5); % Highlight edges for better visibility
end

function [N, M, edges] = loadData(filename)
    fileID = fopen(filename, 'r');
    data = fscanf(fileID, '%d %d', [1, 2]);
    M = data(1);
    N = data(2);
    edges = fscanf(fileID, '%d %d %f', [3, Inf])';
    fclose(fileID);
end

function cycle = nocycle(vs, e)
    v1 = e(1);
    v2 = e(2);
    v1_root = findroot(vs, v1);
    v2_root = findroot(vs, v2);
    cycle = (v1_root ~= v2_root);
end

function root = findroot(vs, v)
    while vs(v, 1) == 2
        if vs(v, 2) ~= 0
            v = vs(v, 2);
        else
            v = vs(v, 3);
        end
    end
    root = v;
end
