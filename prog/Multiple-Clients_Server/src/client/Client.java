package client;

import java.net.*;
import java.io.*;

import util.Global;

class Client {

	public static void main(String args[]) throws IOException {
		int port = (args.length < 1) ? Global.DEFAULT_PORT : Integer.parseInt(args[0]);

		Socket soc = null;
		BufferedReader br = null;
		DataOutputStream dos = null;
		BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));

		try {
			soc = new Socket(InetAddress.getLocalHost(), port, InetAddress.getLocalHost(), Global.RANDOM_PORT);
			br = new BufferedReader(new InputStreamReader(soc.getInputStream()));
			dos = new DataOutputStream(soc.getOutputStream());
		} catch (Exception e) {
			System.out.println(e.getMessage());
			System.exit(0);
		}

		System.out.println("Welcome message:" + br.readLine());
		String incomingMessage = "";
		String outgoingMessage = "";
		do{
			outgoingMessage = userInput.readLine();
			dos.writeBytes(outgoingMessage);
			dos.write(13);
			dos.write(10);
			dos.flush();
			incomingMessage = br.readLine();
			System.out.println("From server :" + incomingMessage);
		}while(!(Global.END_CONNECTION.equals(outgoingMessage)));

		br.close();
		dos.close();
		soc.close();
	}
}
