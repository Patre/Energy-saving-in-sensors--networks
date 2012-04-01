import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.text.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.awt.BorderLayout;
import java.awt.Container;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JProgressBar;
import javax.swing.border.Border;


// ******************
// *** Main Class ***
// ******************

class replay {

    public static void main(String arg[]){
	
	if (arg.length<1) {
	    System.out.println("Format : wsnet-replay [FILENAME] [ZOOM]"+"\n");
	    System.exit(-1);	    
	}
	else {
	    String log_filename=arg[0];
	    float zoom=1;
	    try {
		if (arg.length==2) zoom=Float.parseFloat(arg[1]); 
	    }
	    catch(Exception ex){
		System.out.println("Error while reading arguments : "+ex.toString());
		System.out.println("Format : wsnet-replay [FILENAME] [ZOOM]"+"\n");
		System.exit(-1);
	    }
	    int period=100;
	    String filename=new String("nodes.data");
	    String filename2=new String("environments.data");

	    progress p=new progress("WSNET topology viewer","Opening "+log_filename+"...", 0);
	    new pause(1000);
	    
	    analyze an=new analyze(log_filename,filename,filename2,period,p);
	    
	    p.close();

	    new data(period, an.getInfo(),zoom).run(filename);
	
	}
    }

}


// ************************************
// *** Class for topology animation ***
// ************************************

class data {

    int nodes_queue_capacity = 50;
    int environments_queue_capacity = 50;
    topoinfo info=null;
    float zoom;

    public BlockingQueue<elements> nodes_queue = new ArrayBlockingQueue<elements>(nodes_queue_capacity);

    public static final elements NO_MORE_NODES = new elements();

    int period; 

    public data(int period, topoinfo info, float zoom) {
	this.period=period;
	this.info=info;
	this.zoom=zoom;
    }

    public void run(String filename) {
	nodes_writer writer=new nodes_writer(this, filename);
	writer.start();

	new nodes_reader(this, period, info, writer, zoom).start();
    }

}


// *** Thread : writing data into a blocking and bounded FIFO ***

class nodes_writer extends Thread {

    BlockingQueue<elements> nodes_queue;
    data d;
    String filename;
    boolean repeat=false;

    public nodes_writer(data d, String filename) {
	this.d=d;
	this.nodes_queue=d.nodes_queue;
	this.filename=filename;
    }

    public void doRepeat() {
	repeat=true;
    }

    public void run() {

	try {
	 
	    FileInputStream inFile = new FileInputStream(filename);
	    ObjectInputStream in = new ObjectInputStream(inFile);
	    elements tmp;
	    int i=0;

	    try {

		if (repeat==true) {
		    in.close();

		    inFile = new FileInputStream(filename);
		    in = new ObjectInputStream(inFile);

		    repeat=false;
		}

		while((tmp=(elements)in.readObject())!=null) {
		    nodes_queue.put(tmp);
		    i++;
		}

		in.close();
	    }
	    catch(EOFException e) {
		//System.out.println("End of stream encountered");
	    }

	    nodes_queue.put(d.NO_MORE_NODES);

	}
	catch(Exception ex) {
	    System.out.println("Erreur thread Writer : "+ex.toString());
	    System.exit(-1);
	}

    }


}

// *** Thread : reading from the FIFO and update graphics ***

class nodes_reader extends Thread {

    BlockingQueue<elements> nodes_queue;
    data d;
    int refresh_period=100;
    int i=0;
    int period;
    topoinfo info;
    float zoom;
    boolean state=true;
    nodes_writer writer;

    public nodes_reader(data d, int period, topoinfo info, nodes_writer writer, float zoom) {
	this.d=d;
	this.nodes_queue=d.nodes_queue;
	this.period=period;
	this.info=info;
	this.writer=writer;
	this.zoom=zoom;
    }

    public void setActive(boolean state) {
	this.state=state;
    }

    public void doRepeat() {
	writer.doRepeat();
    }

    public void run() {

	try {

	    frame t=new frame(zoom,info,this);

	    while (true) {
		Thread.sleep(refresh_period);
		
		while (state!=true) {
		    Thread.sleep(50);
		}

		elements tmp=(elements)nodes_queue.take();
		

		if (tmp==d.NO_MORE_NODES) break;
		
		long time=tmp.data.elementAt(0).time;
		t.refreshData(tmp,time);
		i++;
		
	    }

	    t.finished();

	}
	catch(InterruptedException ex) {
	    System.out.println("Erreur reader : "+ex.toString());
	    System.exit(-1);
	}
    }


}


class progress {

    String title=null;
    String subtitle=null;
    int value=0;

