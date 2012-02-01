
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.net.URL;

class frame extends JFrame {

    static int MIN_WIDTH=500;
    static int MIN_HEIGHT=500;

    JLabel msg=new JLabel("Topology Viewer...",Label.LEFT);
    JProgressBar bar;

    JButton b1=new JButton();
    JButton b2=new JButton();
    JButton b3=new JButton();

    URL url1 = frame.class.getResource("play.gif");
    URL url2 = frame.class.getResource("pause.gif");
    URL url3 = frame.class.getResource("exit.gif");

    Icon icon1 = new ImageIcon(url1);
    Icon icon2 = new ImageIcon(url2);
    Icon icon3 = new ImageIcon(url3);

    drawtopo topo=null;
    topoinfo info;
    
    nodes_reader reader;

    public frame(float zoom, topoinfo info, nodes_reader reader) {
	
	topo=new drawtopo(info.width,info.height,zoom,info.nodes_nbr,this);

	this.info=info;
	this.reader=reader;
	
	Panel p = new Panel(new BorderLayout());
	Panel p1 = new Panel(new FlowLayout());
	Panel p2 = new Panel(new BorderLayout());

	MIN_WIDTH=(int)(info.width*zoom+20);
	MIN_HEIGHT=(int)(info.height*zoom+100);

	setTitle("WSNET topology viewer");

	bar = new JProgressBar();
	bar.setValue(0);
	bar.setStringPainted(true);

	setLayout(new BorderLayout());

	p.add(msg,BorderLayout.NORTH);
       	p.add(topo, BorderLayout.CENTER);

	p.add(p2, BorderLayout.SOUTH);
	p2.add(p1,BorderLayout.WEST);
	
	b1.addActionListener(new traiter_event(reader, 1)); 
	b1.setIcon(icon1);

	b2.setIcon(icon2);
	b2.addActionListener(new traiter_event(reader, 2)); 

	b3.addActionListener(new traiter_event(reader, 3)); 
	b3.setIcon(icon3);

	p1.add(b1);
	p1.add(b2);
	p1.add(b3);

	p2.add(bar,BorderLayout.CENTER);
	
	add(p);

        setSize(MIN_WIDTH,MIN_HEIGHT);

	setLocationRelativeTo(getParent());
	setVisible(true);
	addWindowListener(new CloseFrameListener());
	
	addComponentListener(new java.awt.event.ComponentAdapter() {
		public void componentResized(ComponentEvent e) {
		    int width = getWidth();
		    int height = getHeight();
		    boolean resize = false;
		    if (width < frame.MIN_WIDTH) {
			resize = true;
			width = frame.MIN_WIDTH;
		    }
		    if (height < frame.MIN_HEIGHT) {
			resize = true;
			height = frame.MIN_HEIGHT;
		    }
		    if (resize) {
			setSize(width, height);
		    }
		}
	    }); 

    }

    public void refreshData(elements tmp, long time) {
	if (topo!=null) {
	    topo.refreshData(tmp,time);
	    updateInfo("Time = "+time+"ms /"+(long)(info.max_time*0.000001)+"ms", time);
	}
    }

    
    public void updateInfo(String m, long time) {
	msg.setText(m);
	int value = (int) ((time*100)/((int)(info.max_time*0.000001)));
	bar.setValue((int)value);
    }

    public void finished() {
	msg.setText("Time = "+(long)(info.max_time*0.000001)+"ms /"+(long)(info.max_time*0.000001)+"ms   Finished !");
	bar.setValue(100);
    }

    
    class CloseFrameListener extends WindowAdapter {
	public void windowClosing(WindowEvent event) {
	    System.exit(0);
	}
    }

    private class traiter_event implements ActionListener{
	nodes_reader reader;
	int code=0;

	public traiter_event(nodes_reader reader, int code) {
	    this.reader=reader;
	    this.code=code;
	}

	public void actionPerformed(ActionEvent e){
	    
	    switch(code) {
	    case 1 : reader.setActive(true); break;
	    case 2 : reader.setActive(false); break;
	    case 3 : System.exit(0); break;
	    }

	}
    } 

}

