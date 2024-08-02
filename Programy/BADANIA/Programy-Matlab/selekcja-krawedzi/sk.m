function mst()
    tic;

    [N, M, edges, total_weight] = loadData('C:\Users\mcmys\OneDrive\Pulpit\magisterka repo\magisterka-repo\Programy\testy\edges100.in');

    W = 0; % suma wag krawędzi w MST
    vs = zeros(M+1, 3); % Struktura wierzchołków: [row, v1, v2]

    edges = sortrows(edges, 3);

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
                edge_count = edge_count + 1;
            end
        end
        i = i + 1;
    end

    execution_time = toc;

    disp(['Suma wag krawedzi w MST: ' num2str(W, '%.2f')]);
    disp(['Total weight of the path: ' num2str(total_weight, '%.2f')]);
    disp(['Czas dzialania programu: ' num2str(execution_time, '%.6f') ' s']);

    save_to_csv(W, execution_time, M, total_weight);
end

function [N, M, edges, total_weight] = loadData(filename)
    fileID = fopen(filename, 'r');
    data = fscanf(fileID, '%d %d', [1, 2]);
    M = data(1);
    N = data(2);
    edges = fscanf(fileID, '%d %d %f', [3, Inf])';
    fclose(fileID);
    total_weight = sum(edges(:, 3));
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

function save_to_csv(weight_sum, duration, vertices, total_weight)
    filename = 'result.csv';
    fileID = fopen(filename, 'a');
    if fileID == -1
        error('Unable to open file to write CSV data.');
    end
    if ftell(fileID) == 0
        fprintf(fileID, 'Weight Sum,Execution Time (s),Vertices,Total Weight\n');
    end
    fprintf(fileID, '%.2f,%.6f,%d,%.2f\n', weight_sum, duration, vertices, total_weight);
    fclose(fileID);
end
