package server;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

import util.Global;


public class ClientHandler implements Runnable {
	private BufferedReader br = null;
	private DataOutputStream dos = null;
	private Socket socket = null;
	private int id = 0;//Remote port, two different clients could have the same though!

	public ClientHandler(Socket socket) throws IOException {
		this.br = new BufferedReader(new InputStreamReader(socket.getInputStream()));;
		this.dos = new DataOutputStream(socket.getOutputStream());
		this.socket = socket;
		this.id  = this.socket.getPort();//This ID might not be unique!
		this.sendWelcomeMessage();
		System.out.println("++ #" + String.format("%05d", this.id));
	}

	private void sendWelcomeMessage() throws IOException {
		this.dos.write(new String("Write \"" + Global.END_CONNECTION + "\" to disconnect").getBytes());
		this.dos.write('\r');//send LF(0xa): Line Feed
		this.dos.write('\n');//send CR(0xd): Carriage Return
		this.dos.flush();
	}

	@Override
	public void run() {
		try {
			String incomingMessage;
			boolean quit = false;
			do {
				incomingMessage = br.readLine();
				incomingMessage = (incomingMessage != null) ? incomingMessage.trim() : "";//Because of the trim(), incomingMessage can be null after!

				quit = (Global.END_CONNECTION.equals(incomingMessage));//Apply the equals() to the not null string then. (or test if incoming is null)

				//Display the received message:
				System.out.println("#" + String.format("%05d", this.id) + ":" + incomingMessage);

				//Echo back the incoming message:
				if(incomingMessage != null)
					this.dos.write(incomingMessage.getBytes());
				else
					this.dos.write("#empty message received#".getBytes());

				this.dos.write('\r');//send LF(0xa): Line Feed
				this.dos.write('\n');//send CR(0xd): Carriage Return
				this.dos.flush();
			} while (!quit);

			this.dos.close();
			this.socket.close();

			System.out.println("-- #" + String.format("%05d", this.id));
		} catch (IOException e) {
			System.err.println("Error occurred:" + e.getMessage());
		}
	}

}
