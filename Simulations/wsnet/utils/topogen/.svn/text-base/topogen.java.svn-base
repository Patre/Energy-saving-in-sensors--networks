import java.util.*;
import java.io.*;


class topogen {

    // Topology Parameters
    int topo_type          = -1;
    double topo_radius     = -1;
    int topo_square_width  = -1;
    int topo_circle_radius = -1;
    int topo_rect_width    = -1;
    int topo_rect_height   = -1;
    int dist_grid_size_x   = -1;
    int dist_grid_size_y   = -1;
    static int dist_grid_border   = 10;
    
    
    // Distribution Parameters
    int dist_type                 = -1;
    double dist_uniform_intensity = -1;
    int dist_static_nbr           = -1;


    // Effect Parameters
    int effect_type                 = -1;
    int effect_hole                 = -1;
    int effect_hole_nbr             = -1;
    double effect_hole_size         = -1;
    int effect_cluster              = -1;
    int effect_cluster_nbr          = -1;
    double effect_cluster_size      = -1;
    double effect_cluster_intensity = -1;
    
    // Output Parameter
    int output             = -1;
    String output_filename = null;

    int output_format          = -1;
    String output_formatstring = null;

    int goutput             = -1;
    String goutput_filename = null;

    int gpreview            = -1;
    String filename_preview = "topology.tmp.log";

    int verbose = 1;
    int zoom    = 1;

    // Graph visualisation
    int mode              = -1; // 1: topology generation; 0: graph visualisation 
    String graph_filename = null;
    int graph_width       = -1;
    int graph_zoom        = -1;


    public void showparameters(){
	
	System.out.println("");
	show(12,"Selected parameters :\n");

	// Show topology
	switch(topo_type){
	case 0 : System.out.println("Topology : square "+topo_square_width+"x"+topo_square_width+", coverage area="+topo_radius); break;
        case 1 : System.out.println("Topology : circle with a radius="+topo_circle_radius+", coverage area="+topo_radius); break;
	case 2 : System.out.println("Topology : rectangle "+topo_rect_width+"x"+topo_rect_height+", coverage area="+topo_radius); break;
	}

	// Show Node distribution
	switch(dist_type){
	case 0 : System.out.println("Distribution : uniform, lambda="+dist_uniform_intensity); break;
	case 1 : System.out.println("Distribution : static, nbr nodes="+dist_static_nbr); break;
	case 2 : System.out.println("Distribution : grid, grid size="+dist_grid_size_x+"x"+dist_grid_size_y); break;
	}


	// Show effect
	switch(effect_type){
	case 0 : System.out.println("Effect : "+effect_hole_nbr+" holes of size "+effect_hole_size); break;
	case 1 : System.out.println("Effect : "+effect_cluster_nbr+" cluster of size "+effect_cluster_size+" with lambda="+effect_cluster_intensity); break;
	}


	// Output file
	if (output==1) System.out.println("Output : "+output_filename);
	if (output_format!=-1) System.out.println("Output format : "+output_formatstring);
	else System.out.println("Output format : default");
	if (goutput==1) System.out.println("Graphic output : "+goutput_filename);
	if (gpreview==1) System.out.println("Graphic preview = yes");
	else System.out.println("Graphic preview = no");

	System.out.println("");
    }


    public static void main(String arg[]) {
	new topogen().start(arg);
    }

    public void copyright(){
	System.out.println("Wireless Network Topology Generator || Copyright Elyes Ben Hamida - 2007.");
    }

