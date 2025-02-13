<?php

$a = 2;
$b = 10;
if ($a < 4) {
    $b = "small";
    echo $b;
}
elseif ($a < 6) {
    $b = "medium";
    echo $b;
}
elseif($a < 10) {
    $b = "big";
    echo $b;
}
else {
    $b = "large";
    echo $b;
}

?>
