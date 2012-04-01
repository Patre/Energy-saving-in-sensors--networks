
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.*;
import javax.imageio.*;


class frame2 extends JFrame {
    int width;
    int height;
    Panel2 jPanel;
    int verbose;
    int zoom;
    
    nodes nodes_list;

  public frame2(int zoom, int w, int h, int verbose, nodes nodes_list)  {
	  this.width=w*zoom;
	  this.height=h*zoom;
	  this.verbose=verbose;
	  this.nodes_list=nodes_list;
	  this.zoom=zoom;

	  Container content = getContentPane();
	  content.setLayout(null);
      	  setSize(width+40,height+40);
          setTitle("Graph drawing utility");
	  try {
             jPanel = new Panel2(zoom, nodes_list);
	     jPanel.setBounds(new Rectangle(10, 10, width, height));
             content.add(jPanel);
	     this.addWindowListener(new CloseFrameListener());
	     setVisible(true);
	  }
  	  catch(OutOfMemoryError ex){
	    System.out.println("");
	    log("Out of memory ! ",ex.toString());
	    System.out.println("Please use \"java -Xmx512m ...\" to increase the VM memory, or decrease the node density/number...\n\n");
	    System.exit(-1);
	  }

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
	System.out.println("\nType \"topogen --help\" or \"topogen --examples\" for help...");
  }

  public void savePicture(String filename) {
        JPanel jp=jPanel;

        BufferedImage image = new BufferedImage(jp.getWidth(), jp.getHeight(),BufferedImage.TYPE_INT_RGB);
        Graphics2D g2 = image.createGraphics();

        g2.setBackground(Color.white);
	jp.paint(g2);
	g2.dispose();
        
	try {
          ImageIO.write(image, "png", new File(filename+".png"));
        } catch (IOException e) {      
          e.printStackTrace();
        }


	if (verbose==1) {
	    show(12,"[INFO] ");
	    System.out.println("Graph image saved in "+filename+".png");  
	}
  }
  
  class CloseFrameListener extends WindowAdapter {
    public void windowClosing(WindowEvent event) {
      System.exit(0);
    }
  }
  
  public JPanel getJPanel() {
	  return jPanel;
  }
  
}

class Panel2 extends JPanel {
	
    int node_width;
    int zoom=1;
    nodes nodes_list;

    public Panel2(int zoom, nodes nodes_list) {
		this.zoom=zoom;
		this.nodes_list=nodes_list;  
		node_width=2;
    	
	}
	
	public void paint(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(Color.white);
		g2d.clearRect(0,0,this.getWidth(),this.getHeight());
		
		// Show the edges
		g2d.setColor(Color.gray);

		for (int i=0; i<nodes_list.Nodes.size(); i++) {
		
                    int x1=(int)((node_set)nodes_list.Nodes.get(i)).getX();
		    int y1=(int)((node_set)nodes_list.Nodes.get(i)).getY();
			
		    for (int j=0; j<nodes_list.Nodes.size(); j++) {
			
			if (i!=j && nodes_list.Nodes_adj[i][j]>0) {
			   
			   int x2=(int)((node_set)nodes_list.Nodes.get(j)).getX();
			   int y2=(int)((node_set)nodes_list.Nodes.get(j)).getY();
			   
  		   	   g2d.drawLine(x1*zoom,y1*zoom,x2*zoom,y2*zoom);
			}

		    }
		}
		 
		// Show the vertex
		g2d.setColor(Color.black);
		
		for (int i=0; i<nodes_list.Nodes.size(); i++) {
		    int x1=(int)((node_set)nodes_list.Nodes.get(i)).getX();
		    int y1=(int)((node_set)nodes_list.Nodes.get(i)).getY();

		    g2d.fillOval(x1*zoom-node_width/2,y1*zoom-node_width/2,node_width,node_width);
			
		}


	}
	
}

