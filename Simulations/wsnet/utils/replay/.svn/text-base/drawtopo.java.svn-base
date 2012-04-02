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



class drawtopo extends Canvas implements Runnable {

    /* Global variables */
    public final static int STATE_DEAD     = 0;
    public final static int STATE_ACTIVE   = 1;
    public final static int STATE_TX_BEGIN = 2;
    public final static int STATE_TX_END   = 3;
    public final static int STATE_RX_BEGIN = 4;
    public final static int STATE_RX_END   = 5;

    Color STATE_DEAD_color     = Color.red;
    Color STATE_ACTIVE_color   = Color.blue;
    Color STATE_TX_BEGIN_color = Color.magenta;
    Color STATE_TX_END_color   = Color.magenta;
    Color STATE_RX_BEGIN_color = Color.green;
    Color STATE_RX_END_color   = Color.green;

    private int radio_width = 12;    
    private int sensor_width  = 10;

    /* Variables */
    private Graphics offScreenGraphics;
    private Image offScreenImage;
    private float zoom;
    private int width;
    private int height;
    private int nbr_nodes;
    private long time=0;
 

    Vector<node> V;
    Vector<environment> env=new Vector<environment>();

    NumberFormat format = NumberFormat.getInstance();
    frame f=null;
    
    public drawtopo(int width, int height, float zoom, int nbr_nodes, frame f) {
	this.width=width;
	this.height=height;
	this.zoom=zoom;
	this.nbr_nodes=nbr_nodes;
	this.f=f;

	format.setMaximumFractionDigits(2);


	// Init Data
	V = new Vector<node>();
	for (int i=0; i<nbr_nodes; i++) {
	    node tmp=new node();
	    tmp.id=i;
	    tmp.x=-1;
	    tmp.y=-1;
	    tmp.time=-1;
	    V.addElement(tmp);
	}

	setSize((int)(width*zoom+10),(int)(height*zoom+10));

	Thread thread=new Thread(this);
	thread.start();
    }

    public void refreshData(elements tmp, long time) {
	for (int i=0; i<tmp.data.size(); i++) {
	    // refresh node's parameters
	    if (tmp.data.elementAt(i).element_type==0) {
		
		if (tmp.data.elementAt(i).x!=-1 && tmp.data.elementAt(i).y!=-1) {
		    (V.elementAt(tmp.data.elementAt(i).id)).x=tmp.data.elementAt(i).x;
		    (V.elementAt(tmp.data.elementAt(i).id)).y=tmp.data.elementAt(i).y;
		}

		(V.elementAt(tmp.data.elementAt(i).id)).time=tmp.data.elementAt(i).time;

		
		int o_state=(V.elementAt(tmp.data.elementAt(i).id)).state;
		int n_state=tmp.data.elementAt(i).state;

		if (n_state == drawtopo.STATE_DEAD) {
		    (V.elementAt(tmp.data.elementAt(i).id)).value = tmp.data.elementAt(i).value;
		    (V.elementAt(tmp.data.elementAt(i).id)).state = tmp.data.elementAt(i).state;
		}
		else if (o_state == drawtopo.STATE_TX_BEGIN) {

		    if (n_state == drawtopo.STATE_TX_END) {
			(V.elementAt(tmp.data.elementAt(i).id)).value = tmp.data.elementAt(i).value;
			(V.elementAt(tmp.data.elementAt(i).id)).state = n_state;
		    }
		    
		}
		else if (o_state == drawtopo.STATE_RX_BEGIN) {

		    if (n_state == drawtopo.STATE_RX_END) {
			(V.elementAt(tmp.data.elementAt(i).id)).value = tmp.data.elementAt(i).value;
			(V.elementAt(tmp.data.elementAt(i).id)).state = n_state;
		    }
		    
		}
		else {
		    (V.elementAt(tmp.data.elementAt(i).id)).value = tmp.data.elementAt(i).value;
		    (V.elementAt(tmp.data.elementAt(i).id)).state = tmp.data.elementAt(i).state;
		}
		
		
	    }
	    // refresh env's parameters
	    else if (tmp.data.elementAt(i).element_type==1) {
		
		environment nn=new environment();
		nn.name=tmp.data.elementAt(i).name;
		nn.shape=tmp.data.elementAt(i).shape;
		nn.value=tmp.data.elementAt(i).value;
		nn.x=tmp.data.elementAt(i).x;
		nn.y=tmp.data.elementAt(i).y;
		nn.time=tmp.data.elementAt(i).time;
		
		
		boolean trouv=false;

		for (int j=0; j<env.size(); j++) {
		    if (env.elementAt(j).name.compareTo(tmp.data.elementAt(i).name)==0) {
			env.elementAt(j).value=tmp.data.elementAt(i).value;
			env.elementAt(j).time=tmp.data.elementAt(i).time;
			trouv=true;
			break;
		    }
		}
		
		if (trouv==false) env.addElement(nn);


	    }

	    this.time=time;
	}
    }