    JProgressBar progressBar;
    Border border;
    JFrame f;

    public progress(String title, String subtitle, int value) {
	this.title=title;
	this.subtitle=subtitle;
	this.value=value;

	f = new JFrame(this.title);
	f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	Container content = f.getContentPane();
	progressBar = new JProgressBar();
	progressBar.setValue(this.value);
	progressBar.setStringPainted(true);
	border = BorderFactory.createTitledBorder(this.subtitle);
	progressBar.setBorder(border);
	content.add(progressBar, BorderLayout.NORTH);
	f.setSize(350, 80);
	f.setResizable(false);
	f.setLocationRelativeTo(f.getParent());
	f.setVisible(true);
    }

    public void close() {
	f.setVisible(false);
    }

    public void setValue(int value) {
	progressBar.setValue(value);
    }

    public void setSubTitle(String title) {
	border = BorderFactory.createTitledBorder(title);
	progressBar.setBorder(border);
    }
    
    

}

class pause {

    public pause(int milli) {
	try {
	    Thread.sleep(milli);
	}catch(Exception ex){}
    }

}

// ***************************
// *** Some New Data Types ***
// ***************************

class nodes implements Serializable {
    public Vector<node> data;
    public nodes() {
	data=new Vector<node>();
    }
    public void addNode(int id, double x, double y, long time, double value) {
	node tmp=new node();
	tmp.id=id;
	tmp.x=x;
	tmp.y=y;
	tmp.time=time;
	tmp.value=value;
	data.add(tmp);
    }
}

class environments implements Serializable {
    public Vector data;
    public environments() {
	data=new Vector();
    }
}


class node implements Serializable {
    public int id;
    public int type;
    public int state;
    public double x;
    public double y;
    public double value;
    public long time;
}


class environment implements Serializable {
    public String name;
    public int shape;
    public double value;
    public double x;
    public double y;
    public long time;
}


class elements implements Serializable {
    public Vector<element> data;
    public elements() {
	data=new Vector<element>();
    }
    public void addNode(int id, double x, double y, long time, int state, int type,double value) {
	element tmp=new element();
	tmp.element_type=0;

	tmp.id=id;
	tmp.state=state;
	tmp.type=type;

	tmp.x=x;
	tmp.y=y;
	tmp.time=time;
	tmp.value=value;

	data.add(tmp);
    }

    public void addEnv(String name, double x, double y, long time, int shape, double value) {
	element tmp=new element();
	tmp.element_type=1;

	tmp.name=name;
	tmp.shape=shape;
	tmp.value=value;

	tmp.x=x;
	tmp.y=y;
	tmp.time=time;

	data.add(tmp);
    }
}


class element implements Serializable {

    public int element_type;

    // Node's data
    public int state;
    public int type;
    public int id;

    // Environment's data
    public String name;
    public int shape;
    public double value;

    // Common data
    public double x;
    public double y;
    public long time;

}


class topoinfo {
    long max_time;
    int nodes_nbr;
    int area_3D;
    int width;
    int height;
}

// *********************************************************
// *** Class for analyzing and formating input data file ***
// *********************************************************

class analyze {

    String filename;
    String nodes_filename;
    String environments_filename;
    int period;
    int nodes_nbr=10;

    Vector<node> node_tmp=new Vector<node>();
    Vector<environment> env_tmp=new Vector<environment>(); 
    Vector<node> event_tmp=new Vector<node>(); 

    progress p;

    public analyze(String filename, String nodes_filename, String environments_filename, int period, progress p){
	this.filename=filename;
	this.nodes_filename=nodes_filename;
	this.environments_filename=environments_filename;
	this.period=period;
	this.p=p;

	try {
	    p.setSubTitle("Analyzing "+filename+"...");

	    // Flushing output files
	    File f=new File(nodes_filename);
	    f.delete();
	    f=new File(nodes_filename+".tmp");
	    f.delete();
	    f=new File(environments_filename);
	    f.delete();
	    f=new File(environments_filename+".tmp");
	    f.delete();

	    p.setValue(10);

	    topoinfo tmp=getInfo();
	    this.nodes_nbr=tmp.nodes_nbr;
	    System.out.println("Nodes number         = "+tmp.nodes_nbr);
	    System.out.println("Max simulation time  = "+(tmp.max_time*0.000000001)+"s  ("+tmp.max_time+"ns)");
	    System.out.println("Simulation area size = "+tmp.width+"x"+tmp.height);


	    // Analyzing node's records
	    parseNodes();

	    p.setSubTitle("Removing tmp files...");
	    new pause(1000);


 	    // Removing temp files
	    f=new File(nodes_filename+".tmp");
	    f.delete();
	    f=new File(environments_filename+".tmp");
	    f.delete();

	    p.setValue(100);
	    new pause(1000);
	

	}
	catch(Exception ex){
	    System.out.println("Error while analyzing input file : "+ex);
	    System.exit(-1);
	}


    }

