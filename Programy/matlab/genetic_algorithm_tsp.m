function genetic_algorithm_tsp()
    % Constants
    POPULATION_SIZE = 100;
    GENERATIONS = 500;
    MUTATION_RATE = 0.1;

    % Load edges from file
    [CITY_COUNT, edges, distanceMatrix] = loadEdges('C:\\Users\\mcmys\\source\\repos\\ConsoleApplication6\\ConsoleApplication6\\edges1000.txt');

    % Initialize population
    population = generatePopulation(POPULATION_SIZE, CITY_COUNT);
    fitness = zeros(POPULATION_SIZE, 1);

    logFile = fopen('genetic_algorithm_log.txt', 'w');
    if logFile == -1
        error('Could not open the log file!');
    end

    startTime = tic;

    for generation = 1:GENERATIONS
        genStartTime = tic;

        for i = 1:POPULATION_SIZE
            fitness(i) = routeDistance(population{i}, distanceMatrix);
        end

        newPopulation = cell(POPULATION_SIZE, 1);
        mutationCount = 0;
        crossoverCount = 0;

        for i = 1:POPULATION_SIZE / 2
            parent1 = tournamentSelection(population, fitness);
            parent2 = tournamentSelection(population, fitness);

            child1 = crossover(parent1, parent2);
            child2 = crossover(parent2, parent1);
            crossoverCount = crossoverCount + 2;

            child1 = mutate(child1, MUTATION_RATE);
            child2 = mutate(child2, MUTATION_RATE);
            mutationCount = mutationCount + 2;

            newPopulation{2*i-1} = child1;
            newPopulation{2*i} = child2;
        end

        for i = 1:POPULATION_SIZE
            fitness(i) = routeDistance(newPopulation{i}, distanceMatrix);
        end

        [~, idx] = sort(fitness);
        population = newPopulation(idx(1:POPULATION_SIZE));

        genTime = toc(genStartTime);

        bestFitness = fitness(1);
        worstFitness = fitness(end);
        avgFitness = mean(fitness);
        stdDevFitness = std(fitness);

        if mod(generation, 10) == 0 || generation == GENERATIONS
            fprintf('Generation %d: Best = %.2f (Time: %.2fs)\n', generation, bestFitness, genTime);
            fprintf(logFile, 'Generation %d: Best = %.2f (Time: %.2fs)\n', generation, bestFitness, genTime);
            fprintf(logFile, 'Worst = %.2f, Average = %.2f, StdDev = %.2f\n', worstFitness, avgFitness, stdDevFitness);
            fprintf(logFile, 'Mutations = %d, Crossovers = %d\n', mutationCount, crossoverCount);
        end
    end

    totalTime = toc(startTime);

    % Find best solution
    bestIndex = 1;
    bestRoute = population{bestIndex};
    bestDistance = routeDistance(bestRoute, distanceMatrix);

    fprintf('Best route: ');
    fprintf(logFile, 'Best route: ');
    fprintf('%d ', bestRoute);
    fprintf(logFile, '%d ', bestRoute);
    fprintf('\nBest distance: %.2f\n', bestDistance);
    fprintf(logFile, '\nBest distance: %.2f\n', bestDistance);
    fprintf('Total execution time: %.2fs\n', totalTime);
    fprintf(logFile, 'Total execution time: %.2fs\n', totalTime);

    fclose(logFile);
end

function [CITY_COUNT, edges, distanceMatrix] = loadEdges(filename)
    fileID = fopen(filename, 'r');
    if fileID == -1
        error('Could not open the file!');
    end

    CITY_COUNT = fscanf(fileID, '%d', 1);
    edgeCount = fscanf(fileID, '%d', 1);
    edges = fscanf(fileID, '%d %d %d', [3 edgeCount])';
    fclose(fileID);

    distanceMatrix = zeros(CITY_COUNT);
    for i = 1:edgeCount
        city1 = edges(i, 1);
        city2 = edges(i, 2);
        distance = edges(i, 3);
        distanceMatrix(city1, city2) = distance;
        distanceMatrix(city2, city1) = distance;
    end
end

function distance = routeDistance(route, distanceMatrix)
    distance = 0;
    for i = 1:length(route) - 1
        distance = distance + distanceMatrix(route(i), route(i + 1));
    end
    distance = distance + distanceMatrix(route(end), route(1));
end

function population = generatePopulation(populationSize, cityCount)
    population = cell(populationSize, 1);
    for i = 1:populationSize
        population{i} = randperm(cityCount);
    end
end

function child = crossover(parent1, parent2)
    n = length(parent1);
    start = randi([1, n]);
    endIdx = start + randi([0, n - start]);

    child = -ones(1, n);
    child(start:endIdx) = parent1(start:endIdx);

    current = 1;
    for i = 1:n
        if isempty(find(child == parent2(i), 1))
            while child(current) ~= -1
                current = current + 1;
            end
            child(current) = parent2(i);
        end
    end
end

function mutatedIndividual = mutate(individual, mutationRate)
    mutatedIndividual = individual;
    if rand < mutationRate
        i = randi(length(individual));
        j = randi(length(individual));
        temp = mutatedIndividual(i);
        mutatedIndividual(i) = mutatedIndividual(j);
        mutatedIndividual(j) = temp;
    end
end

function bestIndividual = tournamentSelection(population, fitness)
    tournamentSize = 5;
    best = randi(length(population));
    for i = 1:tournamentSize - 1
        contender = randi(length(population));
        if fitness(contender) < fitness(best)
            best = contender;
        end
    end
    bestIndividual = population{best};
end
