function [final_count] = boyer(haystack, needle, nlength, start_pos, end_pos )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
tic;

temp_count = 0;
final_count = 0;
offset = nlength;

lookup_table = createTable(needle, nlength);

fprintf('Start %d End %d',start_pos, end_pos);
while(start_pos <= end_pos - nlength + 1)
        
    last_char = haystack(start_pos + nlength - 1);
     fprintf('%s ',last_char);
    
    if(last_char == needle(nlength) & strncmpi(haystack(1,start_pos:end_pos), needle, nlength - 1) == 1)
        temp_count = temp_count + 1;
    end
    
   % fprintf('size of start_pos is %d',size(start_pos)); 
   % fprintf(' value of start_pos is %d \n', start_pos);
   % fprintf('size of lookup index is %d \n', lookup_table(last_char));
    %fprintf('lookup index is %s \n',last_char);
   
    
    start_pos = start_pos + lookup_table(last_char);
end

final_count = final_count + temp_count;

btime = toc;
fprintf('total time: %f', btime);
end

