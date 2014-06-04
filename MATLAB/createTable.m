function [lookup_table] = createTable(needle, nlength)

%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
% pre-allocating storage for the look up table

UCHAR_MAX = 255;
bad_char_skip_table = zeros(1, UCHAR_MAX);
bad_char_skip_table(1, 1:UCHAR_MAX) = nlength;

if nlength >= 1
    for i = (1: nlength - 1)
        bad_char_skip_table(needle(i)) = nlength - i;
    end
end
lookup_table = bad_char_skip_table;

end


