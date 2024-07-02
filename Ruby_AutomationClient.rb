require 'win32ole'

# MyMath CLSID
objMyMath = WIN32OLE.new('{26E23E71-0117-4390-BB23-14E797DADED6}')

num1 = 700
num2 = 300

resultAddition = objMyMath.SumOfTwoIntegers(num1, num2)
puts "#{num1} + #{num2} = #{resultAddition}"

resultSubtraction = objMyMath.SubtractionOfTwoIntegers(num1, num2)
puts "#{num1} - #{num2} = #{resultSubtraction}"



