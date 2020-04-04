// Analyzer GUI - Virtual Scope
// Author: Tom Fallon
// Copyright 2020
// Rev 1.1

import javax.swing.*;      // Packages used
import java.awt.*;
import java.awt.event.*;
import javax.swing.JScrollBar;
import java.lang.Math;
import java.awt.Graphics;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.math.RoundingMode;
import java.text.DecimalFormat;

public class AnalyzeGUI extends JFrame implements AdjustmentListener, ActionListener, MouseListener, ItemListener
{
	// our command and parameter String
	String message = "";
	// our command identifier String
	String commandID;
	// our parameter identifier String
	String parameterID;

	// our GUI element event validator
	boolean validEvent = false;

	Scrollbar acqSB = new Scrollbar(Scrollbar.VERTICAL, 0,1,10,200);
	// Declare a state variable to determine if mouse release is from
	// the scrollbar or not
	private boolean acqSBValid = false;
	// Create a delay variable to vary the display speed
	// Its default delay is 1ms to match the server
	private int delay = 1;
	private JPanel p1;
	private JPanel p2;
	private JPanel p3;
	private JPanel p4;
	private JButton startButton;
	private JButton connectButton;
	private boolean connectState;
	private JLabel acqLabel;

	// Create our checkbox, zoom label, and zoom state variable
	private Checkbox zoomChk;
	private JLabel zoomLabel;
	private boolean zoomValid = false;

	// Single thread state object for Scan
	private boolean single = true;
	// Repaint state object
	private boolean repaint = false;

	// Create our accessor types
    private oscData accessor;
	private static IData accessorItf;

	public AnalyzeGUI(String s)
	{
		super(s);

	  	getContentPane().setLayout( new BorderLayout());

	  	p1 = new JPanel();
	  	p1.setBackground(Color.lightGray);

	  	p2 = new JPanel();
	  	p2.setBackground(new Color(255,120,50));
	  	p2.setLayout(new GridLayout(2,1,2,2));
		acqLabel = new JLabel("Acq Rate 10");
		p2.add(acqLabel);
	  	acqSB.addAdjustmentListener(this);
	  	acqSB.addMouseListener(this);
	  	acqSB.setBackground(new Color(220,200,20));
	  	p2.add(acqSB);
	  	// Temporarily disable the scrollbar
	  	acqSB.setEnabled(false);

	  	//JPanel p3 = new JPanel();
		p3 = new JPanel();
	  	p3.setBackground(new Color(255,120,50));
		p3.setLayout(new GridLayout(1,7,1,1));

		// Set our connect state variable to false
		connectState = false;
	  	connectButton = new JButton("Connect");
        connectButton.addActionListener(this);
		p3.add(connectButton);

		JLabel blank1 = new JLabel("");
		p3.add(blank1);

	  	startButton = new JButton("Start");
        startButton.addActionListener(this);
		p3.add(startButton);

		JLabel blank2 = new JLabel("");
		p3.add(blank2);
		JLabel blank3 = new JLabel("");
		p3.add(blank3);
		JLabel blank4 = new JLabel("");
		p3.add(blank4);

		zoomLabel = new JLabel("Oscope");
		p3.add(zoomLabel);
		zoomChk = new Checkbox("",null,true);
		zoomChk.addItemListener(this);
		p3.add(zoomChk);

		getContentPane().add(p1, "Center");
		getContentPane().add(p2, "East");
		getContentPane().add(p3, "South");

		// Instantiate our xcvr object for communications to our BBB; recast
		// this object as an IData interface object so that we are limited to those
		// methods specified in the interface only.
		accessor = new oscData(this);
		accessorItf = (IData)accessor;
	}
   public void actionPerformed(ActionEvent e)
	{
		if (e.getSource() == connectButton)
		{
			// If we haven't connected yet, connect to the BBB
			if (!connectState)
			{
				connectState = !connectState;
				connectButton.setText("Exit");
				connectButton.setBackground(Color.green);

				System.out.println("Entered actionPerformed 2");
			}
			else
			{
				System.exit(0);
			}
		}
		else
		{
			startButton.setText("Restart");

			// Only start one Scan thread
			if (single)
			{
				single = false;
				String message = "start";
				accessorItf.xmit(message);

				Scan scan = new Scan( );
				scan.start( );
			}
			else
				repaint = true;
		}
	}

	public static void main(String[] args)
	{
		AnalyzeGUI frame = new AnalyzeGUI("Virtual Analyzer");
		frame.setSize(700,500);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);

		String message = "AnalyzeGUI connected to BBB";
		accessorItf.xmit(message);
	}

	public void adjustmentValueChanged(AdjustmentEvent e)
	{
		if (e.getSource() == acqSB)
		{
			acqLabel.setText("Acq Rate \n" + Integer.toString(acqSB.getValue()));
			// Set state variable true so that mouse release knows
			// it was truly a scollbar event
			acqSBValid = true;
		}
	}
