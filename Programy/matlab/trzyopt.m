function main
    startTime = tic; % Start measuring time

    % Initialization
    [N, M, W, Route, TotalWeight] = Initialization();

    % Calculations
    [Tweight, Route] = Calculations(N, W, Route);

    % Display results
    fprintf('Total weight of the route: %d\n', Tweight);
    fprintf('Route: ');
    fprintf('%d ', Route);
    fprintf('\n');

    EdgeSum = 0;
    for i = 1:N-1
        EdgeSum = EdgeSum + W(Route(i), Route(i+1));
    end
    EdgeSum = EdgeSum + W(Route(N), Route(1)); % Add the last edge

    fprintf('Sum of all edge lengths in the route: %d\n', EdgeSum);
    fprintf('Sum of all edge weights in the graph: %d\n', TotalWeight);

    duration = toc(startTime); % End measuring time
    fprintf('Execution time: %.2f ms\n', duration * 1000);
end

function [N, M, W, Route, TotalWeight] = Initialization()
    fid = fopen('C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\THREEOPT\\edges300.in', 'r');
    if fid == -1
        error('File not found');
    end
    
    data = fscanf(fid, '%d %d', 2);
    N = data(1);
    M = data(2);
    
    W = zeros(N, N);
    Route = 1:N;
    TotalWeight = 0;
    
    for i = 1:M
        edge = fscanf(fid, '%d %d %d', 3);
        X = edge(1);
        Y = edge(2);
        Temp = edge(3);
        W(X, Y) = Temp;
        W(Y, X) = Temp;
        TotalWeight = TotalWeight + Temp; % Add edge weight to total sum
    end
    
    fclose(fid);
end

function [Tweight, Route] = Calculations(N, W, Route)
    Ptr = zeros(1, N);
    for i = 1:N-1
        Ptr(Route(i)) = Route(i+1);
    end
    Ptr(Route(N)) = Route(1);
    
    Tweight = sum(W(sub2ind(size(W), Route(1:end-1), Route(2:end))));
    Tweight = Tweight + W(Route(N), Route(1));
    
    BestSwap.Gain = 0;
    BestSwap.X1 = 0; BestSwap.X2 = 0;
    BestSwap.Y1 = 0; BestSwap.Y2 = 0;
    BestSwap.Z1 = 0; BestSwap.Z2 = 0;
    BestSwap.Choice = '';
    
    while true
        BestSwap.Gain = 0;
        Swap.X1 = 1;
        for i = 1:N
            Swap.X2 = Ptr(Swap.X1);
            Swap.Y1 = Swap.X2;
            for j = 2:N-3
                Swap.Y2 = Ptr(Swap.Y1);
                Swap.Z1 = Ptr(Swap.Y2);
                for k = j+2:N-1
                    Swap.Z2 = Ptr(Swap.Z1);
                    Swap.Gain = 0;
                    Swap.Choice = '';
                    Swap = SwapCheck(Swap, W);
                    if Swap.Gain > BestSwap.Gain
                        BestSwap = Swap;
                    end
                    Swap.Z1 = Swap.Z2;
                end
                Swap.Y1 = Swap.Y2;
            end
            Swap.X1 = Swap.X2;
        end
        
        if BestSwap.Gain <= 0
            break;
        end
        
        if BestSwap.Choice == "Asym"
            Ptr = Reverse(Ptr, BestSwap.Z2, BestSwap.X1);
            Ptr(BestSwap.Y1) = BestSwap.X1;
            Ptr(BestSwap.Z2) = BestSwap.Y2;
        else
            Ptr(BestSwap.X1) = BestSwap.Y2;
            Ptr(BestSwap.Y1) = BestSwap.Z2;
        end
        Ptr(BestSwap.Z1) = BestSwap.X2;
        Tweight = Tweight - BestSwap.Gain;
    end
    
    Index = 1;
    for i = 1:N
        Route(i) = Index;
        Index = Ptr(Index);
    end
end

function Swap = SwapCheck(Swap, W)
    DelWeight = W(Swap.X1, Swap.X2) + W(Swap.Y1, Swap.Y2) + W(Swap.Z1, Swap.Z2);
    
    MaxGain = DelWeight - (W(Swap.Y1, Swap.X1) + W(Swap.Z1, Swap.X2) + W(Swap.Z2, Swap.Y2));
    if MaxGain > Swap.Gain
        Swap.Gain = MaxGain;
        Swap.Choice = "Asym";
    end
    
    MaxGain = DelWeight - (W(Swap.X1, Swap.Y2) + W(Swap.Z1, Swap.X2) + W(Swap.Y1, Swap.Z2));
    if MaxGain > Swap.Gain
        Swap.Gain = MaxGain;
        Swap.Choice = "Sym";
    end
end

function Ptr = Reverse(Ptr, start, finish)
    if start ~= finish
        last = start;
        next = Ptr(last);
        while last ~= finish
            ahead = Ptr(next);
            Ptr(next) = last;
            last = next;
            next = ahead;
        end
    end
end
