function dwuopt
    main;
end

function main
    tic; % Rozpocznij pomiar czasu

    [N, M, W, Route, TotalWeight] = Initialization();
    Tweight = calculateInitialWeight(N, W, Route);
    
    [Ptr, Route, Tweight] = Calculations(N, W, Route, Tweight);

    disp('Total weight of the route: ');
    disp(Tweight);

    disp('Route: ');
    disp(Route);

    EdgeSum = 0;
    for i = 1:N-1
        EdgeSum = EdgeSum + W(Route(i), Route(i + 1));
    end
    EdgeSum = EdgeSum + W(Route(N), Route(1)); % Dodaj ostatnią krawędź do sumy

    disp('Sum of all edge lengths in the route: ');
    disp(EdgeSum);

    disp('Sum of all edge weights in the graph: ');
    disp(TotalWeight);

    elapsedTime = toc; % Zakończ pomiar czasu
    disp('Execution time: ');
    disp(elapsedTime * 1000); % Czas w milisekundach
end

function [N, M, W, Route, TotalWeight] = Initialization()
    filename = 'C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\TWOOPT\\edges1000.in';
    fileID = fopen(filename, 'r');
    data = fscanf(fileID, '%d');
    fclose(fileID);
    
    N = data(1);
    M = data(2);
    W = zeros(N, N);
    Route = 1:N;
    TotalWeight = 0;
    
    idx = 3;
    for i = 1:M
        X = data(idx);
        Y = data(idx + 1);
        Temp = data(idx + 2);
        W(X, Y) = Temp;
        W(Y, X) = Temp;
        TotalWeight = TotalWeight + Temp; % Dodaj wagę krawędzi do całkowitej sumy wag
        idx = idx + 3;
    end
    
    % Initialize W diagonal to 0
    for i = 1:N
        W(i, i) = 0;
    end
end

function Tweight = calculateInitialWeight(N, W, Route)
    Tweight = 0;
    for i = 1:N-1
        Tweight = Tweight + W(Route(i), Route(i + 1));
    end
    Tweight = Tweight + W(Route(N), Route(1));
end

function [Ptr, Route, Tweight] = Calculations(N, W, Route, Tweight)
    Ptr = zeros(1, N);
    for I = 1:N-1
        Ptr(Route(I)) = Route(I + 1);
    end
    Ptr(Route(N)) = Route(1);

    while true
        Max = 0;
        I1 = 1;
        for I = 1:N-2
            if I == 1
                Limit = N - 1;
            else
                Limit = N;
            end
            I2 = Ptr(I1);
            J1 = Ptr(I2);
            for J = I + 2:Limit
                J2 = Ptr(J1);
                Max1 = W(I1, I2) + W(J1, J2) - (W(I1, J1) + W(I2, J2));
                if Max1 > Max
                    S1 = I1;
                    S2 = I2;
                    T1 = J1;
                    T2 = J2;
                    Max = Max1;
                end
                J1 = J2;
            end
            I1 = I2;
        end
        if Max > 0
            Ptr(S1) = T1;
            Next = S2;
            Last = T2;
            while true
                Ahead = Ptr(Next);
                Ptr(Next) = Last;
                Last = Next;
                Next = Ahead;
                if Next == T2
                    break;
                end
            end
            Tweight = Tweight - Max;
        else
            break;
        end
    end

    Index = 1;
    for I = 1:N
        Route(I) = Index;
        Index = Ptr(Index);
    end
end