    public void run() {

	while(true) {
	    try {
		
		Thread.sleep(1);
		repaint();

	    }catch (InterruptedException e) {
		System.out.println(e);
	    }
	    repaint();
	}

    }

    private AlphaComposite makeComposite(float alpha) {
	int type = AlphaComposite.SRC_OVER;
	return(AlphaComposite.getInstance(type, alpha));
    }

    private void drawOval(Graphics2D g2d, float alpha, int x, int y, int width) {
	g2d.drawOval(x,y,width,width);
	Composite originalComposite = g2d.getComposite();
	g2d.setComposite(makeComposite(alpha));
	g2d.fillOval(x,y,width,width);
	g2d.setComposite(originalComposite);
    }

    private void drawSensor(Graphics2D g2d, int id, int x, int y, int state, int radius1, int radius2, int value) {
	
	/* Draw radio state */

	switch(state) {
	   case drawtopo.STATE_TX_BEGIN : g2d.setColor(STATE_TX_BEGIN_color); break;
	   case drawtopo.STATE_RX_BEGIN : g2d.setColor(STATE_RX_BEGIN_color); break;
	}
	
	if (state != drawtopo.STATE_DEAD && state != drawtopo.STATE_ACTIVE && state != drawtopo.STATE_TX_END && state != drawtopo.STATE_RX_END ) {


	    g2d.fillOval(x-radius2, y-radius2, 2*radius2, 2*radius2);

	    if (value>0) {
		g2d.setColor(Color.red);
		g2d.drawOval(x-value, y-value, 2*value, 2*value);
	    }
	    
	}
	

	/* Draw sensor state */

	switch(state) {
	   case drawtopo.STATE_DEAD : g2d.setColor(STATE_DEAD_color); break;
	   default : g2d.setColor(STATE_ACTIVE_color); break;
	}

	g2d.fillOval(x-radius1, y-radius1, 2*radius1, 2*radius1);
	
	g2d.setColor(Color.white);
	g2d.drawString( Integer.toString(id), x - (radius1 / 2), y+radius1/3 );

    }


    public void drawTopology(Graphics g) {
	Graphics2D g2d = (Graphics2D)g;

	g2d.clearRect(0,0,this.getWidth(),this.getHeight());

	g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
	g2d.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);	g2d.clearRect(0,0,this.getWidth(),this.getHeight());

	g2d.setBackground(Color.black);
	
	// drawing environments

	for (int i=0; i<env.size(); i++) {

	    String name=env.elementAt(i).name;
	    int xx=(int)env.elementAt(i).x;
	    int yy=(int)env.elementAt(i).y;
	    int value=(int)env.elementAt(i).value;
	    int shape=env.elementAt(i).shape;

	    if (xx!=-1 && yy!=-1) {
		
		if (shape==0 && name.startsWith("fire")) {

		    g2d.setColor(new Color(255,165,0));
		    float alpha=0.4F;
		    drawOval(g2d, alpha, (int)((xx-value)*zoom), (int)((yy-value)*zoom), (int)(2*value*zoom)); 

		}
		else if (shape==0 && name.startsWith("heat")) {

		    g2d.setColor(new Color(178,34,34));
		    float alpha=0.4F;
		    drawOval(g2d, alpha, (int)((xx-value)*zoom), (int)((yy-value)*zoom), (int)(2*value*zoom)); 

		}

	    }
	}

	// Drawing nodes

	for (int i=0; i<V.size(); i++) {

	    int id=(int)V.elementAt(i).id;
	    int xx=(int)V.elementAt(i).x;
	    int yy=(int)V.elementAt(i).y;
	    int state=(int)V.elementAt(i).state;
	    double value=V.elementAt(i).value;

	    if (xx!=-1 && yy!=-1) {
		drawSensor(g2d, id, (int)(xx*zoom), (int)(yy*zoom), state, sensor_width, radio_width, (int)(value*zoom));
	    }
	}
	
    }


    public void update(Graphics g) {

	if (offScreenGraphics == null) {
	    offScreenImage = createImage(getSize().width, getSize().height);
	    offScreenGraphics = offScreenImage.getGraphics();
	}
    
	// Draw the sprites offscreen
	drawTopology(offScreenGraphics);

	// Draw the scene onto the screen
	if(offScreenImage != null) g.drawImage(offScreenImage, 1, 1, this);
	
    }


    public void paint(Graphics g) {
	//Nothing required here.  All 
	// drawing is done in the update 
	// method above.
    }


}

