/**
 * 
 * Author : Benjamn Kataliko Viranga
 * ID : 8842942
 * 
 * Class : CSI3531 - Summer 2020 - Devoir 2
 * 
 * University of Ottawa 
 * 
 * How to run from the command line ?
 * 
 * javac Q2.java
 * java Q2 number
 * 
 * where number is an integer
 * 
 * java Q2 will also work and you will enter the value directly in the program
 */

import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

class Fibonacci extends Thread
{
    private String name;
    private BlockingQueue<Integer> queue;
    private int userValue;

    public Fibonacci(String name, BlockingQueue<Integer> queue, int userValue)
    {
        // Thread name
        this.name = name;
        // Queue for sharing Data between Thread
        this.queue = queue;
        // Number of Fibonacci number requested by the user
        this.userValue = userValue;
    }

    public void run()
    {
        System.out.println("\nStarting Child Thread : "+ name);
        int x1 = 0;
        int x2 = 1;

        try{
            // Fibonacci Algorithm
            for (int i = 0; i < this.userValue; i++)
            {
                // Add element in queue
                this.queue.put(x1);
                // Fibonacci
                int sum = x1 + x2;
                x1 = x2;
                x2 = sum;
            }
        }catch(InterruptedException e)
        {
            e.printStackTrace();
        }

        System.out.println("End of the Thread " + this.name);
    }
}



class Q2
{
    public static void main(String[] args)
    {

        //user value
        int user_value;

        if(args.length == 0 || args.length > 1)
        {
            // Only expect one Integer for the program to run
            System.out.println("Expected only one parameter for the program to run.");
            Scanner sc = new Scanner(System.in);
            System.out.printf("Enter an integer : ");
            user_value = sc.nextInt();
            sc.close();

        }
        else user_value = Integer.parseInt(args[0]); //get the first parameter of the command line
       
        // Queue for sharing data between Threads
        BlockingQueue<Integer> queue = new ArrayBlockingQueue<Integer>(user_value);

        
       //Thread for fibonacci
       Fibonacci fibonacci = new Fibonacci("Child Fibonacci", queue, user_value);
       fibonacci.start();
       try
       {
           fibonacci.join(); //wait for the fibonacci (child) to complete
       }
       catch(InterruptedException e)
       {
           e.printStackTrace();
       }

       System.out.printf("\nOutputting %d fibonacci number(s) :\n ", user_value );

       //Empty the queue
       while(queue.peek() != null)
       {
           System.out.printf("%d ", queue.poll());
       }

       System.out.println("\nParent Thread Terminating\n");
      
    }
}