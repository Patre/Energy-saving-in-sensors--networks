import java.io.*;
import java.util.*;

class graphGenerator {

    private nodes nodes_list;
    private random rd;
    private int verbose=1;

    // Topology Parameters
    int topo_type=-1;
    double topo_radius=-1;
    int topo_square_width=-1;
    int topo_circle_radius=-1;
    int topo_rect_width=-1;
    int topo_rect_height=-1;
    int dist_grid_size_x   = -1;
    int dist_grid_size_y   = -1;

    // Distribution Parameters
    int dist_type=-1;
    double dist_uniform_intensity=-1;
    int dist_static_nbr=-1;

    // Effect Parameters
    int effect_type=-1;

    int effect_hole=-1;
    int effect_hole_nbr=-1;
    double effect_hole_size=-1;

    int effect_cluster=-1;
    int effect_cluster_nbr=-1;
    double effect_cluster_size=-1;
    double effect_cluster_intensity=-1;

    
    // ** Constructor **

    public graphGenerator(int verbose){
	this.verbose=verbose;
    }

    public nodes getNodes() {
	return nodes_list;
    }

    // ** Setting topology parameters **

    public void setTopology(int type, int width, double radius){
	topo_type=type;
	topo_square_width=width;
	topo_radius=radius;
    }


    public void setTopology(int type, int width, int height, double radius){
	topo_type=type;
	topo_rect_width=width;
	topo_rect_height=height;
	topo_radius=radius;
    }

    public void setTopologyc(int type, int r, double radius){
	topo_type=type;
	topo_radius=radius;
	topo_circle_radius=r;
    }


    // ** Setting node distribution  **

    public void setDistribution(int type, double intensity){
	dist_type=type;
	dist_uniform_intensity=intensity;
    }

    public void setDistribution(int type, int nbr){
	dist_type=type;
	dist_static_nbr=nbr;
    }
    public void setDistribution(int type, int grid_x, int grid_y){
	dist_type        = type;
	dist_grid_size_x = grid_x;
	dist_grid_size_y = grid_y;
    }


    // ** Setting effect  **

    public void setEffect(int nbr, double size){
	effect_type=1;
	effect_hole=1;
	effect_hole_nbr=nbr;
	effect_hole_size=size;
    }

    public void setEffect(int nbr, double size, double intensity){
	effect_type=1;
	effect_cluster=1;
	effect_cluster_nbr=nbr;
	effect_cluster_size=size;
	effect_cluster_intensity=intensity;
    }

