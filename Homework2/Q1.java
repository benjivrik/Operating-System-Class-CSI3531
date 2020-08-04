/**
 * 
 * Author : Benjamn Kataliko Viranga
 * 
 * Class : CSI3531 - Devoir 2
 * 
 * 
 * How to run from the command line ?
 * 
 * javac Q1.java
 * java Q1 number
 * 
 * where number is an integer
 * 
 * java Q1 will also work and you will enter the value directly in the program
 * 
 */
import java.lang.InterruptedException;
import java.lang.Thread;
import java.util.Scanner;

class PrimeNumber extends Thread
{
    private String name;
    private int user_value;

    public PrimeNumber(String name, int user_value)
    {
        this.name = name;
        this.user_value = user_value;
    }
    public void run()
    {
        System.out.println("Running Thread : " + this.name);

        //String for displaying the result
        String result = " ";

        //prime number starting at 2
        for(int index = 2; index <= this.user_value; index++)
        {
            boolean primeNumber = true;
            //check the divisors of the corresponding index
            for(int number =  2; number < index; number++)
            {
                if( index  % number == 0 )
                {
                    primeNumber =  false; //the number is not a prime number
                    break;
                }
            }
            //add to the result to be printed
            if(primeNumber)
            {
                result = result + index + " ";
            }
        }

        System.out.printf("The following numbers are the prime numbers less than or equal to %d\n", this.user_value);
        System.out.printf("%s\n", result);
        System.out.printf("End of Thread : %s.\n", name);
        
    }
}
public class Q1
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

        //Thread  userThread      = new UserInput("User Input Thread", queue);
        Thread  primeNumberThread = new PrimeNumber("Prime Number Thread", user_value);


        primeNumberThread.start(); //start the prime number thread

        try{
            //make sure this primeNumberThread is terminated before printing
            //end of program
            primeNumberThread.join();
        
        }catch(InterruptedException e)
        {
            e.printStackTrace();
        }
        
        System.out.println(">>>>>>>  END OF THE PROGRAM <<<<<<<");
    }

}
