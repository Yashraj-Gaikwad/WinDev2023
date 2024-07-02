use Win32::OLE;

# CLSID of CLSID_MyMath to Create OLE Automation WIN32OLE object
my $obj = Win32::OLE -> new("{26E23E71-0117-4390-BB23-14E797DADED6}");

my $num1 = 40;
my $num2 = 15;

my $resultAddition = $obj -> SumOfTwoIntegers($num1, $num2);
print "$num1 + $num2 = $resultAddition \n";

my $resultSubtraction = $obj -> SubtractionOfTwoIntegers($num1, $num2);
print "$num1 - $num2 = $resultSubtraction \n";

