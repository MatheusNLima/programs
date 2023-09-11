<?php
// Windows
$output = `ipconfig`;

// Abre a tag HTML pre
echo '<pre>';

// Mostra o resultado na tela
echo $output;

// Fecha a tag HTML pre
echo '</pre>';
?>