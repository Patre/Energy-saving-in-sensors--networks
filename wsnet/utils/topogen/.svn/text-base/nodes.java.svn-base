import java.io.*;
import java.util.*;

class nodes {

    public Vector <node_set> Nodes = new Vector <node_set>();
    public int Nodes_adj[][];
    
    private int Nodes_nbr;
    private int verbose=1;

    public nodes(int nbr, int verbose){
	Nodes_nbr = nbr;
	this.verbose = verbose;
    }

    public void free(){
	Nodes=null;
	Nodes_adj=null;
    }

    public void addNode(int id, double x, double y){
	node_set tmp=new node_set(id, x, y);
	Nodes.addElement(tmp);
    }

    public void refresh(){
	Nodes_nbr=Nodes.size();
	Nodes_adj=new int[Nodes_nbr][Nodes_nbr];
    }

    public void addEdge(int u, int v) {
	Nodes_adj[u][v]=1;
	Nodes_adj[v][u]=1;
    }

    public void refresh(double radius){
	// update nodes ID
	for (int i=0; i<Nodes.size(); i++) {
	    ((node_set)Nodes.get(i)).setId(i);
	}

	// computing edges
	Nodes_nbr=Nodes.size();
	Nodes_adj=new int[Nodes_nbr][Nodes_nbr];
	
	double x1=0, x2=0;
	double y1=0, y2=0;

	for (int i=0; i<Nodes_nbr; i++) {
	    
	    x1=((node_set)Nodes.get(i)).getX();
	    y1=((node_set)Nodes.get(i)).getY();

	    for (int j=0; j<Nodes_nbr; j++) {
		if (i!=j) {
		    x2=((node_set)Nodes.get(j)).getX();
		    y2=((node_set)Nodes.get(j)).getY();
		    
		    double D=Math.sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
		    if (D<=radius) {
			Nodes_adj[i][j]=1;
			Nodes_adj[j][i]=1;
		    }
		    else Nodes_adj[i][j]=0;

		}
	
	    }
	}
	
    }

    node_set getNode(int id){
	return (node_set)Nodes.get(id);
    }

    public int getNodesNbr(){
	return Nodes_nbr;
    }

    public void removeNodesInsideCircle(double circle_x, double circle_y, double radius){
	int n=0;

	for (int i=0; i<Nodes_nbr; i++) {
	    double x=((node_set)Nodes.get(i)).getX();
	    double y=((node_set)Nodes.get(i)).getY();
	    double D=Math.sqrt((x-circle_x)*(x-circle_x)+(y-circle_y)*(y-circle_y));
	    if (D<=radius){
		n++;
		Nodes.remove(i);
		i=0;
		Nodes_nbr--;
	    }	    
	}

    }

    public void showNodes(){
	for (int i=0; i<Nodes_nbr; i++) {
	    int id=((node_set)Nodes.get(i)).getId();
	    double x=((node_set)Nodes.get(i)).getX();
	    double y=((node_set)Nodes.get(i)).getY();
	    System.out.println(id+"\t"+x+"\t"+y);
	}
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


    public void saveNodes(int format_type, String format, String filename){
	try {

	    FileWriter fw = new FileWriter(filename);
	    
	    for (int i=0; i<Nodes_nbr; i++) {
		int id=((node_set)Nodes.get(i)).getId();
		double x=((node_set)Nodes.get(i)).getX();
		double y=((node_set)Nodes.get(i)).getY();

		switch(format_type){

		    case -1 : fw.write(id+" "+x+" "+y+" "+"0"+'\n'); break; // default
		    case 1 : fw.write("STATIC_STATIC "+x+" "+y+" 0.0"+'\n'); break; // WSNET
		    case 3 : fw.write(id+" "+x+" "+y);
			     for (int k=0; k<Nodes_nbr; k++)
				 if (Nodes_adj[i][k]==1 && i!=k) fw.write(" "+k);
			     fw.write("\n");
			     break;

		    case 0 : StringTokenizer tok=new StringTokenizer(format,"#");
		             while (tok.hasMoreTokens()){
				 String mot=tok.nextToken();
				 if (mot.toLowerCase().equals("x")) fw.write(Double.toString(x));
				 else if (mot.toLowerCase().equals("y")) fw.write(Double.toString(y));
				 else if (mot.toLowerCase().equals("xi")) fw.write(Integer.toString((int)x));
				 else if (mot.toLowerCase().equals("yi")) fw.write(Integer.toString((int)y));
				 else if (mot.toLowerCase().equals("id")) fw.write(Integer.toString(id));
				 else if (mot.equals("\\t")) fw.write('\t');
				 else if (mot.equals("\\r")) fw.write('\r');
				 else if (mot.equals("\\n")) fw.write('\n');
				 else fw.write(mot);
			     }
			     fw.write('\n');
			     break;
		   
                    default : fw.write(id+"\t"+x+"\t"+y+'\n'); break;

		}

	    }
	    fw.close();
	}
	catch(Exception ex){
	    log("I/O error while writing to file...",ex.toString());
	    System.out.println("");
	    System.exit(-1);
	}
	
	if (verbose==1) {
	    show(12,"[INFO] ");
	    System.out.println("Graph saved in "+filename+".");
	}
    }

}

