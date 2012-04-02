% *******************************************************************************
%  maps2video : converts maps into an avi file
%
%  author     : elyes ben hamida and guillaume chelius
%  date       : 2007
%
%  parameters :
%     *) directory  : directory containing energy maps
%     *) fileprefix : directory containing energy maps
%     *) avifile    : output avi filename
%     *) starttime  : start time in ms
%     *) endtime    : end time in ms
%     *) period     : period of logging in ms
%
%  usage example : maps2video('./log/','energymap','video.avi',0,50000,1000)
%
%  to convert the avi to gif type : ffmpeg -i video.avi -pix_fmt rgb24 video.gif
%
% *******************************************************************************

function res=maps2video(directory, prefix, avifile, starttime, endtime, period)

k=1;

for j=starttime:period:endtime
    
    filename=strcat(directory,prefix);
    filename=strcat(filename,'.');
    filename=strcat(filename,int2str(j));
    filename=strcat(filename,'.data');
    
    disp(sprintf('Analyzing : %s',filename));
    
    data=load(filename);
    x=data(:,2);
    y=data(:,3);
    z=data(:,5);

    width  = ceil(max(x));
    height = ceil(max(y));

    nbr=size(z);

    for i=1:nbr(1)
        z(i)=1-z(i);
    end

    ti = 0:0.5:width; 
    tj = 0:0.5:height; 
    [XI,YI] = meshgrid(ti,tj);
    ZI = griddata(x,y,z,XI,YI);

    colormap(jet(256));

    pcolor(XI,YI,ZI);
    shading interp;
    
    caxis([0 1]);
    axis([0 width 0 height]);
    axis ij;

    axis off;

    M(k) = getframe;
    k = k + 1;
end


disp(sprintf('Generating %s...',avifile));
movie2avi(M, avifile, 'fps', 30)

disp(sprintf('Done ! \n',avifile));

end