    public void usage(){
	System.out.println("\nUsage: wsnet-topogen [options]");
	System.out.println("");
	System.out.println("Possible options :");
	System.out.println("  -s,  --show"+'\t'+'\t'+"[filename] "+"[graph_width] "+"[zoom]"+'\t'+'\t'+"show the graph");
	System.out.println("  -t,  --topo "+'\t'+'\t'+"[type] [parameters]"+'\t'+'\t'+'\t'+"set topology parameters");
	System.out.println("  -d,  --distribution"+'\t'+"[type] [parameters]"+'\t'+'\t'+'\t'+"set node distribution parameters");
	System.out.println("  -e,  --effect"+'\t'+'\t'+"[type] [parameters]"+'\t'+'\t'+'\t'+"add effects to generate irregular topologies");
	System.out.println("  -o,  --output"+'\t'+'\t'+"[filename]"+'\t'+'\t'+'\t'+'\t'+"set output file");
	System.out.println("  -of, --outputformat"+'\t'+"[format]"+'\t'+'\t'+'\t'+'\t'+"set output file format");
	System.out.println("  -go, --goutput"+'\t'+"[filename]"+'\t'+'\t'+'\t'+'\t'+"set graphic output file");
	System.out.println("  -gp, --gpreview"+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+"graphical preview of the generated graph");
	System.out.println("  -gz, --gzoom"+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+"set the graphical preview zoom");
	System.out.println("  -h,  --help"+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+"show help");
	System.out.println("  -nv, --noverbose"+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+"no verbose");
	System.out.println("");
	System.out.println("Topology types and parameters (--topo option) :");
	System.out.println("  s, square"+'\t'+'\t'+"parameters : [square_width] [node_coverage_area]");
        System.out.println("  c, circle"+'\t'+'\t'+"parameters : [circle_radius] [node_coverage_area]");
	System.out.println("  r, rect"+'\t'+'\t'+"parameters : [square_width] [square_height] [node_coverage_area]");
	System.out.println("");
	System.out.println("Distribution types and parameters (--distribution option) :");
	System.out.println("  u, uniform"+'\t'+'\t'+"parameters : [node_density]");
	System.out.println("  s, static"+'\t'+'\t'+"parameters : [node_number]");
	System.out.println("  g, grid"+'\t'+'\t'+"parameters : [grid_size_x] [grid_size_y]");
	System.out.println("");
	System.out.println("Effect types and parameters (--effect option) :");
	System.out.println("  h, hole"+'\t'+'\t'+"parameters : [hole_number] [hole_radius]");
	System.out.println("  c, cluster"+'\t'+'\t'+"parameters : [cluster_number] [cluster_radius] [cluster_node_density]");
	System.out.println("");
	System.out.println("Output format (--outputformat option) :");
	System.out.println("  topogen"+'\t'+'\t'+'\t'+"Output format for topogen with the --show option ");
	System.out.println("  wsnet1"+'\t'+'\t'+'\t'+"Output format for the Worldsens simulator (2006)");
	System.out.println("  wsnet2"+'\t'+'\t'+'\t'+"Output format for the Worldsens simulator (2007)");
	System.out.println("  \"row1#...#rown\""+'\t'+'\t'+"User defined : each row seperated by a \"#\"");
	System.out.println("");
	System.exit(-1);
    }

   public void examples(){
	System.out.println("\nExamples: ");
	System.out.println("");
	System.out.println("*) Square topology of size 500x500, a radio range of 20, and a poisson uniform distribution of intensity 0.01 : ");
	System.out.println("  => wsnet-topogen  --topo square 500 20 --distribution uniform 0.01 -o topology.data -gp");
	System.out.println("");

	System.out.println("*) Circular topology of radius 250, a radio range of 20, 2000 randomly distributed nodes, and 1 random hole of radius 60 : ");
	System.out.println("  => wsnet-topogen  --topo circle 250 20 --distribution static 2000 --effect hole 1 60 -o data.txt -gp");
	System.out.println("");

	System.out.println("*) Rectangular topology of size 600x400 with a radio range of 20, a grid distribution of size 38x18, and 6 random holes of radius 30 : ");
	System.out.println("  => wsnet-topogen  --topo rect 600 300 15 --distribution grid 38 18 -o data.txt --effect hole 6 30 -gp");
	System.out.println("");
	
	System.exit(-1);
    }


    public void show(int color, String msg){
	
	switch(color){
	case 0 : System.out.print("\033[31m"+msg+"\033[0m"); break; // red
	case 1 : System.out.print("\033[32m"+msg+"\033[0m"); break; // green
	case 2 : System.out.print("\033[33m"+msg+"\033[0m"); break; // yellow
	case 3 : System.out.print("\033[34m"+msg+"\033[0m"); break; // blue
	case 4 : System.out.print("\033[35m"+msg+"\033[0m"); break; // magenta
	case 5 : System.out.print("\033[36m"+msg+"\033[0m"); break; // cyan
	case 6 : System.out.print("\033[91m"+msg+"\033[0m"); break; // bright red
	case 7 : System.out.print("\033[92m"+msg+"\033[0m"); break; // bright green
	case 8 : System.out.print("\033[93m"+msg+"\033[0m"); break; // bright yellow
	case 9 : System.out.print("\033[94m"+msg+"\033[0m"); break; // bright blue
	case 10 : System.out.print("\033[95m"+msg+"\033[0m"); break; // bright magenta
	case 11 : System.out.print("\033[96m"+msg+"\033[0m"); break; // bright cyan
	case 12 : System.out.print("\033[97m"+msg+"\033[0m"); break; // bright white

	default: System.out.print(msg);
	}

    }