    int getNodeNbr() {
	return this.nodes_nbr;
    }

    topoinfo getInfo() {
	try {

	    Vector<Integer> Noeuds = new Vector<Integer>();

	    topoinfo tmp=new topoinfo();
	    tmp.max_time=0;
	    tmp.nodes_nbr=0;
	    tmp.area_3D=0;
	    tmp.width=0;
	    tmp.height=0;

	    BufferedReader input = new BufferedReader(new FileReader(this.filename));
	    String line = null; 
	    
	    while (( line = input.readLine()) != null){

		if (line.startsWith("mobility") || line.startsWith("environement") ||  line.startsWith("environment")  || line.startsWith("kill") || line.startsWith("radio") ) {
		    StringTokenizer tk=new StringTokenizer(line," ");
		    tk.nextToken();

		    long time=Long.parseLong(tk.nextToken());
		    if (time>tmp.max_time) tmp.max_time=time;
		    
		    if (line.startsWith("mobility") || line.startsWith("kill")) {
			int id=Integer.parseInt(tk.nextToken());
			if (!Noeuds.contains((int)id)) Noeuds.addElement((int)id); 
			
			double x=Double.parseDouble(tk.nextToken());
			double y=Double.parseDouble(tk.nextToken());
			double z=Double.parseDouble(tk.nextToken());
			
			if (tmp.area_3D==0 && z!=0) tmp.area_3D = 1;
			if (x>tmp.width) tmp.width = (int)x;
			if (y>tmp.height) tmp.height = (int)y;

		    }
		}
	    }
		
	    tmp.nodes_nbr=Noeuds.size();

	    return tmp;
	}
	catch(Exception ex){
	    System.out.println("Error during getinfo : "+ex.toString());
	    System.exit(-1);
	    return null;
	}
    }

    void parseNodes() {
	
	BufferedReader input = null;
	
	try {

	    // Extracting node's informations

	    p.setSubTitle("Reading nodes informations...");
	    new pause(1000);

	    for (int i=0; i<nodes_nbr; i++) { 

		input = new BufferedReader(new FileReader(this.filename));
		String line = null; 
		node_tmp.removeAllElements();


		while (( line = input.readLine()) != null){
		
		    // Extract node's records
		    if (line.startsWith("mobility")) {
			
			StringTokenizer tk=new StringTokenizer(line," ");
			tk.nextToken();
			long time=Long.parseLong(tk.nextToken());
			int id=Integer.parseInt(tk.nextToken());
			double x=Double.parseDouble(tk.nextToken());
			double y=Double.parseDouble(tk.nextToken());
			double z=Double.parseDouble(tk.nextToken());
		    
			if (id==i) {

			    node tmp=new node();
			    tmp.x=x;
			    tmp.y=y;
			    tmp.id=id;
			    tmp.time=(long)(time*0.000001);

			    tmp.state=drawtopo.STATE_ACTIVE;
			    tmp.type=0;

			    node_tmp.addElement(tmp);

			}

		    }//end if

				

		}//end while

		input.close();

		if (node_tmp.size()!=0) {
		    approximate(node_tmp, period, i, nodes_filename+".tmp");
		}

	    }// end for
	    
	    p.setValue(30);

	    p.setSubTitle("Reading environments informations...");
	    new pause(500);

	    // Extracting environment's informations

	    try {
		input = new BufferedReader(new FileReader(this.filename));
		String line = null; 
		node_tmp.removeAllElements();
		env_tmp.removeAllElements();


		while (( line = input.readLine()) != null){
		    if (line.startsWith("envir")) {
			StringTokenizer tk=new StringTokenizer(line);
			tk.nextToken();
			long time=Long.parseLong(tk.nextToken());
			String name=tk.nextToken();
			String shape=tk.nextToken();
			double x=Double.parseDouble(tk.nextToken());
			double y=Double.parseDouble(tk.nextToken());
			double z=Double.parseDouble(tk.nextToken());
			double value=Double.parseDouble(tk.nextToken());

			environment tmp=new environment();
			tmp.name=name;
			tmp.shape=0; // TODO : supporting other shape types...
			tmp.value=value;
			tmp.x=x;
			tmp.y=y;
			tmp.time=(long)(time*0.000001);
			
			env_tmp.addElement(tmp);

		    }
		}

		p.setValue(50);

		p.setSubTitle("Reading events informations...");
	        new pause(500);


		// Extracting Event informations

		input = new BufferedReader(new FileReader(this.filename));
		line = null; 

		while (( line = input.readLine()) != null){
		    if (line.startsWith("kill")) {
			StringTokenizer tk=new StringTokenizer(line);
			tk.nextToken();
			long time=Long.parseLong(tk.nextToken());
			int id=Integer.parseInt(tk.nextToken());
			double x=Double.parseDouble(tk.nextToken());
			double y=Double.parseDouble(tk.nextToken());

			node tmp=new node();
			tmp.id=id;
			tmp.x=x;
			tmp.y=y;
			tmp.time=(int)(time*0.000001);
			tmp.state=drawtopo.STATE_DEAD;
			tmp.type=0;

			event_tmp.addElement(tmp);

		    }
		    else if (line.startsWith("radio-")) {
			StringTokenizer tk=new StringTokenizer(line);

			node tmp=new node();

			String radio_event=tk.nextToken();
			long time=Long.parseLong(tk.nextToken());
			int id=Integer.parseInt(tk.nextToken());
			double y=0;
		
			if (radio_event.startsWith("radio-tx0")) {
			    y=Double.parseDouble(tk.nextToken());
			    tmp.state=drawtopo.STATE_TX_BEGIN;
			}
			else if (radio_event.startsWith("radio-tx1")) {
			    tmp.state=drawtopo.STATE_TX_END;
			}
			else if (radio_event.startsWith("radio-rx0")) {
			    tmp.state=drawtopo.STATE_RX_BEGIN;
			}
			else if (radio_event.startsWith("radio-rx1")) {
			    tmp.state=drawtopo.STATE_RX_END;
			}
			else tmp.state=drawtopo.STATE_ACTIVE;

			tmp.id=id;
			tmp.x=-1;
			tmp.y=-1;
			tmp.time=(long)(time*0.000001);
			tmp.type=0;
			tmp.value=y;

			event_tmp.addElement(tmp);

		    }

		}

		p.setValue(70);

	    }
	    catch(Exception ex){
		System.out.println("Error while parsing env : "+ex.toString());
		System.exit(-1);
	    }

	    
	    
	    p.setSubTitle("Generating output file...");
	    new pause(500);

	    // Sorting and Generating data file
	    sort(nodes_filename+".tmp", nodes_filename, env_tmp, event_tmp);

	    p.setValue(90);
		
	}
	catch(Exception ex) {
	    System.out.println("Erreur : "+ex.toString());
	    System.exit(-1);
	}
    }

