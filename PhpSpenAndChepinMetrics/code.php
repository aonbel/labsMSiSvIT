<?php

class Person
{
	public $name, $surname, $age, $profession;

	function print_info()
	{
		echo "My name is " . $this->name . "\n";
		echo "My surname is " . $this->surname . "\n";
		echo "I'm " . $this->profession . "\n";
		echo "I'm " . $this->age . " years old";
		echo "\n";
	}
}


function print_numbers()
{
	for ($i = 1; $i <= 10; ++$i)
	{
		for ($j = 1; $j <= 10; ++$j)
		{
			if ($i % 2 == 1 && $j % 2 == 1)
			{
				echo ($i * $j);
			}
			else
			{
				echo " ";
			}
		}
		echo "\n";
	}
}


function print_squares()
{
	for ($i = 1; $i < 10; $i++)
	{
		$result = $i * $i;
    		if($result>80)
    		{
        		break;
    		}
    		echo $i . " в квадрате равно " . $result;
    		echo "\n";
	}
}


function print_squares_while()
{
	$counter = 0;
	while(++$counter<10)
	{
        	echo $counter . " в квадрате равно " . $counter * $counter;
        	echo "\n";
	}
}


function sort_data(array $data)
{
	$count_elements = count($data);
	for ($i=0; $i < $count_elements; $i++)
	{
        	for ($j=$i; $j < $count_elements; $j++)
        	{
    		   	if ($data[$j] < $data[$i])
        	   	{
        	  		$temp = $data[$i];
        	         	$data[$i] = $data[$j];
        	 	        $data[$j] = $temp;
        	   	}
        	}
	}

	return $data;
}


function print_massive($data)
{
	$cnt = count($data);
	for ($i = 0; $i < $cnt; ++$i)
	{
		echo $data[$i] . " ";
	}

	echo "\n";
} 


function binary_search($arr, $x) 
{  
	if (count($arr) === 0) 
	{
			return false; 
	}
	$low = 0; 
	$high = count($arr) - 1; 
	
	while ($low <= $high)
	{ 
		 
		$mid = floor(($low + $high) / 2);
 
		if($arr[$mid] == $x)
		{ 
			echo $x . " exists";
			echo "\n";
			return; 
		} 

		if ($x < $arr[$mid]) 
		{ 
			$high = $mid -1; 
		} 
		else 
		{  
			$low = $mid + 1; 
		} 
	} 
	
	echo $x . " doesn't exists";
	echo "\n";
	return; 
} 



$Tom = new Person();
$Tom->name = "Tom";
$Tom->surname = "Parser";
$Tom->age = 20;
$Tom->profession = "programmer";
$data = [10, 6, 8, 2, 1, 3, 5, 9, 4, 7];
$sorted_data = sort_data($data);
$Tom->print_info();
print_numbers();
print_squares();
print_squares_while();
$data = sort_data($data);
print_massive($data);
binary_search($data, 5);
binary_search($data, 15);


?>
