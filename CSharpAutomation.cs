// This is a namespace
using System;

// Sub namespace for Interoperability
using System.Runtime.InteropServices;

// Our Dll file
using AutomationTypeLibraryForDotNet;

// Main Class
// Name of class same as file name, usually
public class CSharpAutomation
{
	
	// Entry Point function
	// M is captial of Main()
	public static void Main()
	{
		// Create an instance of COM class
		CMyMathClass objCMyMathClass = new CMyMathClass();
		
		// Cast the COM object to an Interface IMyMath
		IMyMath objCMyMathClass = (IMyMath)objCMyMathClass;
		
		// Declare variables
		int num1 = 75;
		int num2 = 25;
		int sum,sub;
		
		// Call SumOfTwoIntegers
		sum = objCMyMathClass.SumOfTwoIntegers(num1, num2);		// Only 2 params, because no pointer in C sharp, idl file puts 3rd param as sum
		
		// Print the results
		Console.WriteLine("Sum Of " + num1 + " And " + num2 + " Is " + sum);

		// Call SubtractionOfTwoIntegers
		sub = objCMyMathClass.SubtractionOfTwoIntegers(num1, num2);
		
		// Print the results
		Console.WriteLine("Subtraction Of " + num1 + " And " + num2 + " Is " + sub);
		
	}

}