    void approximate(Vector<node> node_tmp, int period, int id, String filename) {
	
	try {

	    int nbr=node_tmp.size();
	    int t1=0;
	    int t2=1;

	    FileWriter outFile = new FileWriter(filename,true);
	    PrintWriter out = new PrintWriter(outFile);

	    out.println(node_tmp.get(t1).time+":"+id+":"+node_tmp.get(t1).x+":"+node_tmp.get(t1).y+":"+node_tmp.get(t1).state+":"+node_tmp.get(t1).type+":"+node_tmp.get(t1).value);
	
	    if (node_tmp.size()>1) { 


		for (int i=0; i<nbr-1; i++) {

		    int segments=(int)(node_tmp.get(t2).time-node_tmp.get(t1).time)/period;
    
		    for (int j=1; j<=segments; j++) {

			double xx=node_tmp.get(t1).x+((float)(node_tmp.get(t2).x-node_tmp.get(t1).x)/segments)*j;
			double yy=node_tmp.get(t1).y+((float)(node_tmp.get(t2).y-node_tmp.get(t1).y)/segments)*j;

			out.println((node_tmp.get(t1).time+period*j)+":"+id+":"+xx+":"+yy+":"+node_tmp.get(t1).state+":"+node_tmp.get(t1).type+":"+node_tmp.get(t1).value);


		    }

		    if ((t2+1<nbr) &&  node_tmp.get(t2).time==node_tmp.get(t2+1).time) {
			t1=t2+1;
			t2=t1+1;
			i++;
		    }
		    else {
			node_tmp.get(t2).time=node_tmp.get(t1).time+period*segments;
			t1++;
			t2++;
		    }

		}//end for

	  }//end if

	  out.close();

	}
	catch(Exception ex){
	    System.out.println("Error approx : "+ex.toString());
	    System.exit(-1);
	}

    }