// Checkbox event. Toggle between Oscope and Zoom
public void itemStateChanged(ItemEvent ie)
{
	if (!zoomChk.getState())
	{
		zoomLabel.setText("Zoom 9:2");
		zoomValid = true;
	}
	else
	{
		zoomLabel.setText("Oscope");
		zoomValid = false;
	}
}

// Mouse events. Use mouse release event to verify that new depth setting is valid
public void mousePressed(MouseEvent e) {}

public void mouseReleased(MouseEvent e)
{
	if (acqSBValid)
	{
		acqSBValid = false;
		// Send two subsequent messages to the server:
		// the first will indicate that the delay value
		// is to change, and the second by how much
		String msg1 = "delay";
		accessorItf.xmit(msg1);
		String msg2 = Integer.toString(acqSB.getValue());
		accessorItf.xmit(msg2);
		// Now update the AnalyzerGUI delay to be 10 times
		// faster than the server output
		delay = (int)acqSB.getValue()/10;
//		p1.repaint();
		repaint = true;
	}
}

public void mouseEntered(MouseEvent e) {}

public void mouseExited(MouseEvent e) {}

public void mouseClicked(MouseEvent e){}

public int normalize(float inputVoltage, boolean zoom)
{
	// Declare our old and new range values
	// Note: the old range corresponds to our ADC output
	// for our thermal sensor
	float old_min, old_max;

	if (zoom)
	{
		old_min = 0.5f;
		old_max = 0.9f;
	}
	else
	{
		old_min = 0.0f;
		old_max = 1.8f;
	}
	float new_min = 0f;
//	float new_max = 200f;
	float new_max = 400f;

	// Calculate relative position of value with its original range
	float old_rel = (inputVoltage - old_min)/(old_max - old_min);

	return (int)(old_rel*(new_max - new_min) + new_min);
}

void drawAxis(boolean zoom)
{
	Graphics g = p1.getGraphics();

	g.setColor(Color.black);
	// Set our label font and brush, then label our dependent axis
	Font labelFont = new Font("Arial", Font.PLAIN,8);
	g.setFont(labelFont);

	DecimalFormat df = new DecimalFormat("0.00");
	// Initialize our support types with defaults
	float pixel = 400f, y_position;
	float y_divisor = 1.0f;
	float pixelInc = 10f;
	float maxVal = 1.8f;

	if (zoom)
	{
		pixel = 400f;
		y_divisor = 1.0f;
		pixelInc = 10f;
		maxVal = 0.9f;
	}
	else
	{
		pixel = 1800f;
		y_divisor = 4.5f;
		pixelInc = 45f;
		maxVal = 1.8f;

	}

	// Label our axis
	for (int j=1;j<41;j++)
	{
		y_position = (maxVal - pixel/1000f);
		g.drawString(df.format(y_position),4,(int)(pixel/y_divisor));
		pixel -= pixelInc;
	}

	// drawLine method requires arguments x1,y1,x2,y2 as interger inputs
	// 400 is added to y values to shift downward on screen
	//Draw our axis
	g.setColor(Color.black);
	// Draw our independent axis
	g.drawLine(-1,200,600,200);
	// Draw our dependent axis with voltage labels
	g.drawLine(3,400,3,0);
}

public class Scan extends Thread
{
	// Declare our global graphics object
	private Graphics g;

	public void run()
	{
		g = p1.getGraphics();
		int y = 100;
		int stop = 600;
		int i;
		boolean first_point = true;
		// Draw and label our axis
		drawAxis(zoomValid);

		for (i=0; i<stop;i++)
		{
			// Add specified delay between each drawn dot in curve
			// Default delay is 1ms
			try
			{
				Thread.sleep(delay);
			}
			catch(InterruptedException ie){}

			g.setColor(Color.red);
			String t = accessorItf.rcrv();
//			System.out.println("Received from server " + t);
			float f = Float.parseFloat(t);
//			System.out.println("Received float from server " + f + "i = " + i);

			if (first_point)
			{
				first_point = false;
				// The first point is drawn with identical y values
				// Note: must subtract normalized value from offset,
				// because y axis increases downward
				int init_y = 400 - normalize(f,zoomValid);
				g.drawLine(i-1,init_y,i,init_y);
				// Set y equal to init_y because it is the first y
				// of the second line segment to be painted
				y = init_y;
			}
			else
			{
				// Second and subsequent pass through for-loop
				// therefore, y2 is a new ADC voltage
				int y2 = 400 - normalize(f,zoomValid);
				System.out.println("Y2 value is " + y2);
				g.drawLine(i-1,y,i,y2);
				y = y2;
				// Repaint the screen when at the right display edge
				if ((i > stop -2) || repaint)
				{
					repaint = false;
					p1.repaint();
					//Reset loop variable to 0 to reset counting
					i = 0;

					// Repaint the axis
					drawAxis(zoomValid);
				}
			}
		} //end of for-loop
	} //end of run
} //end of inner-class Scan
}