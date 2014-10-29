package server;

import java.io.IOException;
import java.net.ServerSocket;

import util.Global;

class Server {

	/**
	 * @param args [port] [max accepted clients]
	 */
	public static void main(String args[]) {
		int port = (args.length < 1) ? Global.DEFAULT_PORT : Integer.parseInt(args[0]);
		int maxAcceptedClients = (args.length < 2) ? Global.MAX_ACCEPTED_CLIENTS : Integer.parseInt(args[1]);
		maxAcceptedClients = (maxAcceptedClients < 100) ? maxAcceptedClients : 99;

		ServerSocket ss = null;
		//Listening on a port:
		try {
			ss = new ServerSocket(port);
		} catch (IOException ioe) {
			System.out.println("Error finding port");
			System.exit(1);
		}

		System.out.println("################################");
		System.out.println("#      Listening on " + String.format("%05d", port) + "      #");
		System.out.println("# Waiting for clients (" + String.format("%02d", maxAcceptedClients) + " max) #");
		System.out.println("################################");

		int acceptedClients = 0;
		try {

			do {
				new Thread(new ClientHandler(ss.accept())).start();
				acceptedClients++;
			}while(acceptedClients < maxAcceptedClients);

		} catch (IOException ioe) {
			System.err.println("Server failed to accept a client");
		}
	}
}