    // ** Misc for logging and text color **

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
	System.out.println("");
    }
    
    // ** Graph visualisation  **

    public void load(String filename){
	try{
	    System.out.println(filename);

	    // Evaluating nodes number
	    BufferedReader lecteurAvecBuffer = new BufferedReader(new FileReader(filename));
	    String ligne=null;
	    int i=0;
	    while ((ligne = lecteurAvecBuffer.readLine()) != null) {
		i++;
	    }
	    lecteurAvecBuffer.close();
	    
	    int node_nbr=i;
	    nodes_list=new nodes(node_nbr,verbose);

	    
	    // Reading nodes id
	    lecteurAvecBuffer = new BufferedReader(new FileReader(filename));
	    ligne=null;
	    i=0;
	    while ((ligne = lecteurAvecBuffer.readLine()) != null) {
		StringTokenizer t=new StringTokenizer(ligne," ");
		int id=Integer.parseInt(t.nextToken());
		int x=(int)Double.parseDouble(t.nextToken());
		int y=(int)Double.parseDouble(t.nextToken());
		nodes_list.addNode(id,x,y);
	    }
	    lecteurAvecBuffer.close();

	    // Computing nodes edges

	    nodes_list.refresh();

	    lecteurAvecBuffer = new BufferedReader(new FileReader(filename));
	    ligne=null;
	    while ((ligne = lecteurAvecBuffer.readLine()) != null) {
		StringTokenizer t=new StringTokenizer(ligne," ");
		int id=Integer.parseInt(t.nextToken());
		t.nextToken();// x position
		t.nextToken();// y position
		while(t.hasMoreTokens()) {
		    int neighbor_id=Integer.parseInt(t.nextToken());
		    nodes_list.addEdge(id,neighbor_id);
		}

	    }

	    
	    System.out.println("fin");
	    lecteurAvecBuffer.close();

	}
	catch(Exception ex){
    	    System.out.println("");
	    log("Error while parsing input file ! ",ex.toString());
	    System.out.println("Please use this format for graph descrition : node_x node_y nieghbor1_x neighbor1_y ...\n\n");
	    System.exit(-1);
	}

    }

    // ** Starts graph generation **

    public void process(){
	if (topo_type==-1 || dist_type==-1){
	    System.out.println("[INTERNAL ERROR] graph generation impossible !!! ("+topo_type+", "+dist_type+")\n");
	    System.exit(-1);
	}
	
	rd=new random();
	int nbr=0;

	// Computing nodes nbr
	if (dist_type==0 && topo_type==0) nbr=(int)rd.nextPoisson((int)(dist_uniform_intensity*topo_square_width*topo_square_width));
	else if (dist_type==0 && topo_type==1) nbr=(int)rd.nextPoisson((int)(dist_uniform_intensity*Math.PI*topo_circle_radius*topo_circle_radius));
	else if (dist_type==0 && topo_type==2) nbr=(int)rd.nextPoisson((int)(dist_uniform_intensity*topo_rect_width*topo_rect_height));
	else if (dist_type==1) nbr=dist_static_nbr;
	else if (dist_type==2) nbr=dist_grid_size_x*dist_grid_size_y;
	else System.out.println(topo_type+" "+dist_type);

	
	try {
	    
	    if (verbose==1) {
		show(12,"[INIT] ");
		System.out.print("Initializing data structures for "+nbr+" nodes... ");
	    }

	    int max_nbr=0;
	    
	    if (effect_cluster==1) {
		if (effect_cluster_intensity<=1) {
		    max_nbr = (int)rd.nextPoisson((int)(effect_cluster_intensity*effect_cluster_size*effect_cluster_size));
		}
		else {
		    max_nbr = (int)effect_cluster_intensity;
		}
	    }
	    

	    // Init data structure
	    nodes_list=new nodes(nbr+(max_nbr*effect_cluster_nbr),verbose);
	    if (verbose==1) System.out.println("done !");


	    // Generates nodes
	    if (verbose==1) {
		show(12,"[PROCESS] ");
		System.out.print("Generating a random graph with "+nbr+" nodes... ");
	    }
	    
	    
	    switch(topo_type){

		// square topology
	        case 0 : 

		    if (dist_type != 2) {
			for (int i=0; i<nbr; i++){
			    nodes_list.addNode(i,rd.nextRandom()*topo_square_width,rd.nextRandom()*topo_square_width);
			}
		    
		    }
		    else {
			double y = topogen.dist_grid_border;
			int i = 0;

			for (int k = 1; k <= dist_grid_size_y; k++) {

			    double x = topogen.dist_grid_border;

			    for (int j = 1; j <= dist_grid_size_x; j++) {
				nodes_list.addNode(i, x, y);
				x += topo_radius;
				i++;
			    }

			    y += topo_radius;
			}

		    }
		    break;

	        // Circle topology
		case 1 : 
		    
		    for (int i=0; i<nbr; i++){
			double rr=rd.nextRandom();
			double oo=rd.nextRandom()*(2*Math.PI);
			double xx=topo_circle_radius+(Math.sqrt(rr)*Math.cos(oo))*topo_circle_radius;
			double yy=topo_circle_radius+(Math.sqrt(rr)*Math.sin(oo))*topo_circle_radius;
			nodes_list.addNode(i,xx,yy);
		    }
		    break;

	        // Rectangle topology
		case 2 : 

		    if (dist_type != 2) {
			for (int i=0; i<nbr; i++){
			    nodes_list.addNode(i,rd.nextRandom()*topo_rect_width,rd.nextRandom()*topo_rect_height); 
			}
		    
		    }
		    else {
			double y = topogen.dist_grid_border;
			int i = 0;

			for (int k = 1; k <= dist_grid_size_y; k++) {

			    double x = topogen.dist_grid_border;

			    for (int j = 1; j <= dist_grid_size_x; j++) {
				nodes_list.addNode(i, x, y);
				x += topo_radius;
				i++;
			    }

			    y += topo_radius;
			}

		    }

		    break;
		
	    }

	    int id=nbr;

	    if (verbose==1) System.out.println("done !");

	    if (effect_type!=-1) {
		
		if (verbose==1) {
		    show(12,"[PROCESS] ");
		    System.out.print("Applying effects... ");
		}

		// Applying effect

		// cluster
		if (effect_cluster==1){

		    // Generates clusters


		    for (int i=0; i<effect_cluster_nbr; i++){
			

			int w=0;
			int h=0;
			double xx=0;
			double yy=0;

			if (topo_type==0) {
			    w=topo_square_width;
			    h=w;
			    xx=effect_cluster_size+rd.nextRandom()*(w-2*effect_cluster_size);
			    yy=effect_cluster_size+rd.nextRandom()*(h-2*effect_cluster_size);
			}
			else if (topo_type==1) {
			    w=topo_circle_radius*2;
			    h=w;
			    double circle_radius=topo_circle_radius-effect_cluster_size;
			    double rr=rd.nextRandom();
			    double oo=rd.nextRandom()*(2*Math.PI);
			    xx=topo_circle_radius+(Math.sqrt(rr)*Math.cos(oo))*circle_radius;
			    yy=topo_circle_radius+(Math.sqrt(rr)*Math.sin(oo))*circle_radius;	
			}
			else if (topo_type==2) {
			    w=topo_rect_width;
			    h=topo_rect_height;
			    xx=effect_cluster_size+rd.nextRandom()*(w-2*effect_cluster_size);
			    yy=effect_cluster_size+rd.nextRandom()*(h-2*effect_cluster_size);
			}
			
			for (int j=0; j<max_nbr; j++){
			    
			    double rr=rd.nextRandom();
			    double oo=rd.nextRandom()*(2*Math.PI);
			    
			    double xxx=xx+(Math.sqrt(rr)*Math.cos(oo))*effect_cluster_size;
			    double yyy=yy+(Math.sqrt(rr)*Math.sin(oo))*effect_cluster_size;
			    
			    nodes_list.addNode(id,xxx,yyy);

			    id++;
			}

			
		    }

		} // end cluster section
		

		// Hole
		if (effect_hole==1) {
		    
		    // Generates circular holes

		    for (int i=0; i<effect_hole_nbr; i++) {
			int w=0;
			int h=0;
			double xx=0;
			double yy=0;

			if (topo_type==0) {
			    w=topo_square_width;
			    h=w;
			    xx=effect_hole_size+rd.nextRandom()*(w-effect_hole_size);
			    yy=effect_hole_size+rd.nextRandom()*(h-effect_hole_size);
			}
			else if (topo_type==1) {
			    w=topo_circle_radius*2;
			    h=w;

			    double circle_radius=topo_circle_radius-effect_hole_size;
			    double rr=rd.nextRandom();
			    double oo=rd.nextRandom()*(2*Math.PI);
			    
			    xx=topo_circle_radius+(Math.sqrt(rr)*Math.cos(oo))*circle_radius;
			    yy=topo_circle_radius+(Math.sqrt(rr)*Math.sin(oo))*circle_radius;			    

			}
			else if (topo_type==2) {
			    w=topo_rect_width;
			    h=topo_rect_height;
			    xx=effect_hole_size+rd.nextRandom()*(w-effect_hole_size);
			    yy=effect_hole_size+rd.nextRandom()*(h-effect_hole_size);		
			}

			nodes_list.removeNodesInsideCircle(xx, yy, effect_hole_size);	
		    }

		} // end hole section


		if (verbose==1) System.out.println("done !");

	    }

	    // Refresh nodes ID et computing edges
	    nodes_list.refresh(topo_radius);
	    


	}
	catch(OutOfMemoryError ex){
	    System.out.println("");
	    log("[graph generation] Out of memory ! ",ex.toString());
	    System.out.println("Please use \"java -Xmx512m ...\" to increase the VM memory, or decrease the node density/number...\n\n");
	    System.exit(-1);
	}

    }


    void free(){
	nodes_list.free();
	nodes_list=null;
    }

    void showGraph(){
	if (nodes_list!=null) nodes_list.showNodes();
    }

    void saveGraph(int format_type, String format, String filename){
	if (nodes_list!=null) nodes_list.saveNodes(format_type, format, filename);
    }

    


}
