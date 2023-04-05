
<?php
$sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
$msg = '2';
$len = strlen($msg);
socket_sendto($sock, $msg, $len, 0, '42.192.164.131',1520);
header("Location: http://helloqt.top/tv");
socket_close($sock);
?>
