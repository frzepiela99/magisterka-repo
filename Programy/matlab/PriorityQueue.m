classdef PriorityQueue < handle
    properties
        elements
        priorities
        count
    end

    methods
        function obj = PriorityQueue(capacity)
            obj.elements = zeros(1, capacity);
            obj.priorities = inf(1, capacity);
            obj.count = 0;
        end

        function insert(obj, element, priority)
            obj.count = obj.count + 1;
            obj.elements(obj.count) = element;
            obj.priorities(obj.count) = priority;
        end

        function [element, priority] = extractMin(obj)
            [priority, minIdx] = min(obj.priorities(1:obj.count));
            element = obj.elements(minIdx);
            obj.elements(minIdx) = obj.elements(obj.count);
            obj.priorities(minIdx) = obj.priorities(obj.count);
            obj.count = obj.count - 1;
        end

        function isEmpty = isEmpty(obj)
            isEmpty = obj.count == 0;
        end
    end
end