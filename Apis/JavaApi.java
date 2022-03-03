package com.company;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Main test = new Main();
        test.start();
    }

    public void start() {
        try {
            Runtime runtime = Runtime.getRuntime();

            Process process = runtime.exec("run client.bat");
            InputStream data = process.getInputStream();
            OutputStream commandStream = process.getOutputStream();

            CommandThread commandThread = new CommandThread(commandStream);
            commandThread.start();

            ReceiverThread receiverThread = new ReceiverThread(data);
            receiverThread.start();

//            process.destroy(); //please kill process when done
        } catch (Exception exc) {
            System.out.println("Error start " + exc.getMessage());
        }
    }

    private void commandLoop(OutputStream commandStream) {
        Scanner scanner = new Scanner(System.in);
        String command;

        PrintWriter printOut = new PrintWriter(commandStream);

        while (true) {
            try {
                command = scanner.nextLine();
                if(command.compareTo("") != 0) {
                    printOut.println(command);
                    printOut.flush();
                }
            } catch (Exception e) {
                System.out.println("Error in command loop " + e.getMessage());
                return;
            }
        }
    }

    private void receiverLoop(InputStream data) {
        BufferedReader bufferedReader;
        String string;

        while (true) {
            try {
                 bufferedReader = new BufferedReader(new InputStreamReader(data));
                 while((string = bufferedReader.readLine()) != null) {
                     System.out.println(string);
                 }
            } catch (Exception e) {
                System.out.println("Error in receiver loop " + e.getMessage());
                return;
            }
        }
    }

    private class CommandThread extends Thread {
        OutputStream commandStream;

        CommandThread(OutputStream commandStream) {
            this.commandStream = commandStream;
        }

        public void run() {
            try {
                commandLoop(commandStream);
            } catch (Throwable t) {
                System.out.println("Error in Command Thread " + t.getMessage());
            }
        }
    }

    private class ReceiverThread extends Thread {
        InputStream data;

        ReceiverThread(InputStream data) {
            this.data = data;
        }

        public void run() {
            try {
                receiverLoop(data);
            } catch (Throwable t) {
                System.out.println("Error in Receiver Thread " + t.getMessage());
            }
        }
    }
}
