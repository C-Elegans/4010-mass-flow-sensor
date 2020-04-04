// oscData library
// Author: Tom Fallon
// Copyright 2020

import java.net.*;
import java.io.*;
import javax.swing.*;      // Packages used
import java.awt.*;
import java.awt.event.*;
import java.util.Scanner;

public class oscData implements IData
{
	// Declare Gloabl Variables
	private String serverName;
	private Socket sock = null;
	private BufferedReader in = null;
	private PrintWriter out = null;
	// Empty constructor for interface access
	public oscData(AnalyzeGUI agui)
	{
		try
		{
			// Hard-code our socket data to BBB for now
			sock = new Socket( "192.168.7.2", 8081 );
		    //sock = new Socket("127.0.0.1", 8081);

			in = new BufferedReader(new InputStreamReader( sock.getInputStream() ) );
			out = new PrintWriter( sock.getOutputStream() );

		}
		catch(IOException e){}
	}

	// Transceiver method for connecting and communicating to Reverse server.
	// Single socket with writer and reader was used for bi-directional communication.
	public void xmit(String sendStr)
	{
		out.println( sendStr );
		out.flush();
	}

	public String rcrv()
	{
		String t;

		try
		{
			// readLine method seeks a \n or \r\f terminator
			t = in.readLine();
		}
		catch(IOException ioe2)
		{
			return ioe2.toString();
		}

		return t;
	}

	// Initialize our Server Name
	public void setName(String nameStr)
	{
		serverName = nameStr;
	}
}