    public void log(String msg, String msg_error){
	System.out.println("");
	show(6,"[ERROR] ");
	System.out.println(msg+" ("+msg_error+")");
	System.out.println("\nType \"wsnet-topogen --help\" or \"wsnet-topogen --examples\" for help...");
	System.out.println("");
	System.exit(-1);
    }

    public void parse(String [] arg){

	if (arg.length<1) log("Missing arguments !",null);
	
	// Parsing the arguments
	for (int i=0; i<arg.length; i++){
	    
	    // Help
	    if (arg[i].toLowerCase().equals("--help") || arg[i].toLowerCase().equals("-h") || arg[i].toLowerCase().equals("help") ){
		usage();
	    }

	    // Show examples
	    else if (arg[i].toLowerCase().equals("--examples") || arg[i].toLowerCase().equals("-ex")){
		examples();
	    }

	    // Set verbose off
	    else if (arg[i].toLowerCase().equals("--noverbose") || arg[i].toLowerCase().equals("-nv")){
		verbose=0;
	    }

	    // Graph visualisation
	    else if (arg[i].toLowerCase().equals("--show") || arg[i].toLowerCase().equals("-s")){
		if (i+3>arg.length-1) log("Missing graph parameters !",null);
		i++;
		graph_filename=arg[i];
		i++;
		String tmp=arg[i]; // extracting graph width
		try{graph_width=Integer.parseInt(tmp);}
		catch(Exception ex) {log("Invalid graph width : "+tmp+" ! ",ex.toString());}
		i++;
		tmp=arg[i]; // extracting graph zoom
		try{graph_zoom=Integer.parseInt(tmp);}
		catch(Exception ex) {log("Invalid graph zoom : "+tmp+" ! ",ex.toString());}
		mode=0;
		return;
	    }

	    // topology parameters
	    else if (arg[i].toLowerCase().equals("--topo") || arg[i].toLowerCase().equals("-t")){
		if (i==arg.length-1) log("Missing topology parameters !",null);
		i++;
		String tmp=arg[i];
		if (tmp.toLowerCase().equals("square") || tmp.toLowerCase().equals("s")){
		    topo_type=0;
		    if (i+2>arg.length-1) log("Missing square parameters !",null);
		    i++;
		    tmp=arg[i]; // extracting square width
		    try{topo_square_width=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid square width : "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting square coverage
		    try{topo_radius=Double.parseDouble(tmp);}
		    catch(Exception ex) {log("Invalid node coverage area : "+tmp+" ! ",ex.toString());}
		}
		else if (tmp.toLowerCase().equals("circle") || tmp.toLowerCase().equals("c")){
		    topo_type=1;
		    if (i+2>arg.length-1) log("Missing circle parameters !",null);
		    i++;
		    tmp=arg[i]; // extracting circle radius
		    try{topo_circle_radius=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid circle radius : "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting node coverage
		    try{topo_radius=Double.parseDouble(tmp);}
		    catch(Exception ex) {log("Invalid node coverage area : "+tmp+" ! ",ex.toString());}


		}
		else if (tmp.toLowerCase().equals("rect") || tmp.toLowerCase().equals("r")){
		    topo_type=2;
		    if (i+3>arg.length-1) log("Missing rectangle parameters !",null);
		    i++;
		    tmp=arg[i]; // extracting rect width
		    try{topo_rect_width=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid rectangle width : "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting rect height
		    try{topo_rect_height=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid rectangle height : "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting node coverage area
		    try{topo_radius=Double.parseDouble(tmp);}
		    catch(Exception ex) {log("Invalid node coverage area : "+tmp+" ! ",ex.toString());}
		}
		else{
		    log("Undefined topology type : "+tmp+" !",null);
		}
		mode=1;
	    }

	    // Distribution parameters
	    else if (arg[i].toLowerCase().equals("--distribution") || arg[i].toLowerCase().equals("-d")){
		if ((i+2) > arg.length-1) log("Missing distribution parameters !",null);
		i++;
		String tmp=arg[i];
		if (tmp.toLowerCase().equals("uniform") || tmp.toLowerCase().equals("u")){
		    dist_type=0;
		    i++;
		    tmp=arg[i]; // extracting intensity
		    try{dist_uniform_intensity=Double.parseDouble(tmp);}
		    catch(Exception ex) {log("Invalid node density : "+tmp+" ! ",ex.toString());}

		}
		else if (tmp.toLowerCase().equals("static") || tmp.toLowerCase().equals("s")){
		    dist_type=1;
		    i++;
		    tmp=arg[i]; // extracting node nbr
		    try{dist_static_nbr=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid node number : "+tmp+" ! ",ex.toString());}
		}
		else if (tmp.toLowerCase().equals("grid") || tmp.toLowerCase().equals("g")){
		    if (i+2>arg.length-1) log("Missing grid parameters !",null);
		    dist_type=2;
		    i++;
		    tmp=arg[i]; // extracting grid size X
		    try{dist_grid_size_x=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid grid size X : "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting grid size Y
		    try{dist_grid_size_y=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid grid size Y : "+tmp+" ! ",ex.toString());}
		}
		else{
		    log("Undefined distribution type : "+tmp+" !",null);
		}
		mode=1;
	    }

	    // Effect parameters
	    else if (arg[i].toLowerCase().equals("--effect") || arg[i].toLowerCase().equals("-e")){
		
		if (i==arg.length-1) log("Missing effect parameters !",null);
		i++;
		String tmp=arg[i];
		if (tmp.toLowerCase().equals("hole") || tmp.toLowerCase().equals("h")){
		    if (i+2>arg.length-1) log("Missing effect parameters !",null);
		    effect_hole=1;
		    i++;
		    tmp=arg[i]; // extracting hole number
		    try{effect_hole_nbr=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid hole number: "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting hole size
		    try{effect_hole_size=Double.parseDouble(tmp);}
		    catch(Exception ex) {log("Invalid hole size: "+tmp+" ! ",ex.toString());}
		}
		else if (tmp.toLowerCase().equals("cluster") || tmp.toLowerCase().equals("c")){
		    if (i+3>arg.length-1) log("Missing effect parameters !",null);
		    effect_cluster=1;
		    i++;
		    tmp=arg[i]; // extracting cluster number
		    try{effect_cluster_nbr=Integer.parseInt(tmp);}
		    catch(Exception ex) {log("Invalid cluster number: "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting cluster size
		    try{effect_cluster_size=Double.parseDouble(tmp);}
		    catch(Exception ex) {log("Invalid cluster size: "+tmp+" ! ",ex.toString());}
		    i++;
		    tmp=arg[i]; // extracting cluster intensity
		    try{effect_cluster_intensity=Double.parseDouble(tmp);}
		    catch(Exception ex) {log("Invalid cluster intensity: "+tmp+" ! ",ex.toString());}
		}
		else{
		    log("Undefined effect type : "+tmp+" !",null);
		}
		mode=1;
	    }

	    // Output parameter
	    else if (arg[i].toLowerCase().equals("--output") || arg[i].toLowerCase().equals("-o")){
		if ((i+1) > arg.length-1) log("Missing output filename !",null);
		i++;
		output_filename=arg[i];		
		output=1;
		mode=1;
	    }

	    // Output format
	    else if (arg[i].toLowerCase().equals("--outputformat") || arg[i].toLowerCase().equals("-of")){
		if ((i+1) > arg.length-1) log("Missing output format !",null);
		i++;
		output_formatstring=arg[i];
		if (output_formatstring.toLowerCase().equals("wsnet1")) output_format=1;
		else if (output_formatstring.toLowerCase().equals("ns")) output_format=2;
		else if (output_formatstring.toLowerCase().equals("topogen")) output_format=3;
		else output_format=0; // user defined...
		mode=1;
	    }

	    // Graphic output parameter
	    else if (arg[i].toLowerCase().equals("--goutput") || arg[i].toLowerCase().equals("-go")){
		if ((i+1) > arg.length-1) log("Missing graphic output filename !",null);
		i++;
		goutput=1;
		goutput_filename=arg[i];	
		mode=1;
	    }

	    // Preview
	    else if (arg[i].toLowerCase().equals("--gpreview") || arg[i].toLowerCase().equals("-gp")){
		gpreview=1;
		mode=1;
	    }

	    // Preview zoom
	    else if (arg[i].toLowerCase().equals("--gzoom") || arg[i].toLowerCase().equals("-gz")){
		if ((i+1) > arg.length-1) log("Missing zoom !",null);
		i++;
		String tmp=arg[i];
  	        try{zoom=Integer.parseInt(tmp);}
		catch(Exception ex) {log("Invalid zoom : "+tmp+" ! ",ex.toString());}
		mode=1;
	    }

	    // Undefined argument !!!
	    else{
		log("Undefined argument : "+arg[i]+" !", null);
	    }
	   
	}

	// 

	if (dist_type == 2) {


	    if (topo_type == 1) log("Grid works only with square/rectangle topologie !", null);
	    else if (topo_type == 0) {

		double x = topo_square_width - dist_grid_border;
		double y = topo_square_width - dist_grid_border;

		if ( dist_grid_size_x * topo_radius > x ) log("Grid width > topology width (radius="+topo_radius+", topology="+x+"x"+y+") !", null);
		else if ( dist_grid_size_y * topo_radius > y ) log("Grid height > topology height (radius="+topo_radius+", topology="+x+"x"+y+") !", null);
	    }
	    else if (topo_type == 2) {

		double x = topo_rect_width - dist_grid_border;
		double y = topo_rect_height - dist_grid_border;

		if ( dist_grid_size_x * topo_radius > x ) log("Grid width > topology width (radius="+topo_radius+", topology="+x+"x"+y+") !", null);
		else if ( dist_grid_size_y * topo_radius > y ) log("Grid height > topology height (radius="+topo_radius+", topology="+x+"x"+y+") !", null);

	    }

	}
	
    }

    public void start(String arg[]) {
	// Show copyright
	copyright();

	// Parse arguments
	parse(arg);

	if (mode==0) {

	    System.out.println("graph visualisation : "+graph_filename+" "+graph_width+" "+graph_zoom);
   	    graphGenerator g=new graphGenerator(verbose);

	    g.load(graph_filename);
            
	    frame2 f=new frame2(graph_zoom, graph_width, graph_width, verbose, g.getNodes());


	    return;
	}
	
	// Test if all parameters are defined
	if (topo_type==-1) log("Please set the topology parameters !",null);
	else if (dist_type==-1) log ("Please define the node distribution parameters !",null);
	else if (output==-1) log("Please set the output filename !",null);
	else if (goutput!=-1 && gpreview==-1) log("Graphic output is only available with graphic preview option...",null);

	// Show selected parameters
	if (verbose==1) showparameters();

	// Starts graph generation
	graphGenerator g=new graphGenerator(verbose);

	switch(topo_type){
	case 0 : g.setTopology(topo_type, topo_square_width, topo_radius); break;
	case 1 : g.setTopologyc(topo_type, topo_circle_radius, topo_radius); break;
	case 2 : g.setTopology(topo_type, topo_rect_width, topo_rect_height, topo_radius); break;
	}

	switch(dist_type){
	case 0 : g.setDistribution(dist_type,dist_uniform_intensity); break;
	case 1 : g.setDistribution(dist_type,dist_static_nbr); break;
	case 2 : g.setDistribution(dist_type,dist_grid_size_x,dist_grid_size_y); break;
	}

	if (effect_hole!=-1) g.setEffect(effect_hole_nbr, effect_hole_size);

	if (effect_cluster!=-1) g.setEffect(effect_cluster_nbr,effect_cluster_size, effect_cluster_intensity);

	

	g.process();
	
	// save output file
	g.saveGraph(output_format, output_formatstring, output_filename);


	// preview
	
	if (gpreview!=-1) {

            int w=-1;
	    int h=-1;
	    if (topo_type==0) {
		w=topo_square_width;
		h=w;
	    }
	    else if (topo_type==1) {
		w=2*(int)topo_circle_radius;
		h=w;
	    }
	    else {
		w=topo_rect_width;
		h=topo_rect_height;
	    }

            frame2 f=new frame2(zoom, w, h, verbose, g.getNodes());

	    if (goutput!=-1) {
		try {
		    Thread.sleep(1500);
		}catch(Exception ex){
		}
		
		f.savePicture(goutput_filename);

	    }

	}

	System.out.println("");
	System.out.println("DONE !");
	System.out.println("");

    }

}








