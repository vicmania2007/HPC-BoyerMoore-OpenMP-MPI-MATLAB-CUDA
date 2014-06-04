
function [matches] = stringmatch_seq()
tic;

string2search = 'gttggta';  % you can use 'the king' or 'wonderland' also
num_tasks = 1;

% needle is the string to be searched in the haystack(text)
needle = string2search;

% Get the length of the string to be searched
nlength = length(needle); 

% open the text file and read it
fid = fopen('gene.txt', 'rt');
haystack = fscanf(fid,'%c');
fclose(fid);

% Get the length of the text
hlength = length(haystack);

[startValues, endValues] = splitdata(hlength, num_tasks);

% create the job 
%parallel.defaultClusterProfile('discovery_local_r2013b');
%cluster = parcluster('discovery_local_r2013b');
%job1 = createJob(cluster);

%p = 'PATH_TO_LOCAL_DIRECTORY_OF_FUNCTIONS';
%set(job1,'FileDependencies',{p});

% create the tasks
for index = 1: num_tasks
    start_pos = startValues(index);
    end_pos = endValues(index);
    %ret = createTask(job1,@boyer,1,{haystack,needle,nlength,start_pos,end_pos});

    [final_count] = boyer(haystack, needle, nlength, start_pos, end_pos);
    %disp(nargin);
    %fprintf('For index %d the createTask value is\n',index); 
    %disp(class(ret));
    %disp(ret); 
end

%matches = final_count;

%disp(final_count);

% Submit and wait for the results
%submit(job1);
%wait(job1);

% Report the number of matches
%results = fetchOutputs(job1);

matches = final_count;

fprintf('%d',matches);

%finishjobs=findJob(c,'State','finished','Username','<put your username here>');
%delete(job1);

wtime = toc;
fprintf('total time: %f', wtime);

end


function [startValues, endValues] = splitdata(hlength, num_tasks)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
% Divide the number of elements by the number of tasks
numElemPerTask = repmat(floor(hlength/num_tasks), 1, num_tasks);
reminder = rem(hlength, num_tasks);

numElemPerTask(1:reminder) = numElemPerTask(1:reminder) + 1;
endValues = cumsum(numElemPerTask);

startValues = [1 endValues(1:end-1) + 1];
end



