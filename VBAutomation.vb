' Import Statements'
Imports System.Windows.Forms

Imports AutomationTypeLibraryForDotNet

'Class Definition'
Public Class VBAutomation
	Inherits Form
	
	'Constructor'
	Public Sub New()
		'Dim - Dimension'
		
		'object of CMyMathClass'
		Dim MyIDispatch As Object
		
		Dim MyRef As New CMyMathClass
		
		MyIDispatch = MyRef
		
		'Variable Declarations'
		Dim iNum1 = 175
		Dim iNum2 = 125
		
		'Call SumOfTwoIntegers'
		Dim iSum = MyIDispatch.SumOfTwoIntegers(iNum1, iNum2)
		
		'Positional Paramter, also in Swift'
		Dim str As String = String.Format("Sum of {0} And {1} is {2}", iNum1, iNum2, iSum)
		
		'Default Message Box with only 1 button, Ok'
		MsgBox(str)
		
		'Call SubtractionOfTwoIntegers'
		Dim iSub = MyIDispatch.SubtractionOfTwoIntegers(iNum1, iNum2)
		str = String.Format("Subtraction Of {0} And {1} Is {2}", iNum1, iNum2, iSub)
		
		'Default Message Box with only 1 button, Ok'
		MsgBox(str)

		'End works as Destroy Windows(hwnd)'
		End
	
	End Sub
	
	'Single Threaded Apartment'
	<STAThread()>
	Shared Sub Main()
		'Main Method'
		'Enables visual styles for GUI'
		Application.EnableVisualStyles()
		'Starts Windows Forms'
		Application.Run(New VBAutomation())
	End Sub
End Class