    void sort(String input, String output, Vector<environment> env_tmp, Vector<node> event_tmp) {

	try {

	    BufferedReader in = null;
	    in = new BufferedReader(new FileReader(input));

	    String line = null; 
	    Vector<Long> V=new Vector<Long>();
	    
	    // Parsing input file
	    while (( line = in.readLine()) != null){

		StringTokenizer tk=new StringTokenizer(line,":");
		long time = Long.parseLong(tk.nextToken());
		
		// Insert time into Vector
		int i=0;
		for (i=0; i<V.size(); i++)
		    if (V.elementAt(i)==time) break;
		if (V.size()==0 || i==V.size()) V.addElement(time);
		
		// Writing to output TMP files
		FileWriter outFile = new FileWriter(output+Long.toString(time),true);
		PrintWriter out = new PrintWriter(outFile);
		out.println(line);
		out.close();

	    }
	    in.close();

	    // Parsing Env Vector
	    for (int i=0; i<env_tmp.size(); i++) {
		
		environment tmp=env_tmp.elementAt(i);

		long time = tmp.time;

		// Insert time into Vector
		int j=0;
		for (j=0; j<V.size(); j++)
		    if (V.elementAt(j)==time) break;
		
		if (V.size()==0 || j==V.size()) V.addElement(time);
		
		// Writing to output file
		FileWriter outFile = new FileWriter(output+Long.toString(time),true);
		PrintWriter out = new PrintWriter(outFile);
		out.println("env:"+tmp.time+":"+tmp.name+":"+tmp.shape+":"+tmp.x+":"+tmp.y+":"+tmp.value);
		out.close();

	    }

	    // Parsing Event Vector
	    for (int i=0; i<event_tmp.size(); i++) {
		
		node tmp=event_tmp.elementAt(i);

		long time = tmp.time;

		// Insert time into Vector
		int j=0;
		for (j=0; j<V.size(); j++)
		    if (V.elementAt(j)==time) break;
		
		if (V.size()==0 || j==V.size()) V.addElement(time);
		
		// Writing to output file
		FileWriter outFile = new FileWriter(output+Long.toString(time),true);
		PrintWriter out = new PrintWriter(outFile);

		    
		String ss=new String(time+":"+tmp.id+":"+tmp.x+":"+tmp.y+":"+tmp.state+":"+tmp.type+":"+tmp.value);

		out.println(ss);		
		out.close();

	    }

	    // Sorting Vector's elements
	    Collections.sort(V);

	    // Merging and removing files

	    FileOutputStream outFile = new FileOutputStream(output);
	    ObjectOutputStream out = new ObjectOutputStream(outFile);

	    for (int i=0; i<V.size(); i++) {

		// Generating nodes obj
		elements tmp=new elements();
		
		in = new BufferedReader(new FileReader(output+V.elementAt(i)));

		while (( line = in.readLine()) != null){
		    if (line.startsWith("env")) {
			
			StringTokenizer tk=new StringTokenizer(line,":");
			tk.nextToken();

			long time=Long.parseLong(tk.nextToken());
			String name=tk.nextToken();
			int shape=Integer.parseInt(tk.nextToken());
			double x=Double.parseDouble(tk.nextToken());
			double y=Double.parseDouble(tk.nextToken());
			double value=Double.parseDouble(tk.nextToken());

			tmp.addEnv(name,x,y,time,shape,value);

		    }
		    else {

			StringTokenizer tk=new StringTokenizer(line,":");
			long time=Long.parseLong(tk.nextToken());
			int id=Integer.parseInt(tk.nextToken());
			double x=conversionDouble(tk.nextToken());
			double y=conversionDouble(tk.nextToken());
			if (x==-1 || y==-1) {
			    x = -1; y = -1;
			}
			int state=Integer.parseInt(tk.nextToken());
			int type=Integer.parseInt(tk.nextToken());
			double value=conversionDouble(tk.nextToken());

			tmp.addNode(id,x,y,time,state,type,value);

		    }
		}
		in.close();


		out.writeObject(tmp);

		File f=new File(output+V.elementAt(i));
		f.delete();
		
	    }//end for

	    out.flush();
	    out.close();

	}
	catch(Exception ex) {
	    System.out.println("Error sort : "+ex);
	    System.exit(-1);
	}

    }

    double conversionDouble(String chaine) {
	double tmp=0;
	try {
	    tmp = Double.parseDouble(chaine);
	}catch(Exception ex) {
	    System.out.println("Error while converting "+chaine+" to double...");
	    tmp = -1;
	}
	return tmp;
    }

}

// *** Class used by Collection to sort a vector of integers

class Comparer implements Comparator {
    public int compare(Object obj1, Object obj2)
    {
	int i1 = ((Integer)obj1).intValue();
	int i2 = ((Integer)obj2).intValue();
        
	return Math.abs(i1) - Math.abs(i2);
    }
}


