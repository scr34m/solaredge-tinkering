<?php
$data = base64_decode(file_get_contents('mapping.txt'));
$key = base64_decode('OrdNqHZeamOr2VBIMfwlGg==');
$iv = base64_decode('PxEnyMyNvG40ixqM2A6PCA==');

$output = openssl_decrypt($data, 'AES-128-CBC', $key, OPENSSL_RAW_DATA, $iv);
file_put_contents('mapping.json', $output);
