% *******************************************************************************
%  map2png    : converts a map into a png file
%
%  author     : elyes ben hamida and guillaume chelius
%  date       : 2007
%
%  parameters :
%     *) file   : input map filename
%
%  usage example : map2png('energymap.0.data')
%
% *******************************************************************************
function res=map2png(file)

%load input file
data=load(file);
x=data(:,2);
y=data(:,3);
z=data(:,5);

width  = ceil(max(x));
height = ceil(max(y));

nbr=size(z);

for i=1:nbr(1)
    z(i)=1-z(i);
end

% perform interpolation
ti_w = 0:0.5:width; 
ti_h = 0:0.5:height; 

[XI,YI] = meshgrid(ti_w,ti_h);
ZI = griddata(x,y,z,XI,YI);

%fullscreen = get(0,'ScreenSize');
%figure('Position',[0 -50 fullscreen(3) fullscreen(4)])

% shows energy map
colormap(jet(256));

% colormap with grey color for printing
%colormap(bone) 

pcolor(XI,YI,ZI);
shading interp;

title('Energy distribution map','FontSize',20);

caxis([0 1]); % MAX/MIN color
axis([0 width 0 height]);

axis off;

colorbar;

set(gcf,'renderer', 'zbuffer'); 
saveas(gcf,(strcat(file,'.png')),'png');
