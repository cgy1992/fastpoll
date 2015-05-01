#!/usr/bin/env php
<?php

$dir = realpath(__DIR__ . '/../');

function main($argc, $argv) {
  global $dir;
  
  $out = fopen("$dir/src/tpl.h", 'w+');
  fwrite($out, "#pragma once\n");
  fwrite($out, "/* this is a generated file, do not edit */\n");
  
  foreach (glob("$dir/src/tpl/*.tpl") as $view) {
    fwrite($out, '#define FSP_TPL_');
    fwrite($out, strtoupper(basename($view, '.tpl')));
    fwrite($out, " \"\" \\\n");
    
    $data = render($view);
    $data = str_replace("\r\n", "\n", $data);
    $data = str_replace('"', '\\"', $data);
    
    foreach (explode("\n", $data) as $line) {
      fwrite($out, '"');
      fwrite($out, $line);
      fwrite($out, "\\n\" \\\n");
    }
    
    fwrite($out, "\"\"\n");
  }
  
  fclose($out);
}

function render($view) {
  ob_start();
  include $view;
  return ob_get_clean();
}

/* ------------------------------------ */

if (!isset ($argc)) {
  $argc = $_SERVER['argc'];
  $argv = $_SERVER['argv'];
}

main($argc, $argv);
